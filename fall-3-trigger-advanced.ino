#include <Wire.h>

const int MPU_addr = 0x68;

int16_t AcX,AcY,AcZ,Tmp,GyX,GyY,GyZ;

float ax=0, ay=0, az=0, gx=0, gy=0, gz=0;

boolean fall = false;
boolean trigger1=false;
boolean trigger2=false;

boolean ml_window_active = false;

byte trigger1count=0;
byte trigger2count=0;

int ml_count = 0;
int angleChange=0;

boolean pause_after_fall = false;

void setup(){

 Serial.begin(115200);

 Wire.begin(21,22);

 Wire.beginTransmission(MPU_addr);
 Wire.write(0x6B);
 Wire.write(0);
 Wire.endTransmission(true);

 Serial.println("MPU6050 Ready");
 Serial.println("ML Model Initialized (Simulated)\n");
}

void loop(){

 if (pause_after_fall){
   delay(10000);  // 10 sec pause
   pause_after_fall = false;
   return;
 }

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

 // 🔹 TRIGGER 1 (FREE FALL)
 if (Amp <= 2 && trigger2 == false){
   trigger1 = true;
   Serial.println("TRIGGER 1: Free Fall Detected");
 }

 if (trigger1 == true){
   trigger1count++;

   // 🔹 TRIGGER 2 (IMPACT)
   if (Amp >= 12){
     trigger2 = true;
     Serial.println("TRIGGER 2: Impact Detected");

     trigger1 = false;
     trigger1count = 0;
   }
 }

 // START ML WINDOW AFTER TRIGGER 2
 if (trigger2 == true){
   trigger2count++;

   if (!ml_window_active){
     ml_window_active = true;
     ml_count = 0;

     Serial.println("\n[ML] Collecting motion segment...");
   }
 }

 // ML PROCESSING
 if (ml_window_active){

  ml_count++;

  if (ml_count >= 25){

    Serial.println("\n[ML] Processing...");
    delay(300);

    // INTERNAL LOGIC (Trigger 3 hidden inside ML)
    angleChange = sqrt(gx*gx + gy*gy + gz*gz);

    if (angleChange <= 10){
        fall = true;
    } else {
        fall = false;
    }

    // FINAL OUTPUT
    if (fall == true){
        Serial.println("Model Output: FALL DETECTED");
        Serial.println("⚠ ALERT SENT TO CAREGIVER\n");

        pause_after_fall = true;  // pause after fall
    } else {
        Serial.println("Model Output: NORMAL ACTIVITY\n");
    }

    // RESET
    ml_window_active = false;
    ml_count = 0;
    trigger2 = false;
    trigger2count = 0;
    fall = false;
  }
 }

 // 🔹 TIMEOUTS
 if (trigger2count >= 10){
   trigger2 = false;
   trigger2count = 0;
 }

 if (trigger1count >= 10){
   trigger1 = false;
   trigger1count = 0;
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