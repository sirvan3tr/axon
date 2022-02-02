#ifndef ADS1299_H
#define ADS1299_H
/*
# ADS1299 ADC
The ADS1299-x is a low-noise, low-power, multichannel, simultaneously-sampling, 24-bit, delta-sigma (ΔΣ)
analog-to-digital converter (ADC) with an integrated programmable gain amplifier (PGA). These devices integrate
various EEG-specific functions that makes the family well-suited for scalable electrocardiogram (ECG),
electroencephalography (EEG) applications. These devices can also be used in high-performance, multichannel,
data acquisition systems by powering down the ECG or EEG-specific circuitry.

The devices have a highly-programmable multiplexer that allows for temperature, supply, input short, and bias
measurements. Additionally, the multiplexer allows any input electrodes to be programmed as the patient
reference drive. The PGA gain can be chosen from one of seven settings (1, 2, 4, 6, 8, 12, and 24). The ADCs in
the device offer data rates from 250 SPS to 16 kSPS. Communication to the device is accomplished using an
SPI-compatible interface. The device provides four general-purpose input/output (GPIO) pins for general use.
Multiple devices can be synchronized using the START pin.

The internal reference generates a low noise 4.5 V internal voltage when enabled and the internal oscillator
generates a 2.048-MHz clock when enabled. The versatile patient bias drive block allows the average of any
electrode combination to be chosen in order to generate the patient drive signal. Lead-off detection can be
accomplished by using a current source or sink. A one-time, in-band, lead-off option and a continuous, out-ofband, internal lead-off option are available.

# Power-up sequence
- Before device power up, all digital and analog inputs must be low. At the time of power up, keep all of these
signals low until the power supplies have stabilized.

* The ADS features two modes to control conversations:
  * Continuous mode
  * Single-shot mode

- **START**. pull START pin high for at least 2 t_{CLK} periods.
- **Reset**.
- **Power-down**.
- ** Data Ready**.

- **SPI interface**. CS, SCLK, DIN, DOUT.
- The ADS1299-x serial interface decodes commands in bytes and requires 4 tCLK cycles to decode and execute.
Therefore, when sending multi-byte commands (such as RREG or WREG), a 4 tCLK period must separate the
end of one byte (or command) and the next.
*/

// Type support library
#include <cstdint>

// ADS1299 SPI Command Definition Byte Assignments
enum SPICommand {
	_WAKEUP=0x02, // Wake-up from standby mode
	_STANDBY=0x04, // Enter Standby mode
	_RESET=0x06, // Reset the device registers to default
	_START=0x08, // Start and restart (synchronize) conversions
	_STOP=0x0A, // Stop conversion
	_RDATAC=0x10, // Enable Read Data Continuous mode (default mode at power-up)
	_SDATAC=0x11, // Stop Read Data Continuous mode
	_RDATA=0x12, // Read data by command supports multiple read back
	_RREG, // Read n nnnn registers starting at address r rrrr
	_WREG // Write n nnnn registers starting at address r rrrr
};

//ASD1299 Register Addresses
enum RegisterAddress {
};

#define ADS_ID	0x3E	// product ID for ADS1299
#define ID_REG  0x00	// this register contains ADS_ID
#define CONFIG1 0x01
#define CONFIG2 0x02
#define CONFIG3 0x03
#define LOFF 0x04
#define CH1SET 0x05
#define CH2SET 0x06
#define CH3SET 0x07
#define CH4SET 0x08
#define CH5SET 0x09
#define CH6SET 0x0A
#define CH7SET 0x0B
#define CH8SET 0x0C
#define BIAS_SENSP 0x0D
#define BIAS_SENSN 0x0E
#define LOFF_SENSP 0x0F
#define LOFF_SENSN 0x10
#define LOFF_FLIP 0x11
#define LOFF_STATP 0x12
#define LOFF_STATN 0x13
#define GPIO 0x14
#define MISC1 0x15
#define MISC2 0x16
#define CONFIG4 0x17

#define OUTPUT_NOTHING (0)  // quiet
#define OUTPUT_8_CHAN (1)   // not using Daisy module
#define OUTPUT_16_CHAN (2)  // using Daisy module
#define ON_BOARD	(8)			  // slave address for on board ADS
#define ON_DAISY	(3)				// slave address for daisy ADS
// CHANNEL SETTINGS
#define POWER_DOWN      (0)
#define GAIN_SET        (1)
#define INPUT_TYPE_SET  (2)
#define BIAS_SET        (3)
#define SRB2_SET        (4)
#define SRB1_SET        (5)
#define YES      	(0x01)
#define NO      	(0x00)

//gainCode choices
#define ADS_GAIN01 (0b00000000)	// 0x00
#define ADS_GAIN02 (0b00010000)	// 0x10
#define ADS_GAIN04 (0b00100000)	// 0x20
#define ADS_GAIN06 (0b00110000)	// 0x30
#define ADS_GAIN08 (0b01000000)	// 0x40
#define ADS_GAIN12 (0b01010000)	// 0x50
#define ADS_GAIN24 (0b01100000)	// 0x60

//inputType choices
#define ADSINPUT_NORMAL     (0b00000000)
#define ADSINPUT_SHORTED    (0b00000001)
#define ADSINPUT_BIAS_MEAS  (0b00000010)
#define ADSINPUT_MVDD       (0b00000011)
#define ADSINPUT_TEMP       (0b00000100)
#define ADSINPUT_TESTSIG    (0b00000101)
#define ADSINPUT_BIAS_DRP   (0b00000110)
#define ADSINPUT_BIAL_DRN   (0b00000111)

//test signal choices...ADS1299 datasheet page 41
#define ADSTESTSIG_AMP_1X (0b00000000)
#define ADSTESTSIG_AMP_2X (0b00000100)
#define ADSTESTSIG_PULSE_SLOW (0b00000000)
#define ADSTESTSIG_PULSE_FAST (0b00000001)
#define ADSTESTSIG_DCSIG (0b00000011)
#define ADSTESTSIG_NOCHANGE (0b11111111)

#define ADS1299_CONFIG1_DAISY (0b10110000)
#define ADS1299_CONFIG1_DAISY_NOT (0b10010000)

//Lead-off signal choices
#define LOFF_MAG_6NA        (0b00000000)
#define LOFF_MAG_24NA       (0b00000100)
#define LOFF_MAG_6UA        (0b00001000)
#define LOFF_MAG_24UA       (0b00001100)
#define LOFF_FREQ_DC        (0b00000000)
#define LOFF_FREQ_7p8HZ     (0b00000001)
#define LOFF_FREQ_31p2HZ    (0b00000010)
#define LOFF_FREQ_FS_4      (0b00000011)
#define PCHAN (0)
#define NCHAN (1)
#define OFF (0)
#define ON (1)

class ADS1299 {
  public:
    ADS1299(uint8_t _chip_select);
		// CS/SS of the ADS1299
    uint8_t chip_select;
		// We have 24 registers. They start from zero.
		uint8_t reg_data[24];

		void spi_transaction(
										uint8_t data,
										uint8_t time_delay_ms1,
										uint8_t time_delay_ms2
										);

		// SPI Commands:
    void START();
    void STOP();
    void RESET();
		void RDATAC();
		void SDATAC();
		void RDATA();
		void RREG(uint8_t register_address, uint8_t number_of_registers);
		void WREG(uint8_t register_address, uint8_t _data);
};

#endif
