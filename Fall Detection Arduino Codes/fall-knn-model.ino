#include <Wire.h>
#include <string.h>

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

float amp_buffer[25];
float angle_buffer[25];


// MODEL 
void score(double * input, double * output) {
    double var0[2];

    if (input[1] <= 3.6754653) {
        if (input[6] > 2.312645) {
            if (input[0] > 11.492050) {
                if (input[7] > 0.543256) {
                    if (input[4] > 63.56845) {
                        if (input[8] <= 17.153267) {
                            memcpy(var0, (double[]){0.0, 1.0}, 2 * sizeof(double));
                        } else {
                            memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                        }
                    } else {
                        memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                    }
                } else {
                    memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
                }
            } else {
                memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
            }
        } else {
            memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
        }
    } else {
        memcpy(var0, (double[]){1.0, 0.0}, 2 * sizeof(double));
    }

    memcpy(output, var0, 2 * sizeof(double));
}

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
   delay(10000);
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

 // TRIGGER 1
 if (Amp <= 3 && trigger2 == false){
   trigger1 = true;
   Serial.println("TRIGGER 1: Free Fall Detected");
   if (!ml_window_active){
     ml_window_active = true;
     ml_count = 0;

     Serial.println("\n[ML] Collecting motion segment...");
   }
 }

 if (trigger1 == true){
   trigger1count++;

   if (Amp >= 12){
     trigger2 = true;
     Serial.println("TRIGGER 2: Impact Detected");

     trigger1 = false;
     trigger1count = 0;
   }
 }

 if (trigger2 == true){
   trigger2count++;
 }

 if (ml_window_active){

  float angle = sqrt(gx*gx + gy*gy + gz*gz);

  amp_buffer[ml_count] = Amp;
  angle_buffer[ml_count] = angle;

  ml_count++;

  if (ml_count >= 30){

    Serial.println("\n[ML] Processing...");
    delay(300);

    // FEATURE EXTRACTION
    float max_amp = amp_buffer[0];
    float min_amp = amp_buffer[0];
    float sum_amp = 0;

    float max_angle = angle_buffer[0];
    float sum_angle = 0;

    int freefall_duration = 0;
    int impact_duration = 0;

    for (int i = 0; i < 30; i++){
      float a = amp_buffer[i];
      float ang = angle_buffer[i];

      if (a > max_amp) max_amp = a;
      if (a < min_amp) min_amp = a;

      if (ang > max_angle) max_angle = ang;

      sum_amp += a;
      sum_angle += ang;

      if (a <= 2) freefall_duration++;
      if (a >= 12) impact_duration++;
    }

    float mean_amp = sum_amp / 25;
    float std_amp = 0;

    for (int i = 0; i < 25; i++){
      std_amp += (amp_buffer[i] - mean_amp) * (amp_buffer[i] - mean_amp);
    }
    std_amp = sqrt(std_amp / 25);

    float mean_angle = sum_angle / 25;
    float final_angle = angle_buffer[24];

    // PREPARE INPUT
    double input[9] = {
      max_amp, min_amp, mean_amp, std_amp,
      max_angle, mean_angle,
      (double)freefall_duration,
      (double)impact_duration,
      final_angle
    };

    double output[2];

    // MODEL CALL
    score(input, output);

    // FINAL OUTPUT 
    if (output[1] > output[0]){
        Serial.println("Model Output: FALL DETECTED");
        Serial.println("⚠ ALERT SENT TO CAREGIVER\n");

        pause_after_fall = true;
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

 // TIMEOUTS
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
