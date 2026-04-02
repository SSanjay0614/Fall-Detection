#include <Wire.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"


const int MPU_addr = 0x68;

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

float ax=0, ay=0, az=0;

void setup(){
 
 Serial.begin(115200);

 Wire.begin(21,22);

 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);
 Wire.write(0);
 Wire.endTransmission(true);

 Serial.println("MPU6050 Ready");

 WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

}

void loop(){

 mpu_read();

 ax = (AcX)/16384.00;
 ay = (AcY)/16384.00;
 az = (AcZ)/16384.00;

 float Raw_Amp = sqrt(ax*ax + ay*ay + az*az);

 int Amp = Raw_Amp * 10;

 Serial.print("Acceleration magnitude: ");
 Serial.println(Amp);

 if (Amp >= 18){

   Serial.println("⚠ FALL DETECTED");


   delay(2000);
 }

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
