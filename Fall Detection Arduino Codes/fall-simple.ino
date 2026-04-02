#include <Wire.h>
#include <MPU6050.h>
#include <math.h>

MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  Wire.begin(21,22);

  Serial.println("MPU6050 Initialized");

  mpu.initialize();
}

void loop() {

  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  float accMagnitude = sqrt(ax*ax + ay*ay + az*az);

  Serial.print("Acceleration magnitude: ");
  Serial.println(accMagnitude);

  if(accMagnitude > 30000){
    Serial.println("⚠ FALL DETECTED!");
  }

  delay(300);
}