#include "ads1299.h"

ADS1299::ADS1299(uint8_t _chip_select) {
  // Save the ADS CS/SS as a variable in the class
  chip_select = _chip_select;
  // Initiate the ADS on the board
  pinMode(_chip_select, OUTPUT);
  digitalWrite(_chip_select, HIGH);
}

void ADS1299::init() {
  // recommended power up sequence requiers >Tpor (~32mS)
  delay(50);
  pinMode(ADS_RST, OUTPUT); // ADS Reset Pin
  digitalWrite(ADS_RST, LOW);  // reset pin connected to both ADS ICs
  delayMicroseconds(4);        // toggle reset pin
  digitalWrite(ADS_RST, HIGH); // this will reset the Daisy if it is present
  delayMicroseconds(20);       // recommended to wait 18 Tclk before using device (~8uS);
  // initalize the  data ready chip select and reset pins:
  pinMode(ADS_DRDY, INPUT); // we get data ready / DRDY asertion from the on-board ADS
  delay(40);
  resetADS(BOARD_ADS); // reset the on-board ADS registers, and stop DataContinuousMode
  delay(10);
  // tell on-board ADS to output its clk, set the data rate to 250SPS
  WREG(CONFIG1, (ADS1299_CONFIG1_DAISY | curSampleRate), BOARD_ADS);
  delay(40);
  resetADS(DAISY_ADS); // software reset daisy module if present
  delay(10);

    WREG(CONFIG1, (ADS1299_CONFIG1_DAISY_NOT | curSampleRate), BOARD_ADS); // turn off clk output if no daisy present
    numChannels = 8;                                                       // expect up to 8 ADS channels

  // DEFAULT CHANNEL SETTINGS FOR ADS
  defaultChannelSettings[POWER_DOWN] = NO;                  // on = NO, off = YES
  defaultChannelSettings[GAIN_SET] = ADS_GAIN24;            // Gain setting
  defaultChannelSettings[INPUT_TYPE_SET] = ADSINPUT_NORMAL; // input muxer setting
  defaultChannelSettings[BIAS_SET] = YES;                   // add this channel to bias generation
  defaultChannelSettings[SRB2_SET] = YES;                   // connect this P side to SRB2
  defaultChannelSettings[SRB1_SET] = NO;                    // don't use SRB1

  for (int i = 0; i < numChannels; i++) {
    for (int j = 0; j < 6; j++)
      channelSettings[i][j] = defaultChannelSettings[j]; // assign default settings
    useInBias[i] = true; // keeping track of Bias Generation
    useSRB2[i] = true;   // keeping track of SRB2 inclusion
  }
  boardUseSRB1 = daisyUseSRB1 = false;

	// write settings to the on-board and on-daisy ADS if present
  writeChannelSettings(); 

  WREG(CONFIG3, 0b11101100, BOTH_ADS);
  delay(1); // enable internal reference drive and etc.
  for (int i = 0; i < numChannels; i++) {
    // turn off the impedance measure signal
    leadOffSettings[i][PCHAN] = OFF;
    leadOffSettings[i][NCHAN] = OFF;
  }
  verbosity = false; // when verbosity is true, there will be Serial feedback
  firstDataPacket = true;
  streaming = false;
}


//print out the state of all the control registers
void ADS1299::print_registers() {
  boolean prevverbosityState = verbosity;
  verbosity = true;                   // set up for verbosity output
  RREGS(0x00, 0x0C, targetSS);        // read out the first registers
  delay(10);                          // stall to let all that data get read by the PC
  RREGS(0x0D, 0x17 - 0x0D, targetSS); // read out the rest
  verbosity = prevverbosityState;
}

uint8_t ADS1299::get_device_id() {
  uint8_t data = RREG(ID_REG);
  if (verbosity) { 
    // verbosity otuput
    printAll("On Board ADS ID ");
    printHex(data);
    printlnAll();
    sendEOT();
  }
  return data;
}

/**
 * # SPI COMMANDS
 * 
 * *note:* These commands and their respective definitions have been taken from
 * the ADS1299IPAG datsheet.
 * 
 * The ADS1299-x provides flexible configuration control. The commands control
 * and configure device operation. The commands are stand-alone, except for the
 * register read and write operations that require a second command byte plus
 * data. CS can be taken high or held low between commands but must stay low
 * for the entire command operation (especially for multi-byte commands).
 * System commands and the RDATA command are decoded by the device on the
 * seventh SCLK falling edge. The register read and write commands are decoded
 * on the eighth SCLK falling edge. Be sure to follow SPI timing requirements
 * when pulling CS high after issuing a command.
 */ 

