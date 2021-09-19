#include <Wire.h>

const int MPU = 0x68;


// I have iproved the precision of the angles and have been able to keep them relatively stable

// 7:49 I know the exact, stable values, of the angles and the acceleration in the 3 axis








// Acceleration and gyroscopic data
float AccX, AccY, AccZ;
float GyrX, GyrY, GyrZ;
float AccPosX, AccPosY;
float GyrPosX, GyrPosY;
float PosX, PosY, PosZ;
float AccErrorX, AccErrorY;
float GyroErrorX, GyroErrorY, GyroErrorZ;
float AccStableX, AccStableY, AccStableZ;
float VelX, VelY, VelZ;
float PoslinearX, PoslinearY, PoslinearZ;
float sx, sy, sz;
float result=0;
float sum=0;
int n=0;
float distance;
bool up= false;

// Gravity!
float g = 1.0;

// Complementary filter calculation
const float sampling_frequency = 100;
const float time_delay = 100;
const float alpha = (time_delay/(time_delay + (1/sampling_frequency)));

// For integrating gyroscope data
float start_time, delta_time, current_time;

int c = 0;
float Delta = 0.05;

float Velocity(float a, float v){
  float vel;
  vel= v+ a*delta_time;
  return vel;
}

float Position(float x, float v, float a){
  float pos;
  pos = x + v*delta_time + a*delta_time*delta_time/2;
  return pos;
}

void setup () {
  Serial.begin(9600);
  Wire.begin();

  // Begin transmission
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0x00);
  Wire.endTransmission(true);


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

  AccX -= 0.084;
  AccZ += 1;
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

  GyrPosX += GyrX*delta_time+7.25;
  GyrPosY += GyrY*delta_time-1.57;

  // Use a complementary filter to combine the data
  PosZ += GyrZ*delta_time +0.85;
  
  PoslinearX = Position(PoslinearX, VelX, AccX);
  PoslinearY = Position(PoslinearY, VelY, AccY);
  PoslinearZ = Position(PoslinearX, VelZ, AccZ);

  VelX = Velocity(AccX, VelX);
  VelY = Velocity(AccY, VelY);
  VelZ = Velocity(AccZ, VelZ);

  


  

  //Now we can output the x,y,z values of the accelerometer (theoretically)

      

 
 Serial.println(AccX);
 if (AccZ < 1.6){
      up = true;
    }

}
