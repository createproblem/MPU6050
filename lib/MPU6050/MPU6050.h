#ifndef MPU6050_H_
#define MPU6050_H_

#define MPU6050_DEFAULT_ADDRESS     0x68
#define MPU6050_DEFAULT_DEVICE_ID   0x34

#define MPU6050_RA_GYRO_CONFIG      0x1B
#define MPU6050_RA_ACCEL_CONFIG     0x1C
#define MPU6050_RA_PWR_MGMT_1       0x6B
#define MPU6050_RA_WHO_AM_I         0x75

#define MPU6050_GCONFIG_FS_SEL_BIT      4
#define MPU6050_GCONFIG_FS_SEL_LENGTH   2

#define MPU6050_GYRO_FS_250         0x00
#define MPU6050_GYRO_FS_500         0x01
#define MPU6050_GYRO_FS_1000        0x02
#define MPU6050_GYRO_FS_2000        0x03

#define MPU6050_ACONFIG_AFS_SEL_BIT         4
#define MPU6050_ACONFIG_AFS_SEL_LENGTH      2

#define MPU6050_ACCEL_FS_2          0x00
#define MPU6050_ACCEL_FS_4          0x01
#define MPU6050_ACCEL_FS_8          0x02
#define MPU6050_ACCEL_FS_16         0x03

#define MPU6050_PWR1_SLEEP_BIT          6

#define MPU6050_WHO_AM_I_BIT        6
#define MPU6050_WHO_AM_I_LENGTH     6

#include <I2Cdev.h>

class MPU6050
{
public:
  static String gyroRangeToString(uint8_t range)
  {
    String grange = "unknown";

    if (range == MPU6050_GYRO_FS_250)
        grange = "250";
    if (range == MPU6050_GYRO_FS_500)
      grange = "500";
    if (range == MPU6050_GYRO_FS_1000)
      grange = "1000";
    if (range == MPU6050_GYRO_FS_2000)
      grange = "2000";

    return grange;
  }

  static String accelRangeToString(uint8_t range)
  {
    String arange = "unknown";

    if (range == MPU6050_ACCEL_FS_2)
        arange = "2";
    if (range == MPU6050_ACCEL_FS_4)
      arange = "4";
    if (range == MPU6050_ACCEL_FS_8)
      arange = "8";
    if (range == MPU6050_ACCEL_FS_16)
      arange = "16";

    return arange;
  }

  MPU6050();
  void initialize();
  bool isConnected();
  uint8_t getDeviceID();
  void setSleepEnabled(bool enabled);
  bool isSleepEnabled();

  // Gyro config
  void setFullScaleGyroRange(uint8_t range);
  uint8_t getFullScaleGyroRange();
  void setGyroSelfTestEnabled(bool enabled);
  void isGyroSelfTestEnabled(uint8_t *x, uint8_t *y, uint8_t *z);

  // Accel config
  void setFullScaleAccelRange(uint8_t range);
  uint8_t getFullScaleAccelRange();

private:
  uint8_t _devAddr;
};

#endif /* MPU6050_H_ */
