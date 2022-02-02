#include "ads1299.h"

ADS1299::ADS1299(uint8_t _chip_select) {
  // Save the ADS CS/SS as a variable in the class
  chip_select = _chip_select;
  // Initiate the ADS on the board
  pinMode(_chip_select, OUTPUT);
  digitalWrite(_chip_select, HIGH);
}

void ADS1299::initialize_ads() {
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

  writeChannelSettings(); // write settings to the on-board and on-daisy ADS if present

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

//////////////////////////////////////////////
///////////// STREAM METHODS /////////////////
//////////////////////////////////////////////

/**
* @description Used to activate a channel, if running must stop and start after...
* @param channelNumber int the channel you want to change
* @author AJ Keller (@pushtheworldllc)
*/
void OpenBCI_32bit_Library::streamSafeChannelActivate(byte channelNumber) {
  bool wasStreaming = streaming;
  // Stop streaming if you are currently streaming
  if (streaming)
    streamStop();
  // Activate the channel
  activateChannel(channelNumber);
  // Restart stream if need be
  if (wasStreaming)
    streamStart();
}

/**
* @description Used to deactivate a channel, if running must stop and start after...
* @param channelNumber int the channel you want to change
* @author AJ Keller (@pushtheworldllc)
*/
void OpenBCI_32bit_Library::streamSafeChannelDeactivate(byte channelNumber) {
  bool wasStreaming = streaming;
  // Stop streaming if you are currently streaming
  if (streaming)
    streamStop();
  // deactivate the channel
  deactivateChannel(channelNumber);
  // Restart stream if need be
  if (wasStreaming)
    streamStart();
}

/**
* @description Used to set lead off for a channel, if running must stop and start after...
* @param `channelNumber` - [byte] - The channel you want to change
* @param `pInput` - [byte] - Apply signal to P input, either ON (1) or OFF (0)
* @param `nInput` - [byte] - Apply signal to N input, either ON (1) or OFF (0)
* @author AJ Keller (@pushtheworldllc)
*/
void OpenBCI_32bit_Library::streamSafeLeadOffSetForChannel(byte channelNumber, byte pInput, byte nInput) {
  bool wasStreaming = streaming;
  // Stop streaming if you are currently streaming
  if (streaming)
    streamStop();
  changeChannelLeadOffDetect(channelNumber);
  // leadOffSetForChannel(channelNumber, pInput, nInput);
  // Restart stream if need be
  if (wasStreaming)
    streamStart();
}

/**
* @description Used to set lead off for a channel, if running must stop and start after...
* @param see `.channelSettingsSetForChannel()` for parameters
* @author AJ Keller (@pushtheworldllc)
*/
void OpenBCI_32bit_Library::streamSafeChannelSettingsForChannel(byte channelNumber, byte powerDown, byte gain, byte inputType, byte bias, byte srb2, byte srb1) {
  bool wasStreaming = streaming;
  // Stop streaming if you are currently streaming
  if (streaming)
    streamStop();
  writeChannelSettings(channelNumber);
  // channelSettingsSetForChannel(channelNumber, powerDown, gain, inputType, bias, srb2, srb1);
  // Restart stream if need be
  if (wasStreaming)
    streamStart();
}

/**
* @description Used to report (Serial0.print) the default channel settings
*                  if running must stop and start after...
* @author AJ Keller (@pushtheworldllc)
*/
void OpenBCI_32bit_Library::streamSafeReportAllChannelDefaults(void) {
  bool wasStreaming = streaming;
  // Stop streaming if you are currently streaming
  if (streaming)
    streamStop();
  reportDefaultChannelSettings();
  // Restart stream if need be
  if (wasStreaming)
    streamStart();
}

/**
* @description Used to set all channels on Board (and Daisy) to the default
*                  channel settings if running must stop and start after...
* @author AJ Keller (@pushtheworldllc)
*/
void OpenBCI_32bit_Library::streamSafeSetAllChannelsToDefault(void) {
  bool wasStreaming = streaming;
  // Stop streaming if you are currently streaming
  if (streaming)
    streamStop();
  setChannelsToDefault();
  // Restart stream if need be
  if (wasStreaming)
    streamStart();
}

/**
* @description Used to set the sample rate
* @param sr {SAMPLE_RATE} - The sample rate to set to.
* @author AJ Keller (@pushtheworldllc)
*/
void OpenBCI_32bit_Library::streamSafeSetSampleRate(SAMPLE_RATE sr) {
  boolean wasStreaming = streaming;
  // Stop streaming if you are currently streaming
  if (streaming)
    streamStop();
  setSampleRate(sr);
  // Restart stream if need be
  if (wasStreaming)
    streamStart();
}

/**
 * Return an array of gains in coded ADS form i.e. 0-6 where 6 is x24 and so on.
 * @return  [description]
 */
uint8_t *OpenBCI_32bit_Library::getGains(void) {
  uint8_t gains[numChannels];
  for (uint8_t i = 0; i < numChannels; i++)
    gains[i] = channelSettings[i][GAIN_SET];
  return gains;
}

/**
* @description Call this to start the streaming data from the ADS1299
* @returns boolean if able to start streaming
*/
void OpenBCI_32bit_Library::streamStart() {
  // needs daisy functionality
  if (wifi.present && wifi.tx)
    // TODO: Remove this debug line
    wifi.sendGains(numChannels, getGains());
  streaming = true;
  startADS();
  if (curBoardMode == BOARD_MODE_DEBUG || curDebugMode == DEBUG_MODE_ON)
    Serial1.println("ADS Started");
}

/**
* @description Call this to stop streaming from the ADS1299
* @returns boolean if able to stop streaming
*/
void OpenBCI_32bit_Library::streamStop() {
  streaming = false;
  stopADS();
  if (curBoardMode == BOARD_MODE_DEBUG || curDebugMode == DEBUG_MODE_ON)
    Serial1.println("ADS Stopped");
}

//reset all the ADS1299's settings. Stops all data acquisition
void ADS1299::resetADS() {
  RESET();  // Send RESET command to default all registers
  SDATAC(); // Exit Read Data Continuous mode to communicate with ADS
  delay(100);
  // Turn off all channels
  for (int chan = 1; chan <= 8; chan++)
    deactivateChannel(chan);
}

void OpenBCI_32bit_Library::setChannelsToDefault(void) {
  for (int i = 0; i < numChannels; i++) {
    for (int j = 0; j < 6; j++)
      channelSettings[i][j] = defaultChannelSettings[j];
    useInBias[i] = true; // keeping track of Bias Generation
    useSRB2[i] = true;   // keeping track of SRB2 inclusion
  }
  boardUseSRB1 = daisyUseSRB1 = false;

  writeChannelSettings(); // write settings to on-board ADS

  for (int i = 0; i < numChannels; i++) { 
    // turn off the impedance measure signal
    leadOffSettings[i][PCHAN] = OFF;
    leadOffSettings[i][NCHAN] = OFF;
  }
  changeChannelLeadOffDetect(); // write settings to all ADS

  WREG(MISC1, 0x00, BOARD_ADS); // open SRB1 switch on-board
  if (daisyPresent)
    WREG(MISC1, 0x00, DAISY_ADS);
    // open SRB1 switch on-daisy
}

/**
* @description Writes the default channel settings over the serial port
*/
void OpenBCI_32bit_Library::reportDefaultChannelSettings(void) {
  char buf[7];
  buf[0] = getDefaultChannelSettingForSettingAscii(POWER_DOWN);     // on = NO, off = YES
  buf[1] = getDefaultChannelSettingForSettingAscii(GAIN_SET);       // Gain setting
  buf[2] = getDefaultChannelSettingForSettingAscii(INPUT_TYPE_SET); // input muxer setting
  buf[3] = getDefaultChannelSettingForSettingAscii(BIAS_SET);       // add this channel to bias generation
  buf[4] = getDefaultChannelSettingForSettingAscii(SRB2_SET);       // connect this P side to SRB2
  buf[5] = getDefaultChannelSettingForSettingAscii(SRB1_SET);       // don't use SRB1
  printAll((const char *)buf);
  sendEOT();
}

// write settings for ALL 8 channels for a given ADS board
// channel settings: powerDown, gain, inputType, SRB2, SRB1
void OpenBCI_32bit_Library::writeChannelSettings() {
  bool use_SRB1 = false;
  byte setting, startChan, endChan, targetSS;

  for (int b = 0; b < 2; b++) {
    if (b == 0) {
      targetSS = BOARD_ADS;
      startChan = 0;
      endChan = 8;
    }
    if (b == 1) {
      if (!daisyPresent)
        return;
      targetSS = DAISY_ADS;
      startChan = 8;
      endChan = 16;
    }

    SDATAC(targetSS);
    delay(1); // exit Read Data Continuous mode to communicate with ADS

    for (byte i = startChan; i < endChan; i++)
    { // write 8 channel settings
      setting = 0x00;
      if (channelSettings[i][POWER_DOWN] == YES)
      {
        setting |= 0x80;
      }
      setting |= channelSettings[i][GAIN_SET];       // gain
      setting |= channelSettings[i][INPUT_TYPE_SET]; // input code
      if (channelSettings[i][SRB2_SET] == YES)
      {
        setting |= 0x08;   // close this SRB2 switch
        useSRB2[i] = true; // remember SRB2 state for this channel
      }
      else
      {
        useSRB2[i] = false; // rememver SRB2 state for this channel
      }
      WREG(CH1SET + (i - startChan), setting, targetSS); // write this channel's register settings

      // add or remove this channel from inclusion in BIAS generation
      setting = RREG(BIAS_SENSP, targetSS); //get the current P bias settings
      if (channelSettings[i][BIAS_SET] == YES) {
        bitSet(setting, i - startChan);
        useInBias[i] = true; //add this channel to the bias generation
      } else {
        bitClear(setting, i - startChan);
        useInBias[i] = false; //remove this channel from bias generation
      }
      WREG(BIAS_SENSP, setting, targetSS);
      delay(1); //send the modified byte back to the ADS

      setting = RREG(BIAS_SENSN, targetSS); //get the current N bias settings
      if (channelSettings[i][BIAS_SET] == YES)
        bitSet(setting, i - startChan); //set this channel's bit to add it to the bias generation
      else
        bitClear(setting, i - startChan); // clear this channel's bit to remove from bias generation
      WREG(BIAS_SENSN, setting, targetSS);
      delay(1); //send the modified byte back to the ADS

      if (channelSettings[i][SRB1_SET] == YES)
        use_SRB1 = true; // if any of the channel setting closes SRB1, it is closed for all
    } // end of CHnSET and BIAS settings
  }   // end of board select loop
  if (use_SRB1) {
    for (int i = startChan; i < endChan; i++)
      channelSettings[i][SRB1_SET] = YES;
    WREG(MISC1, 0x20, targetSS); // close SRB1 swtich
    if (targetSS == BOARD_ADS)
      boardUseSRB1 = true;
    if (targetSS == DAISY_ADS)
      daisyUseSRB1 = true;
  } else {
    for (int i = startChan; i < endChan; i++)
      channelSettings[i][SRB1_SET] = NO;
    WREG(MISC1, 0x00, targetSS); // open SRB1 switch
    if (targetSS == BOARD_ADS)
      boardUseSRB1 = false;
    if (targetSS == DAISY_ADS)
      daisyUseSRB1 = false;
  }
}

// write settings for a SPECIFIC channel on a given ADS board
void OpenBCI_32bit_Library::writeChannelSettings(byte N) {
  byte setting, startChan, endChan, targetSS;
  if (N < 9) { 
    // channels 1-8 on board
    targetSS = BOARD_ADS;
    startChan = 0;
    endChan = 8;
  } else { 
    // channels 9-16 on daisy module
    if (!daisyPresent)
      return;
    targetSS = DAISY_ADS;
    startChan = 8;
    endChan = 16;
  }
  // function accepts channel 1-16, must be 0 indexed to work with array
  N = constrain(N - 1, startChan, endChan - 1); //subtracts 1 so that we're counting from 0, not 1
  // first, disable any data collection
  SDATAC(targetSS);
  delay(1); // exit Read Data Continuous mode to communicate with ADS

  setting = 0x00;
  if (channelSettings[N][POWER_DOWN] == YES)
    setting |= 0x80;
  setting |= channelSettings[N][GAIN_SET];       // gain
  setting |= channelSettings[N][INPUT_TYPE_SET]; // input code
  if (channelSettings[N][SRB2_SET] == YES) {
    setting |= 0x08;   // close this SRB2 switch
    useSRB2[N] = true; // keep track of SRB2 usage
  } else {
    useSRB2[N] = false;
  }
  WREG(CH1SET + (N - startChan), setting, targetSS); // write this channel's register settings

  // add or remove from inclusion in BIAS generation
  setting = RREG(BIAS_SENSP, targetSS); //get the current P bias settings
  if (channelSettings[N][BIAS_SET] == YES) {
    useInBias[N] = true;
    bitSet(setting, N - startChan); //set this channel's bit to add it to the bias generation
  } else {
    useInBias[N] = false;
    bitClear(setting, N - startChan); // clear this channel's bit to remove from bias generation
  }
  WREG(BIAS_SENSP, setting, targetSS);
  delay(1);                             //send the modified byte back to the ADS
  setting = RREG(BIAS_SENSN, targetSS); //get the current N bias settings
  if (channelSettings[N][BIAS_SET] == YES) {
    bitSet(setting, N - startChan); //set this channel's bit to add it to the bias generation
  } else {
    bitClear(setting, N - startChan); // clear this channel's bit to remove from bias generation
  }
  WREG(BIAS_SENSN, setting, targetSS);
  delay(1); //send the modified byte back to the ADS

  // if SRB1 is closed or open for one channel, it will be the same for all channels
  if (channelSettings[N][SRB1_SET] == YES) {
    for (int i = startChan; i < endChan; i++)
      channelSettings[i][SRB1_SET] = YES;
    if (targetSS == BOARD_ADS)
      boardUseSRB1 = true;
    if (targetSS == DAISY_ADS)
      daisyUseSRB1 = true;
    setting = 0x20; // close SRB1 swtich
  }
  if (channelSettings[N][SRB1_SET] == NO) {
    for (int i = startChan; i < endChan; i++)
      channelSettings[i][SRB1_SET] = NO;
    if (targetSS == BOARD_ADS)
      boardUseSRB1 = false;
    if (targetSS == DAISY_ADS)
      daisyUseSRB1 = false;
    setting = 0x00; // open SRB1 switch
  }
  WREG(MISC1, setting, targetSS);
}

//  deactivate the given channel.
void OpenBCI_32bit_Library::deactivateChannel(byte N)
{
  byte setting, startChan, endChan, targetSS;
  if (N < 9) {
    targetSS = BOARD_ADS;
    startChan = 0;
    endChan = 8;
  } else {
    if (!daisyPresent)
      return;
    targetSS = DAISY_ADS;
    startChan = 8;
    endChan = 16;
  }
  SDATAC(targetSS);
  delay(1);                                     // exit Read Data Continuous mode to communicate with ADS
  N = constrain(N - 1, startChan, endChan - 1); //subtracts 1 so that we're counting from 0, not 1

  setting = RREG(CH1SET + (N - startChan), targetSS);
  delay(1);             // get the current channel settings
  bitSet(setting, 7);   // set bit7 to shut down channel
  bitClear(setting, 3); // clear bit3 to disclude from SRB2 if used
  WREG(CH1SET + (N - startChan), setting, targetSS);
  delay(1); // write the new value to disable the channel

  //remove the channel from the bias generation...
  setting = RREG(BIAS_SENSP, targetSS);
  delay(1);                         //get the current bias settings
  bitClear(setting, N - startChan); //clear this channel's bit to remove from bias generation
  WREG(BIAS_SENSP, setting, targetSS);
  delay(1); //send the modified byte back to the ADS

  setting = RREG(BIAS_SENSN, targetSS);
  delay(1);                         //get the current bias settings
  bitClear(setting, N - startChan); //clear this channel's bit to remove from bias generation
  WREG(BIAS_SENSN, setting, targetSS);
  delay(1); //send the modified byte back to the ADS

  leadOffSettings[N][0] = leadOffSettings[N][1] = NO;
  changeChannelLeadOffDetect(N + 1);
}

void OpenBCI_32bit_Library::activateChannel(byte N) {
  byte setting, startChan, endChan, targetSS;
  uint8_t start_channel = 0;
  uint8_t end_channel = 8;

  N = constrain(N - 1, startChan, endChan - 1); // 0-7 or 8-15

  SDATAC(targetSS); // exit Read Data Continuous mode to communicate with ADS
  setting = 0x00;
  setting |= channelSettings[N][GAIN_SET];       // gain
  setting |= channelSettings[N][INPUT_TYPE_SET]; // input code
  if (useSRB2[N] == true) {
    channelSettings[N][SRB2_SET] = YES;
  } else {
    channelSettings[N][SRB2_SET] = NO;
  }

  if (channelSettings[N][SRB2_SET] == YES) {
    bitSet(setting, 3);
  } // close this SRB2 switch
  WREG(CH1SET + (N - startChan), setting, targetSS);
  // add or remove from inclusion in BIAS generation
  if (useInBias[N]) {
    channelSettings[N][BIAS_SET] = YES;
  } else {
    channelSettings[N][BIAS_SET] = NO;
  }
  setting = RREG(BIAS_SENSP, targetSS); //get the current P bias settings
  if (channelSettings[N][BIAS_SET] == YES) {
    bitSet(setting, N - startChan); //set this channel's bit to add it to the bias generation
    useInBias[N] = true;
  } else {
    bitClear(setting, N - startChan); // clear this channel's bit to remove from bias generation
    useInBias[N] = false;
  }
  WREG(BIAS_SENSP, setting, targetSS);
  delay(1);                             //send the modified byte back to the ADS
  setting = RREG(BIAS_SENSN, targetSS); //get the current N bias settings
  if (channelSettings[N][BIAS_SET] == YES) {
    bitSet(setting, N - startChan); //set this channel's bit to add it to the bias generation
  } else {
    bitClear(setting, N - startChan); // clear this channel's bit to remove from bias generation
  }
  WREG(BIAS_SENSN, setting, targetSS);
  delay(1); //send the modified byte back to the ADS

  setting = 0x00;
  if (targetSS == BOARD_ADS && boardUseSRB1 == true)
    setting = 0x20;
  if (targetSS == DAISY_ADS && daisyUseSRB1 == true)
    setting = 0x20;
  WREG(MISC1, setting, targetSS); // close all SRB1 swtiches
}

// change the lead off detect settings for all channels
void OpenBCI_32bit_Library::changeChannelLeadOffDetect()
{
  byte setting, startChan, endChan, targetSS;

  for (int b = 0; b < 2; b++)
  {
    if (b == 0)
    {
      targetSS = BOARD_ADS;
      startChan = 0;
      endChan = 8;
    }
    if (b == 1)
    {
      if (!daisyPresent)
      {
        return;
      }
      targetSS = DAISY_ADS;
      startChan = 8;
      endChan = 16;
    }

    SDATAC(targetSS);
    delay(1); // exit Read Data Continuous mode to communicate with ADS
    byte P_setting = RREG(LOFF_SENSP, targetSS);
    byte N_setting = RREG(LOFF_SENSN, targetSS);

    for (int i = startChan; i < endChan; i++)
    {
      if (leadOffSettings[i][PCHAN] == ON)
      {
        bitSet(P_setting, i - startChan);
      }
      else
      {
        bitClear(P_setting, i - startChan);
      }
      if (leadOffSettings[i][NCHAN] == ON)
      {
        bitSet(N_setting, i - startChan);
      }
      else
      {
        bitClear(N_setting, i - startChan);
      }
      WREG(LOFF_SENSP, P_setting, targetSS);
      WREG(LOFF_SENSN, N_setting, targetSS);
    }
  }
}

// change the lead off detect settings for specified channel
void OpenBCI_32bit_Library::changeChannelLeadOffDetect(byte N)
{
  byte setting, targetSS, startChan, endChan;

  if (N < 9)
  {
    targetSS = BOARD_ADS;
    startChan = 0;
    endChan = 8;
  }
  else
  {
    if (!daisyPresent)
    {
      return;
    }
    targetSS = DAISY_ADS;
    startChan = 8;
    endChan = 16;
  }

  N = constrain(N - 1, startChan, endChan - 1);
  SDATAC(targetSS);
  delay(1); // exit Read Data Continuous mode to communicate with ADS
  byte P_setting = RREG(LOFF_SENSP, targetSS);
  byte N_setting = RREG(LOFF_SENSN, targetSS);

  if (leadOffSettings[N][PCHAN] == ON) {
    bitSet(P_setting, N - startChan);
  } else {
    bitClear(P_setting, N - startChan);
  }
  if (leadOffSettings[N][NCHAN] == ON) {
    bitSet(N_setting, N - startChan);
  } else {
    bitClear(N_setting, N - startChan);
  }
  WREG(LOFF_SENSP, P_setting, targetSS);
  WREG(LOFF_SENSN, N_setting, targetSS);
}

void OpenBCI_32bit_Library::configureLeadOffDetection(byte amplitudeCode, byte freqCode)
{
  amplitudeCode &= 0b00001100; //only these two bits should be used
  freqCode &= 0b00000011;      //only these two bits should be used

  byte setting, targetSS;
  for (int i = 0; i < 2; i++) {
    if (i == 0)
      targetSS = BOARD_ADS;
    if (i == 1) {
      if (!daisyPresent)
        return;
      targetSS = DAISY_ADS;
    }
    setting = RREG(LOFF, targetSS); //get the current bias settings
    //reconfigure the byte to get what we want
    setting &= 0b11110000;    //clear out the last four bits
    setting |= amplitudeCode; //set the amplitude
    setting |= freqCode;      //set the frequency
    //send the config byte back to the hardware
    WREG(LOFF, setting, targetSS);
    delay(1); //send the modified byte back to the ADS
  }
}

//Configure the test signals that can be inernally generated by the ADS1299
void OpenBCI_32bit_Library::configureInternalTestSignal(byte amplitudeCode, byte freqCode)
{
  byte setting, targetSS;
  for (int i = 0; i < 2; i++) {
    if (i == 0)
      targetSS = BOARD_ADS;
    if (i == 1) {
      if (daisyPresent == false)
        return;
      targetSS = DAISY_ADS;
    }
    if (amplitudeCode == ADSTESTSIG_NOCHANGE)
      amplitudeCode = (RREG(CONFIG2, targetSS) & (0b00000100));
    if (freqCode == ADSTESTSIG_NOCHANGE)
      freqCode = (RREG(CONFIG2, targetSS) & (0b00000011));
    freqCode &= 0b00000011;                               //only the last two bits are used
    amplitudeCode &= 0b00000100;                          //only this bit is used
    byte setting = 0b11010000 | freqCode | amplitudeCode; //compose the code
    WREG(CONFIG2, setting, targetSS);
    delay(1);
    if (curBoardMode == BOARD_MODE_DEBUG || curDebugMode == DEBUG_MODE_ON) {
      Serial1.print("Wrote to CONFIG2: ");
      Serial1.print(setting, BIN);
    }
  }
}

void OpenBCI_32bit_Library::changeInputType(byte inputCode) {
  for (int i = 0; i < numChannels; i++)
    channelSettings[i][INPUT_TYPE_SET] = inputCode;

  // OLD CODE REVERT
  //channelSettingsArraySetForAll();

  writeChannelSettings();
}

// Start continuous data acquisition
// TODO: NEEDS ADS ADDRESS, OR BOTH?
void OpenBCI_32bit_Library::startADS(void) {
  sampleCounter = 0;
  sampleCounterBLE = 0;
  firstDataPacket = true;
  RDATAC(BOTH_ADS); // enter Read Data Continuous mode
  delay(1);
  START(BOTH_ADS); // start the data acquisition
  delay(1);
  isRunning = true;
}

/**
* @description Check status register to see if data is available from the ADS1299.
* @returns {boolean} - `true` if data is available
*/
bool OpenBCI_32bit_Library::isADSDataAvailable(void) {
  return (!(digitalRead(ADS_DRDY)));
}

// CALLED WHEN DRDY PIN IS ASSERTED. NEW ADS DATA AVAILABLE!
void OpenBCI_32bit_Library::updateChannelData(void) {
  // this needs to be reset, or else it will constantly flag us
  channelDataAvailable = false;

  lastSampleTime = millis();

  bool downsample = true;
  if (iSerial0.tx == false && iSerial1.baudRate > OPENBCI_BAUD_RATE_MIN_NO_AVG)
    downsample = false;

  updateBoardData(downsample);
  // if (daisyPresent)
  //   updateDaisyData(downsample);

  switch (curBoardMode) {
    case BOARD_MODE_ANALOG:
      auxData[0] = analogRead(A5);
      auxData[1] = analogRead(A6);
      if (!wifi.present)
        auxData[2] = analogRead(A7);
      break;
    case BOARD_MODE_DIGITAL:
      auxData[0] = digitalRead(11) << 8 | digitalRead(12);
      auxData[1] = (wifi.present ? 0 : digitalRead(13) << 8) | digitalRead(17);
      auxData[2] = wifi.present ? 0 : digitalRead(18);
      break;
    case BOARD_MODE_MARKER:
      if (newMarkerReceived)
      {
        auxData[0] = (short)markerValue;
        newMarkerReceived = false;
      }
      break;
    case BOARD_MODE_BLE:
    case BOARD_MODE_DEBUG:
    case BOARD_MODE_DEFAULT:
      break;
  }
}

void OpenBCI_32bit_Library::updateBoardData(void) {
  updateBoardData(true);
}

void OpenBCI_32bit_Library::updateBoardData(bool downsample) {
  byte inByte;
  int byteCounter = 0;

  if ((daisyPresent || curBoardMode == BOARD_MODE_BLE) && !firstDataPacket && downsample)
    for (int i = 0; i < OPENBCI_ADS_CHANS_PER_BOARD; i++) // shift and average the byte arrays
      lastBoardChannelDataInt[i] = boardChannelDataInt[i]; // remember the last samples

  csLow(BOARD_ADS); //  open SPI
  for (int i = 0; i < 3; i++) {
    inByte = xfer(0x00); //  read status register (1100 + LOFF_STATP + LOFF_STATN + GPIO[7:4])
    boardStat = (boardStat << 8) | inByte;
  }
  for (int i = 0; i < OPENBCI_ADS_CHANS_PER_BOARD; i++) {
    for (int j = 0; j < OPENBCI_ADS_BYTES_PER_CHAN; j++)
    { //  read 24 bits of channel data in 8 3 byte chunks
      inByte = xfer(0x00);
      boardChannelDataRaw[byteCounter] = inByte; // raw data goes here
      byteCounter++;
      boardChannelDataInt[i] = (boardChannelDataInt[i] << 8) | inByte; // int data goes here
    }
  }
  csHigh(BOARD_ADS); // close SPI

  // need to convert 24bit to 32bit if using the filter
  for (int i = 0; i < OPENBCI_ADS_CHANS_PER_BOARD; i++) { 
    // convert 3 byte 2's compliment to 4 byte 2's compliment
    if (bitRead(boardChannelDataInt[i], 23) == 1)
      boardChannelDataInt[i] |= 0xFF000000;
    else
      boardChannelDataInt[i] &= 0x00FFFFFF;
  }
  if ((daisyPresent || curBoardMode == BOARD_MODE_BLE) && !firstDataPacket && downsample) {
    byteCounter = 0;
    for (int i = 0; i < OPENBCI_ADS_CHANS_PER_BOARD; i++)
    { // take the average of this and the last sample
      meanBoardChannelDataInt[i] = (lastBoardChannelDataInt[i] + boardChannelDataInt[i]) / 2;
    }
    for (int i = 0; i < OPENBCI_ADS_CHANS_PER_BOARD; i++)
    { // place the average values in the meanRaw array
      for (int b = 2; b >= 0; b--)
      {
        meanBoardDataRaw[byteCounter] = (meanBoardChannelDataInt[i] >> (b * 8)) & 0xFF;
        byteCounter++;
      }
    }
  }

  if (firstDataPacket == true)
  {
    firstDataPacket = false;
  }
}

// Stop the continuous data acquisition
void OpenBCI_32bit_Library::stopADS()
{
  STOP(chip_select); // stop the data acquisition
  delay(1);
  SDATAC(chip_select); // stop Read Data Continuous mode to communicate with ADS
  delay(1);
  isRunning = false;
}

void OpenBCI_32bit_Library::printSerial(int i) {
  if (iSerial0.tx && !commandFromSPI)
    Serial0.print(i);
  if (iSerial1.tx)
    Serial1.print(i);
}

void OpenBCI_32bit_Library::printSerial(char c) {
  if (iSerial0.tx && !commandFromSPI)
    Serial0.print(c);
  if (iSerial1.tx)
    Serial1.print(c);
}

void OpenBCI_32bit_Library::printSerial(int c, int arg) {
  if (iSerial0.tx && !commandFromSPI)
    Serial0.print(c, arg);
  if (iSerial1.tx)
    Serial1.print(c, arg);
}

void OpenBCI_32bit_Library::printSerial(const char *c) {
  if (c != NULL)
    for (int i = 0; i < strlen(c); i++)
      printSerial(c[i]);
}

void OpenBCI_32bit_Library::printlnSerial(void) {
  printSerial("\n");
}

void OpenBCI_32bit_Library::printlnSerial(char c) {
  printSerial(c);
  printlnSerial();
}

void OpenBCI_32bit_Library::printlnSerial(int c) {
  printSerial(c);
  printlnSerial();
}

void OpenBCI_32bit_Library::printlnSerial(int c, int arg) {
  printSerial(c, arg);
  printlnSerial();
}

void OpenBCI_32bit_Library::printlnSerial(const char *c) {
  printSerial(c);
  printlnSerial();
}

void OpenBCI_32bit_Library::write(uint8_t b) {
  wifi.storeByteBufTx(b);
  writeSerial(b);
}

void OpenBCI_32bit_Library::writeSerial(uint8_t c) {
  if (iSerial0.tx)
    Serial0.write(c);
  if (iSerial1.tx)
    Serial1.write(c);
}

void OpenBCI_32bit_Library::ADS_writeChannelData() {
  ADS_writeChannelDataAvgDaisy();
  ADS_writeChannelDataNoAvgDaisy();
}

void OpenBCI_32bit_Library::ADS_writeChannelDataWifi(boolean daisy) {
  if (daisy)
    for (int i = 0; i < 24; i++)
      wifi.storeByteBufTx(daisyChannelDataRaw[i]);
  else // Send on board
    for (int i = 0; i < 24; i++)
      wifi.storeByteBufTx(boardChannelDataRaw[i]);
}

void OpenBCI_32bit_Library::ADS_writeChannelDataAvgDaisy() {
  if (iSerial0.tx || (iSerial1.tx && iSerial1.baudRate <= OPENBCI_BAUD_RATE_MIN_NO_AVG)) {
    if (daisyPresent) {
      // Code that runs with daisy present
      //CHECK SAMPLE ODD-EVEN AND SEND THE APPROPRIATE ADS DATA
      if (sampleCounter % 2 != 0)
        for (int i = 0; i < OPENBCI_NUMBER_BYTES_PER_ADS_SAMPLE; i++)
          writeSerial(meanBoardDataRaw[i]);
      else
        for (int i = 0; i < OPENBCI_NUMBER_BYTES_PER_ADS_SAMPLE; i++)
          writeSerial(meanDaisyDataRaw[i]);
      // Code that runs without the daisy present
    } else {
      for (int i = 0; i < 24; i++)
        writeSerial(boardChannelDataRaw[i]);
    }
  }
}

void OpenBCI_32bit_Library::ADS_writeChannelDataNoAvgDaisy() {
  if (iSerial1.tx && iSerial1.baudRate > OPENBCI_BAUD_RATE_MIN_NO_AVG) {
    // Don't run this function if the serial baud rate is not greater then the
    // minimum
    // Always write board ADS data
    for (int i = 0; i < OPENBCI_NUMBER_BYTES_PER_ADS_SAMPLE; i++)
      writeSerial(boardChannelDataRaw[i]);
    // Only write daisy data if present
    if (daisyPresent)
      for (int i = 0; i < OPENBCI_NUMBER_BYTES_PER_ADS_SAMPLE; i++)
        writeSerial(daisyChannelDataRaw[i]);
  }
}

//print out the state of all the control registers
void OpenBCI_32bit_Library::printADSregisters(int targetSS) {
  boolean prevverbosityState = verbosity;
  verbosity = true;                   // set up for verbosity output
  RREGS(0x00, 0x0C, targetSS);        // read out the first registers
  delay(10);                          // stall to let all that data get read by the PC
  RREGS(0x0D, 0x17 - 0x0D, targetSS); // read out the rest
  verbosity = prevverbosityState;
}

uint8_t ADS1299::get_device_id() {
  uint8_t data = RREG(ID_REG, chip_select);
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

//  THIS NEEDS CLEANING AND UPDATING TO THE NEW FORMAT
void ADS1299::RDATA() {
  //  use in Stop Read Continuous mode when DRDY goes low
  /**
   * The RDATA command loads the output shift register with the latest data
   * when not in Read Data Continuous mode. Issue this command after DRDY goes
   * low to read the conversion result. There are no SCLK rate restrictions for
   * this command, and there is no wait time needed for the subsequent commands
   * or data retrieval SCLKs. To retrieve data from the device after the RDATA
   * command is issued, make sure either the START pin is high or the START
   * command is issued. When reading data with the RDATA command, the read
   * operation can overlap the next DRDY occurrence without data corruption. 
   */
  byte inByte;     //  to read in one sample of the channels
  csLow(targetSS); //  open SPI
  xfer(_RDATA);    //  send the RDATA command
  for (int i = 0; i < 3; i++) { 
    //  read in the status register and new channel data
    inByte = xfer(0x00);
    boardStat = (boardStat << 8) | inByte; //  read status register (1100 + LOFF_STATP + LOFF_STATN + GPIO[7:4])
  }

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 3; j++) { 
      //  read in the new channel data
      inByte = xfer(0x00);
      boardChannelDataInt[i] = (boardChannelDataInt[i] << 8) | inByte;
    }
  }
  for (int i = 0; i < 8; i++) {
    // convert 3 byte 2's compliment to 4 byte 2's compliment
    if (bitRead(boardChannelDataInt[i], 23) == 1)
      boardChannelDataInt[i] |= 0xFF000000;
    else
      boardChannelDataInt[i] &= 0x00FFFFFF;
  }

  csHigh(targetSS); //  close SPI
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

  SPI.transfer(first_command);
  SPI.transfer(second_command);

  for (int i = 0; i < number_of_registers; i++)
    regData[_address + i] = SPI.transfer(0x00); //  add register byte to mirror array

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

  return regData[_address]; // return requested register value
}

