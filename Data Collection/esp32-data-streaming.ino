#include <Wire.h>

const int MPU_addr = 0x68;

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;

boolean trigger1=false;
byte trigger1count=0;

void setup(){

 Serial.begin(115200);

 Wire.begin(21,22);

 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);
 Wire.write(0);
 Wire.endTransmission(true);

 Serial.println("READY");
}

void loop(){

 mpu_read();

 ax = (AcX)/16384.00;
 ay = (AcY)/16384.00;
 az = (AcZ)/16384.00;

 gx = (GyX)/131.07;
 gy = (GyY)/131.07;
 gz = (GyZ)/131.07;

 float Amp = sqrt(ax*ax + ay*ay + az*az);
 float Angle = sqrt(gx*gx + gy*gy + gz*gz);

 // TRIGGER 1 (FREE FALL)
 if (Amp <= 2){
   trigger1 = true;
   Serial.println("START_RECORDING");  
 }

 // SEND DATA 
 Serial.print(ax); Serial.print(",");
 Serial.print(ay); Serial.print(",");
 Serial.print(az); Serial.print(",");
 Serial.print(gx); Serial.print(",");
 Serial.print(gy); Serial.print(",");
 Serial.print(gz); Serial.print(",");
 Serial.print(Amp); Serial.print(",");
 Serial.println(Angle);

 delay(100);
}

void mpu_read(){

 Wire.beginTransmission(MPU_addr);
 Wire.write(0x3B);
 Wire.endTransmission(false);

 Wire.requestFrom(MPU_addr,14,true);

 AcX=Wire.read()<<8|Wire.read();
 AcY=Wire.read()<<8|Wire.read();
 AcZ=Wire.read()<<8|Wire.read();
 Tmp=Wire.read()<<8|Wire.read();
 GyX=Wire.read()<<8|Wire.read();
 GyY=Wire.read()<<8|Wire.read();
 GyZ=Wire.read()<<8|Wire.read();
}