void ADS1299::spi_transaction(
  uint8_t data, 
  uint8_t time_delay_ms1,
  uint8_t time_delay_ms2
) {
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  digitalWrite(chip_select, LOW);
  SPI.transfer(data);
  digitalWrite(chip_select, HIGH);
  delayMicroseconds(time_delay_ms1);
  SPI.endTransaction();
  delayMicroseconds(time_delay_ms2);
}

void ADS1299::WAKEUP() {
  spi_transaction(_WAKEUP, 0, 3);
}

void ADS1299::STANDBY() {
  spi_transaction(_STANDBY, 0, 0);
}

void ADS1299::RESET() {
  /**
   * There are two methods to reset the ADS1299-x: pull the RESET pin low, or
   * send the RESET command. When using the RESET pin, make sure to follow the
   * minimum pulse duration timing specifications before taking the pin back high.
   * The RESET command takes effect on the eighth SCLK falling edge of the command.
   * After a reset, 18 tCLK cycles are required to complete initialization of the
   * configuration registers to default states and start the conversion cycle. Note
   * that an internal reset is automatically issued to the digital filter whenever
   * the CONFIG1 register is set to a new value with a WREG command.
   */
  spi_transaction(_RESET, 12, 0);
}

void ADS1299::START() {
  /**
   * Pull the START pin high for at least 2 tCLK periods, or send the START
   * command to begin conversions. When START is low and the START command has
   * not been sent, the device does not issue a DRDY signal (conversions are
   * halted).
   * When using the START command to control conversions, hold the START pin low.
   * The ADS1299-x features two modes to control conversions: continuous mode
   * and single-shot mode. The mode is selected by SINGLE_SHOT
   * (bit 3 of the CONFIG4 register). In multiple device configurations,
   * the START pin is used to synchronize devices.
   */

  /**
   * The START command starts data conversions.
   * Tie the START pin low to control conversions by command. If conversions
   * are in progress, this command has no effect. The STOP command stops
   * conversions. If the START command is immediately followed by a STOP
   * command, then there must be a 4-tCLK cycle delay between them. When the
   * START command is sent to the device, keep the START pin low until the
   * STOP command is issued. (See the Start subsection of the SPI Interface
   * section for more details.) There are no SCLK rate restrictions for this
   * command and can be issued at any time. 
   */
  spi_transaction(_START, 0, 0);
}

void ADS1299::STOP() {
  /**
   * The STOP command stops conversions.
   * Tie the START pin low to control conversions by command. When the STOP
   * command is sent, the conversion in progress completes and further
   * conversions are stopped. If conversions are already stopped, this command
   * has no effect. There are no SCLK rate restrictions for this command and can
   * be issued at any time.
   */
  spi_transaction(_STOP, 0, 0);
}

void ADS1299::RDATAC() {
  spi_transaction(_RDATAC, 0, 3);
}

void ADS1299::SDATAC() {
  /**
   * The SDATAC command cancels the Read Data Continuous mode. There are no
   * SCLK rate restrictions for this command, but the next command must wait
   * for 4 tCLK cycles before completion.
   */
  spi_transaction(_SDATAC, 0, 10);
}

