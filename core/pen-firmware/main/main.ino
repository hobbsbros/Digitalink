#include <Wire.h>

const int MPU = 0x68;
float AccX, AccY, AccZ;
float roll, pitch, yaw;

void setup () {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void loop () {
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ = (Wire.read() << 8 | Wire.read()) / 16384.0;
  Serial.print(AccX);
  Serial.print(" | ");
  Serial.print(AccY);
  Serial.print(" | ");
  Serial.println(AccZ);
}
