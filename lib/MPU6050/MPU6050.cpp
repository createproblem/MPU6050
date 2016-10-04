#include "MPU6050.h"

MPU6050::MPU6050() : _devAddr(MPU6050_DEFAULT_ADDRESS)
{}

void MPU6050::initialize()
{
  setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  setSleepEnabled(false);
}

bool MPU6050::isConnected()
{
  return getDeviceID() == MPU6050_DEFAULT_DEVICE_ID;
}

uint8_t MPU6050::getDeviceID()
{
  uint8_t buffer[MPU6050_WHO_AM_I_LENGTH];
  I2Cdev::readBits(_devAddr, MPU6050_RA_WHO_AM_I, MPU6050_WHO_AM_I_BIT, MPU6050_WHO_AM_I_LENGTH, buffer);
  return buffer[0];
}

void MPU6050::setSleepEnabled(bool enabled)
{
  I2Cdev::writeBit(_devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, enabled);
}

bool MPU6050::isSleepEnabled()
{
  uint8_t buffer[1];
  I2Cdev::readBit(_devAddr, MPU6050_RA_PWR_MGMT_1, MPU6050_PWR1_SLEEP_BIT, buffer);
  return buffer[0];
}

void MPU6050::setFullScaleGyroRange(uint8_t range)
{
  I2Cdev::writeBits(_devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, range);
}

uint8_t MPU6050::getFullScaleGyroRange()
{
  uint8_t buffer[MPU6050_GCONFIG_FS_SEL_LENGTH];
  I2Cdev::readBits(_devAddr, MPU6050_RA_GYRO_CONFIG, MPU6050_GCONFIG_FS_SEL_BIT, MPU6050_GCONFIG_FS_SEL_LENGTH, buffer);
  return buffer[0];
}

void MPU6050::setFullScaleAccelRange(uint8_t range)
{
  I2Cdev::writeBits(_devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, range);
}

uint8_t MPU6050::getFullScaleAccelRange()
{
  uint8_t buffer[MPU6050_ACONFIG_AFS_SEL_LENGTH];
  I2Cdev::readBits(_devAddr, MPU6050_RA_ACCEL_CONFIG, MPU6050_ACONFIG_AFS_SEL_BIT, MPU6050_ACONFIG_AFS_SEL_LENGTH, buffer);
  return buffer[0];
}
