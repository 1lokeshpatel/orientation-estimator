#include <Wire.h>
#include "kalman_filter.h"

KalmanFilter kalmanRoll;
KalmanFilter kalmanPitch;

// global variables
const int MPU_addr = 0x68;

// accelerometer variables
float accelX, accelY, accelZ;
float accelRoll, accelPitch;

// gyroscope variables
float gyroX, gyroY, gyroZ;
float gyroRoll, gyroPitch, gyroYaw;
float currTime, prevTime, deltaT;

// orientation variables
float roll, pitch, yaw;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(19200);
  Wire.begin();
  Wire.beginTransmission(MPU_addr);

  // reset IMU
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
  currTime = 0;
}

void loop() {
  // put your main code here, to run repeatedly:

  readAccelerometer();
  readGyro();

  roll = kalmanRoll.estimate(accelRoll, gyroX, deltaT);
  pitch = kalmanPitch.estimate(accelPitch, gyroY, deltaT);
  yaw = gyroYaw;
}

void readAccelerometer(){
  // read accelerometer data
  Wire.beginTransmission(MPU_addr);

  // Start from register 0x3B, first data register for accelerometer
  Wire.write(0x3B);
  Wire.endTransmission(false);

  // read 6 registers for all accelerometer values
  Wire.requestFrom(MPU_addr, 6, true);

  // read Accelerometer data
  accelX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accelY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  accelZ = (Wire.read() << 8 | Wire.read()) / 16384.0;
  
  // calculating roll and pitch
  accelRoll = (atan(accelY / sqrt(pow(accelX, 2)+pow(accelZ,2))) * 180 / PI) - 0.58;
  accelPitch = (atan(-1* accelX/sqrt(pow(accelY,2) + pow(accelZ,2))) * 180 / PI) + 1.58;
}

void readGyro(){
  // reading gyroscope data
  prevTime = currTime;
  currTime = millis();
  deltaT = (currTime - prevTime)/1000;

  Wire.beginTransmission(MPU_addr);
  // start on register 0x43, first register of gyro data
  Wire.write(0x43);
  Wire.endTransmission(false);
  // read all 6 gyro data registers
  Wire.requestFrom(MPU_addr, 6, true);

  // read gyro data
  gyroX = (Wire.read() << 8 | Wire.read()) / 131.0;
  gyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  gyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;

  gyroX = gyroX + 0.56;
  gyroY = gyroY - 2;
  gyroZ = gyroZ + 0.79;

  // gyro data is in deg/s, deg/s * s = deg
  gyroRoll = gyroRoll + (gyroX * deltaT);
  gyroPitch = gyroPitch + (gyroY * deltaT);
  gyroYaw = gyroYaw + (gyroZ * deltaT);
}
