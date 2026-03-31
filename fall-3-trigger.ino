#include <Wire.h>

const int MPU_addr = 0x68;

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;

boolean fall = false;
boolean trigger1=false;
boolean trigger2=false;
boolean trigger3=false;

byte trigger1count=0;
byte trigger2count=0;
byte trigger3count=0;

int angleChange=0;

void setup(){

 Serial.begin(115200);

 Wire.begin(21,22);   // ESP32 I2C pins

 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);
 Wire.write(0);
 Wire.endTransmission(true);

 Serial.println("MPU6050 Ready");
}

void loop(){

 mpu_read();

 ax = (AcX)/16384.00;
 ay = (AcY)/16384.00;
 az = (AcZ)/16384.00;

 gx = (GyX)/131.07;
 gy = (GyY)/131.07;
 gz = (GyZ)/131.07;

 float Raw_Amp = sqrt(ax*ax + ay*ay + az*az);
 int Amp = Raw_Amp * 10;

 Serial.print("Amp: ");
 Serial.println(Amp);

 if (Amp<=2 && trigger2==false){
   trigger1=true;
   Serial.println("TRIGGER 1 ACTIVATED");
 }

 if (trigger1==true){
   trigger1count++;

   if (Amp>=12){
     trigger2=true;
     Serial.println("TRIGGER 2 ACTIVATED");

     trigger1=false;
     trigger1count=0;
   }
 }

 if (trigger2==true){
   trigger2count++;

   angleChange = sqrt(gx*gx + gy*gy + gz*gz);

   Serial.print("Angle Change: ");
   Serial.println(angleChange);

   if (angleChange>=30 && angleChange<=400){

     trigger3=true;
     trigger2=false;
     trigger2count=0;

     Serial.println("TRIGGER 3 ACTIVATED");
   }
 }

 if (trigger3==true){

    trigger3count++;

    if (trigger3count>=10){

       angleChange = sqrt(gx*gx + gy*gy + gz*gz);

       Serial.println(angleChange);

       if (angleChange>=0 && angleChange<=10){

           fall=true;
           trigger3=false;
           trigger3count=0;

       }
       else{

          trigger3=false;
          trigger3count=0;
          Serial.println("TRIGGER 3 DEACTIVATED");

       }
     }
  }

 if (fall==true){

   Serial.println("⚠ FALL DETECTED");

   fall=false;

 }

 if (trigger2count>=6){

   trigger2=false;
   trigger2count=0;

   Serial.println("TRIGGER 2 DEACTIVATED");

 }

 if (trigger1count>=6){

   trigger1=false;
   trigger1count=0;

   Serial.println("TRIGGER 1 DEACTIVATED");

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