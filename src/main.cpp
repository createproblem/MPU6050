#include <Arduino.h>
#include <MPU6050.h>

MPU6050 mpu6050;

void setup()
{
  Serial.begin(9600);

  while (!Serial)
  {
    ;
  }

  Serial.print("Connection to MPU6050: ");
  mpu6050.isConnected() ? Serial.println("OK") : Serial.println("FAILED");
  mpu6050.initialize();
}

void loop()
{
  Serial.println("======= CONFIG =======");

  Serial.print("Device ID: 0x");
  Serial.println(mpu6050.getDeviceID(), HEX);

  Serial.print("Sleep enabled: ");
  mpu6050.isSleepEnabled() ? Serial.println("YES") : Serial.println("NO");

  Serial.print("Gyro range: ");
  Serial.println(MPU6050::gyroRangeToString(mpu6050.getFullScaleGyroRange()));

  Serial.print("Accel range: ");
  Serial.println(MPU6050::accelRangeToString(mpu6050.getFullScaleAccelRange()));
  Serial.println("");
  Serial.println("");

  Serial.println("======= SELF TEST =======");
  uint8_t x, y, z;
  mpu6050.isGyroSelfTestEnabled(&x, &y, &z);

  delay(5000);
}
