#include <Wire.h>

const int MPU = 0x68;
float AccX1, AccX1, AccZ1, AccX2, AccY2, AccZ2, VelX1, VelY1, VelZ1, VelX2, VelY2, VelZ2, PosX1, PosY1, PosZ1, PosX2, PosY2, PosZ2, Delta, TipX, TipY, TipZ;


float VelocityA(float a, float v){
  float vel;
  vel= v+ a*Delta/2;
}

float Position(float x, float v, float a){
  float pos;
  pos = x + v*Delta + a*Delta*Delta/2;
  return pos;
}

float VelocityB(float v, float a){
  float vel;
  vel = v + a*Delta/2;
  return vel;
}


void setup () {
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);
}

void loop () {
  PosX1 = Position(PosX1,VelX1,AccX1);
  PosX2 = Position(PosX2,VelX2,AccX2);
  PosY1 = Position(PosY1,VelY1,AccY1);
  PosY2 = Position(PosY2,VelY2,AccY2);
  PosZ1 = Position(PosZ1,VelZ1,AccZ1);
  PosZ2 = Position(PosZ2,VelZ2,AccZ2);
  
  VelX1 = VelocityA(AccX1,VelX1);
  VelY1 = VelocityA(AccY1,VelY1);
  VelZ1 = VelocityA(AccZ1,VelZ1);
  VelX2 = VelocityA(AccX2,VelX2);
  VelY2 = VelocityA(AccY2,VelY2);
  VelZ2 = VelocityA(AccZ2,VelZ2);
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX1 = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccY1 = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ1 = (Wire.read() << 8 | Wire.read()) / 16384.0;

  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true);
  AccX2 = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccY2 = (Wire.read() << 8 | Wire.read()) / 16384.0;
  AccZ2 = (Wire.read() << 8 | Wire.read()) / 16384.0;

  VelX1 = VelocityB(AccX1,VelX1);
  VelY1 = VelocityB(AccY1,VelY1);
  VelZ1 = VelocityB(AccZ1,VelZ1);
  VelX2 = VelocityB(AccX2,VelX2);
  VelY2 = VelocityB(AccY2,VelY2);
  VelZ2 = VelocityB(AccZ2,VelZ2);

  //now we have the new x,y,z positions of the accelerometers and the velocities and the accelerations figured out

  
  
}
