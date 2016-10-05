#include "MPU6050.h"

MPU6050::MPU6050() : _devAddr(MPU6050_DEFAULT_ADDRESS)
{}

void MPU6050::initialize()
{
  setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  setFullScaleAccelRange(MPU6050_ACCEL_FS_8);
  setSleepEnabled(false);
}

bool MPU6050::isConnected()
{
  return getDeviceID() == MPU6050_DEFAULT_DEVICE_ID;
}

uint8_t MPU6050::getDeviceID()
{
  uint8_t buffer;
  I2Cdev::readBits(_devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, &buffer);
  return buffer;
}

void MPU6050::setSleepEnabled(bool enabled)
{
  I2Cdev::writeBit(_devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

bool MPU6050::isSleepEnabled()
{
  uint8_t buffer;
  I2Cdev::readBit(_devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, &buffer);
  return buffer;
}

void MPU6050::setFullScaleGyroRange(uint8_t range)
{
  I2Cdev::writeBits(_devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

uint8_t MPU6050::getFullScaleGyroRange()
{
  uint8_t buffer;
  I2Cdev::readBits(_devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, &buffer);
  return buffer;
}

/**
 * Enable gyro self test.
 * Gyro scale untouched!
 *
 * Self test for x, y, z are the last 3 bits (5-7)
 *
 * Enabled: 0x07 (0000 0111)
 * Disabled: 0x00 (0000 0000)
 *
 * @param enabled Trun self test on or off
 */
void MPU6050::setGyroSelfTestEnabled(bool enabled)
{
  uint8_t data = enabled ? 0x07 : 0x00;
  I2Cdev::writeBits(_devAddr, MPU6050_RA_GYRO_CONFIG, 7, 3, data);
}

uint8_t MPU6050::getGyroSelfTestEnabled(uint8_t *x, uint8_t *y, uint8_t *z, uint8_t *r)
{
  uint8_t buffer[1], count;

  if ((count = I2Cdev::readByte(_devAddr, MPU6050_RA_GYRO_CONFIG, buffer)) != 0)
  {
    *x = (buffer[0] & 0x80) >> 7; // mask 1000 0000
    *y = (buffer[0] & 0x40) >> 6; // mask 0100 0000
    *z = (buffer[0] & 0x20) >> 5; // mask 0010 0000
    *r = (buffer[0] & 0x18) >> 3; // mask 0001 1000
  }

  return count;
}

void MPU6050::setFullScaleAccelRange(uint8_t range)
{
  I2Cdev::writeBits(_devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

uint8_t MPU6050::getFullScaleAccelRange()
{
  uint8_t buffer;
  I2Cdev::readBits(_devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, &buffer);
  return buffer;
}

void MPU6050::setAccelSelfTestEnabled(bool enabled)
{
  uint8_t data = enabled ? 0x07 : 0x00;
  I2Cdev::writeBits(_devAddr, MPU6050_RA_ACCEL_CONFIG, 7, 3, data);
}

uint8_t MPU6050::getAccelSelfTestEnabled(uint8_t *x, uint8_t *y, uint8_t *z, uint8_t *r)
{
  uint8_t buffer[1], count;

  if ((count = I2Cdev::readByte(_devAddr, MPU6050_RA_ACCEL_CONFIG, buffer)) != 0)
  {
    *x = (buffer[0] & 0x80) >> 7; // mask 1000 0000
    *y = (buffer[0] & 0x40) >> 6; // mask 0100 0000
    *z = (buffer[0] & 0x20) >> 5; // mask 0010 0000
    *r = (buffer[0] & 0x18) >> 3; // mask 0001 1000
  }

  return count;
}

void MPU6050::getSelfTestFactoryTrim(float *destination)
{
  uint8_t buffer[1];
  uint8_t rawData[4];
  uint8_t selfTest[6];
  float factoryTrim[6];

  I2Cdev::readByte(_devAddr, MPU6050_RA_SELF_TEST_X, buffer);
  rawData[0] = buffer[0];

  I2Cdev::readByte(_devAddr, MPU6050_RA_SELF_TEST_Y, buffer);
  rawData[1] = buffer[0];

  I2Cdev::readByte(_devAddr, MPU6050_RA_SELF_TEST_Z, buffer);
  rawData[2] = buffer[0];

  I2Cdev::readByte(_devAddr, MPU6050_RA_SELF_TEST_A, buffer);
  rawData[3] = buffer[0];

  // Extract the acceleration test results first
  selfTest[0] = (rawData[0] >> 3) | (rawData[3] & 0x30) >> 4; // XA_TEST(5bit) Combine LSB SELF_TEST_X with MSB SELF_TEST_A (mask 0011 0000)
  selfTest[1] = (rawData[1] >> 3) | (rawData[3] & 0x0C) >> 4; // YA_TEST(5bit) Combine LSB SELF_TEST_Y with MSB SELF_TEST_A (mask 0000 1100)
  selfTest[2] = (rawData[2] >> 3) | (rawData[3] & 0x03) >> 4; // ZA_TEST(5bit) Combine LSB SELF_TEST_Z with MSB SELF_TEST_A (mask 0000 0011)

  // Extract the gyration test results first
  selfTest[3] = rawData[0] & 0x1F; // XG_TEST(5bit) mask 0001 1111
  selfTest[4] = rawData[1] & 0x1F; // YG_TEST(5bit) mask 0001 1111
  selfTest[5] = rawData[2] & 0x1F; // ZG_TEST(5bit) mask 0001 1111

  // Process results to allow final comparison with factory set values
  factoryTrim[0] = (4096.0 * 0.34) * (pow((0.92 / 0.34), (((float)selfTest[0] - 1.0) / 30.0))); // FT[Xa] factory trim calculation
  factoryTrim[1] = (4096.0 * 0.34) * (pow((0.92 / 0.34), (((float)selfTest[1] - 1.0) / 30.0))); // FT[Ya] factory trim calculation
  factoryTrim[2] = (4096.0 * 0.34) * (pow((0.92 / 0.34), (((float)selfTest[2] - 1.0) / 30.0))); // FT[Za] factory trim calculation
  factoryTrim[3] = (25.0 * 131.0) * (pow(1.046, ((float)selfTest[3] - 1.0))); // FT[Xg] factory trim calculation
  factoryTrim[4] = (-25.0 * 131.0) * (pow(1.046, ((float)selfTest[4] - 1.0))); // FT[Yg] factory trim calculation
  factoryTrim[5] = (25.0 * 131.0) * (pow(1.046, ((float)selfTest[5] - 1.0))); // FT[Yg] factory trim calculation

  for (int i = 0; i < 6; i++)
  {
    destination[i] = 100.0 + 100.0*((float)selfTest[i] - factoryTrim[i])/factoryTrim[i]; // Report percent differences
  }
}