void ADS1299::RDATA() {
  /**
   * The RDATA command loads the output shift register with the latest data
   * when not in Read Data Continuous mode. Issue this command after DRDY goes
   * low to read the conversion result. There are no SCLK rate restrictions for
   * this command, and there is no wait time needed for the subsequent commands
   * or data retrieval SCLKs. To retrieve data from the device after the RDATA
   * command is issued, make sure either the START pin is high or the START
   * command is issued. When reading data with the RDATA command, the read
   * operation can overlap the next DRDY occurrence without data corruption. 
	 *
	 * We are reading the DOUT pin which is as follows. STAT data of 24bits,
	 * followed by the 8 channels' data, which each are 24bits.
   */

  uint8_t in_byte;
  // Lock and open the SPI comms channel
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  digitalWrite(chip_select, LOW);
	// Send the _RDATA SPI command
	SPI.transfer(_RDATA);
	// Read the first 24bits, which is the STAT registers.
  for (int i = 0; i < 3; i++) { 
		in_byte = SPI.transfer(0x00);
		// Read status register (1100 + LOFF_STATP + LOFF_STATN + GPIO[7:4])
    // boardStat = (boardStat << 8) | inByte; 
  }
	// Read the 8 channels' data. Each are 24 bits
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 3; j++) { 
			in_byte = SPI.transfer(0x00);
      boardChannelDataInt[i] = (boardChannelDataInt[i] << 8) | inByte;
    }
  }
  // Convert 3 byte 2's compliment to 4 byte 2's compliment
  for (int i = 0; i < 8; i++) {
    if (bitRead(boardChannelDataInt[i], 23) == 1)
      boardChannelDataInt[i] |= 0xFF000000;
    else
      boardChannelDataInt[i] &= 0x00FFFFFF;
  }
	// End the transaction
  digitalWrite(chip_select, HIGH);
  SPI.endTransaction();
}

uint8_t ADS1299::RREG(uint8_t register_address, uint8_t number_of_registers) {
  /**
   * This command reads register data.
   * The Register Read command is a two-byte command followed by the register
   * data output. The first byte contains the command and register address.
   * The second command byte specifies the number of registers to read – 1.
   * 
   * First command byte: 001r rrrr, where r rrrr is the starting register
   * address.
   * 
   * Second command byte: 000n nnnn, where n nnnn is the number of registers to
   * read – 1
   * 
   * The 17th SCLK rising edge of the operation clocks out the MSB of the first
   * register. When the device is in read data continuous mode, an SDATAC
   * command must be issued before the RREG command can be issued.
   * The RREG command can be issued any time. However, because this command is
   * a multi-byte command, there are SCLK rate restrictions depending on how
   * the SCLKs are issued to meet the tSDECODE timing.
   * Note that CS must be low for the entire command.
   */

  // 0x20 is 00100000
  uint8_t first_command = 0x20 + register_address;
  uint8_t second_command = number_of_registers - 0x01;
  // Lock and open the SPI comms channel
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  digitalWrite(chip_select, LOW);
	// Send the two initial SPI commands
  SPI.transfer(first_command);
  SPI.transfer(second_command);
	// Record the returned register data in an array
  for (uint8_t i = 0; i < number_of_registers; i++)
    reg_data[register_address + i] = SPI.transfer(0x00);
	// End the SPI transaction
  digitalWrite(chip_select, HIGH);
  SPI.endTransaction();

  // if (verbosity) { 
  //   for (int i = 0; i <= _numRegistersMinusOne; i++) {
  //     printRegisterName(_address + i);
  //     printHex(_address + i);
  //     printAll(", ");
  //     printHex(regData[_address + i]);
  //     printAll(", ");
  //     for (int j = 0; j < 8; j++) {
  //       char buf[3];
  //       printAll(itoa(bitRead(regData[_address + i], 7 - j), buf, DEC));
  //       if (j != 7)
  //         printAll(", ");
  //     }
  //     printlnAll();
  //     if (!commandFromSPI)
  //       delay(30);
  //   }
  // }

}

void ADS1299::WREG(uint8_t register_address, uint8_t _data) {
  /**
   * This command writes register data.
   * The Register Write command is a two-byte command followed by the register
   * data input. The first byte contains the command and register address.
   * The second command byte specifies the number of registers to write – 1.
   * 
   * First command byte: 010r rrrr, where r rrrr is the starting register
   * address.
   * 
   * Second command byte: 000n nnnn, where n nnnn is the number of registers to
   * write – 1. 
   * 
   */

  uint8_t first_command = 0x40 + register_address
  uint8_t second_command = number_of_registers - 0x01

  // Lock and open the SPI comms channel
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));
  digitalWrite(chip_select, LOW);
	// What is the starting register address?
  SPI.transfer(first_command);
  // Number of registers to write to
  // SPI.transfer(second_command);
	SPI.transfer(0x00);
  SPI.transfer(_data);
	// End the transaction
  digitalWrite(chip_select, HIGH);
  SPI.endTransaction();

  // if (verbosity) { 
  //   //  verbosity output
  //   printAll("Register ");
  //   printHex(_address);
  //   printlnAll(" modified.");
  //   sendEOT();
  // }
}

