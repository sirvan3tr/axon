/*
# LIS3H Accelerometer
- I2C/SPI digital output interface
- 16-bit data output
*/
#include "lis3dh.h"


void LIS3DH::init(byte g) {
  byte setting = g | 0x08;     // mask the g range for REG4
  pinMode(LIS3DH_DRDY, INPUT); // setup dataReady interupt from accelerometer
  write(TMP_CFG_REG, 0x00);    // Disable ADC inputs, enable temperature sensor
  write(CTRL_REG1, 0x08);      // Disable accel, low power mode
  write(CTRL_REG2, 0x00);      // Don't use the high pass filter
  write(CTRL_REG3, 0x00);      // No interrupts yet
  write(CTRL_REG4, setting);   // Set scale to g, high resolution
  write(CTRL_REG5, 0x00);      // No boot, no fifo
  write(CTRL_REG6, 0x00);
  write(REFERENCE, 0x00);
  DRDYpinValue = lastDRDYpinValue = digitalRead(LIS3DH_DRDY); // take a reading to seed these variables
}

void LIS3DH::enable(byte Hz) {
  LIS3DH::zero_axis_data();          // clear the axisData array so we don't get any stale news 
  byte setting = Hz | 0x07;          // mask the desired frequency
  LIS3DH::write(CTRL_REG1, setting); // set freq and enable all axis in normal mode
  LIS3DH::write(CTRL_REG3, 0x10);    // enable DRDY1 on INT1 (tied to PIC pin 0, LIS3DH_DRDY)
}

void LIS3DH::disable() {
  write(CTRL_REG1, 0x08); // power down, low power mode
  write(CTRL_REG3, 0x00); // disable DRDY1 on INT1
}

byte LIS3DH::get_device_id() { return read(WHO_AM_I); }

bool LIS3DH::data_available() {
  if ((read(STATUS_REG2) & 0x08) > 0)
    return true; // read STATUS_REG
  else
    return false;
}

bool LIS3DH::data_ready() {
  bool r = false;
  DRDYpinValue = digitalRead(LIS3DH_DRDY); // take a look at LIS3DH_DRDY pin
  if (DRDYpinValue != lastDRDYpinValue) {
  // if the value has changed since last looking
    if (DRDYpinValue == HIGH) // see if this is the rising edge
      r = true; // if so, there is fresh data!
    lastDRDYpinValue = DRDYpinValue; // keep track of the changing pin
  }
  return r;
}

void LIS3DH::writeAxisDataSerial(void) {
  for (int i = 0; i < 3; i++) {
    writeSerial(highByte(axisData[i])); // write 16 bit axis data MSB first
    writeSerial(lowByte(axisData[i]));  // axisData is array of type short (16bit)
  }
}

void LIS3DH::writeAxisDataForAxisSerial(uint8_t axis) {
  if (axis > 2)
    axis = 0;
  writeSerial(highByte(axisData[axis])); // write 16 bit axis data MSB first
  writeSerial(lowByte(axisData[axis]));  // axisData is array of type short (16bit)
}

void LIS3DH::writeAxisDataWifi(void) {
  for (int i = 0; i < 3; i++) {
    wifi.storeByteBufTx(highByte(axisData[i])); // write 16 bit axis data MSB first
    wifi.storeByteBufTx(lowByte(axisData[i]));  // axisData is array of type short (16bit)
  }
}

void LIS3DH::writeAxisDataForAxisWifi(uint8_t axis) {
  if (axis > 2)
    axis = 0;
  wifi.storeByteBufTx(highByte(axisData[axis])); // write 16 bit axis data MSB first
  wifi.storeByteBufTx(lowByte(axisData[axis]));  // axisData is array of type short (16bit)
}

void LIS3DH::zero_axis_data(void) {
  for (int i = 0; i < 3; i++)
    axisData[i] = 0;
}

byte LIS3DH::read(byte reg) {
  reg |= READ_REG;                  // add the READ_REG bit
  csLow(LIS3DH_SS);                 // take spi
  spi.transfer(reg);                // send reg to read
  byte inByte = spi.transfer(0x00); // retrieve data
  csHigh(LIS3DH_SS);                // release spi
  return inByte;
}

void LIS3DH::write(byte reg, byte value) {
  csLow(LIS3DH_SS);    // take spi

  spi.setMode(DSPI_MODE3);
  spi.setSpeed(4000000);
  digitalWrite(LIS3DH_SS, LOW);

  spi.transfer(reg);   // send reg to write
  spi.transfer(value); // write value
  csHigh(LIS3DH_SS);   // release spi
}

void transmit_spi_bus(byte device_add, byte register_add, byte data) {
  // 1. cs_low
  // 2. 
  SPI.setMode()
  SPI.setSpeed()

  SPI.transfer(register_add);
  SPI.transfer(data);

}


int LIS3DH::read16(byte reg) { // use for reading axis data.
  int inData;
  reg |= READ_REG | READ_MULTI;                            // add the READ_REG and READ_MULTI bits
  csLow(LIS3DH_SS);                                        // take spi
  spi.transfer(reg);                                       // send reg to start reading from
  inData = spi.transfer(0x00) | (spi.transfer(0x00) << 8); // get the data and arrange it
  csHigh(LIS3DH_SS);                                       // release spi
  return inData;
}

int LIS3DH::getX() { return read16(OUT_X_L); }
int LIS3DH::getY() { return read16(OUT_Y_L); }
int LIS3DH::getZ() { return read16(OUT_Z_L); }

void LIS3DH::update_axis_data() {
  axisData[0] = getX();
  axisData[1] = getY();
  axisData[2] = getZ();
}

void LIS3DH::read_all_regs() {
  byte inByte;
  for (int i = STATUS_REG_AUX; i <= WHO_AM_I; i++) {
    inByte = read(i);
    printAll("0x");
    printHex(i);
    printAll(" ");
    printlnHex(inByte);
    delay(20);
  }
  printlnAll();

  for (int i = TMP_CFG_REG; i <= INT1_DURATION; i++) {
    inByte = read(i);
    // printRegisterName(i);
    printAll("0x");
    printHex(i);
    printAll(" ");
    printlnHex(inByte);
    delay(20);
  }
  printlnAll();

  for (int i = CLICK_CFG; i <= TIME_WINDOW; i++) {
    inByte = read(i);
    printAll("0x");
    printHex(i);
    printAll(" ");
    printlnHex(inByte);
    delay(20);
  }
}