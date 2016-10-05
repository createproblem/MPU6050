#include <Arduino.h>
#include <MPU6050.h>

MPU6050 mpu6050;
void runSelfTest();

void setup()
{
  Serial.begin(9600);

  while (!Serial)
  {
    ;
  }

  Serial.print("Connection to MPU6050: ");
  mpu6050.isConnected() ? Serial.println("OK") : Serial.println("FAILED");
  mpu6050.setSleepEnabled(false);
  runSelfTest();
  // mpu6050.initialize();
}

void loop()
{
  Serial.println("======= CONFIG =======");

  Serial.print("Device ID: 0x");
  Serial.println(mpu6050.getDeviceID(), HEX);

  Serial.print("Sleep enabled: ");
  mpu6050.isSleepEnabled() ? Serial.println("YES") : Serial.println("NO");

  Serial.print("Gyro range: ");
  Serial.print("0x0"); Serial.println(mpu6050.getFullScaleGyroRange(), HEX);

  Serial.print("Accel range: ");
  Serial.print("0x0"); Serial.println(mpu6050.getFullScaleAccelRange(), HEX);
  Serial.println("");

  delay(5000);
}

void runSelfTest()
{
  mpu6050.setAccelSelfTestEnabled();
  delay(250);
  mpu6050.setGyroSelfTestEnabled();
  delay(250);

  float selfTest[6];
  mpu6050.getSelfTestFactoryTrim(selfTest);

  Serial.print("x-axis self test: acceleration trim within : "); Serial.print(selfTest[0],1); Serial.println("% of factory value");
  Serial.print("y-axis self test: acceleration trim within : "); Serial.print(selfTest[1],1); Serial.println("% of factory value");
  Serial.print("z-axis self test: acceleration trim within : "); Serial.print(selfTest[2],1); Serial.println("% of factory value");
  Serial.print("x-axis self test: gyration trim within : "); Serial.print(selfTest[3],1); Serial.println("% of factory value");
  Serial.print("y-axis self test: gyration trim within : "); Serial.print(selfTest[4],1); Serial.println("% of factory value");
  Serial.print("z-axis self test: gyration trim within : "); Serial.print(selfTest[5],1); Serial.println("% of factory value");

  if(selfTest[0] < 1.0f && selfTest[1] < 1.0f && selfTest[2] < 1.0f && selfTest[3] < 1.0f && selfTest[4] < 1.0f && selfTest[5] < 1.0f)
    Serial.println("Self test: PASS");
  else
    Serial.println("Self test: FAILED");
}
