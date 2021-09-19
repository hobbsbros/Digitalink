#include <Wire.h>

const int MPU = 0x68;

// Acceleration and gyroscopic data
float AccX, AccY, AccZ;
float GyrX, GyrY, GyrZ;
float AccPosX, AccPosY;
float GyrPosX, GyrPosY;
float PosX, PosY, PosZ;
float AccErrorX, AccErrorY;
float GyroErrorX, GyroErrorY, GyroErrorZ;

// Complementary filter calculation
const float sampling_frequency = 100;
const float time_delay = 100;
const float alpha = (time_delay/(time_delay + (1/sampling_frequency)));

// For integrating gyroscope data
float start_time, delta_time, current_time;

int c = 0;

void setup () {
  Serial.begin(9600);
  Wire.begin();

  // Begin transmission
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);

  calculate_error_values();

}

void loop () {
  // Set loop start time
  start_time = millis();
  
  // Get accelerometer data
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;

  // Calculate angles from accelerometer data
  AccPosX = (atan(AccY / sqrt(sq(AccX) + sq(AccZ))) * 180/PI) + AccErrorX;
  AccPosY = (atan(-1 * AccX / sqrt(sq(AccY) + sq(AccZ))) * 180 / PI) + AccErrorY;
  
  // Get gyroscopic data and integrate with respect to time
  Wire.beginTransmission(MPU);
  Wire.write(0x43);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  GyrX = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyrY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyrZ = (Wire.read() << 8 | Wire.read()) / 131.0;

  GyrX += GyroErrorX + 1.0*delta_time;
  GyrY += GyroErrorY;
  //GyrZ += GyroErrorZ;
  GyrZ += 1.1;

  // Calculate current loop time
  current_time = millis();
  delta_time = (current_time - start_time);

  GyrPosX += GyrX*delta_time;
  GyrPosY += GyrY*delta_time;

  // Use a complementary filter to combine the data
  PosX = (1 - alpha)*GyrPosX + alpha*AccPosX;
  PosY = (1 - alpha)*GyrPosY + alpha*AccPosY;
  PosZ += GyrZ*delta_time;
  
  Serial.print(PosX);
  Serial.print(",");
  Serial.println(PosY);
  //Serial.print(",");
  //Serial.println(PosZ);
}

void calculate_error_values () {
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
    AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
    AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;
    AccErrorX += (atan(AccY / sqrt(sq(AccX) + sq(AccZ))) * 180/PI);
    AccErrorY += (atan(-1 * AccX / sqrt(sq(AccY) + sq(AccZ))) * 180 / PI);
    c ++;
  }
  AccErrorX /= 200;
  AccErrorY /= 200;
  c = 0;
  while (c < 200) {
    Wire.beginTransmission(MPU);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU, 6, true);
    GyrX = (Wire.read() << 8 | Wire.read()) / 131.0;
    GyrY = (Wire.read() << 8 | Wire.read()) / 131.0;
    GyrZ = (Wire.read() << 8 | Wire.read()) / 131.0;
    GyroErrorX += GyrX;
    GyroErrorY += GyrY;
    GyroErrorZ += GyrZ;
    c ++;
  }
  GyroErrorX /= 200;
  GyroErrorY /= 200;
  GyroErrorZ /= 200;
}