void ADS1299::WREG(uint8_t register_address, uint8_t number_of_registers) {
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

  SPI.transfer(first_command);
  // Number of registers to read
  SPI.transfer(second_command);
  // SPI.transfer(data);
  
  uint8_t max_register_adds = register_address + number_of_registers;
  for (uint8_t i = register_address; i < max_register_adds; i++)
    SPI.transfer(regData[i]);
  

  digitalWrite(chip_select, HIGH);
  SPI.endTransaction();

  // byte opcode1 = _address + 0x40; //  WREG expects 010rrrrr where rrrrr = _address
  // csLow(target_SS);               //  open SPI
  // xfer(opcode1);                  //  Send WREG command & address
  // xfer(0x00);                     //  Send number of registers to read -1
  // xfer(_value);                   //  Write the value to the register
  // csHigh(target_SS);              //  close SPI
  // regData[_address] = _value;     //  update the mirror array

  // if (verbosity) { 
  //   //  verbosity output
  //   printAll("Register ");
  //   printHex(_address);
  //   printlnAll(" modified.");
  //   sendEOT();
  // }
}

void ADS1299::WREGS(byte _address, byte _numRegistersMinusOne, int targetSS) {
  byte opcode1 = _address + 0x40; //  WREG expects 010rrrrr where rrrrr = _address
  csLow(targetSS);                //  open SPI
  xfer(opcode1);                  //  Send WREG command & address
  xfer(_numRegistersMinusOne);    //  Send number of registers to read -1
  for (int i = _address; i <= (_address + _numRegistersMinusOne); i++) {
    xfer(regData[i]); //  Write to the registers
  }
  csHigh(targetSS);
  if (verbosity) {
    printAll("Registers ");
    printHex(_address);
    printAll(" to ");
    printHex(_address + _numRegistersMinusOne);
    printlnAll(" modified");
    sendEOT();
  }
}