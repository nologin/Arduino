
/*
Library Repository : https://github.com/ckalpha/MQ135
Author : Damrongwit Nusuk
Email : jack@racksync.com
Website : http://www.racksync.com
*/

#include "MQ135.h"
#define ANALOGPIN A0    //  Define Analog PIN on Arduino Board
#define RZERO 1515.69    //  Define RZERO Calibration Value
MQ135 gasSensor = MQ135(ANALOGPIN);

void setup()
{
  Serial.begin(115200);
  pinMode(13,OUTPUT);
  pinMode(12, INPUT);
  float rzero = gasSensor.getRZero();
  delay(3000);
  Serial.print("MQ135 RZERO Calibration Value : ");
  Serial.println(rzero);
  digitalWrite(13,LOW);
}

void loop() {

  float ppm = gasSensor.getPPM();
  int stat = digitalRead(13);
  delay(1000);
  Serial.print("CO2 ppm value : ");
  Serial.println(ppm);
  Serial.println(stat);
  if (ppm > 0.30) {
    digitalWrite(13,HIGH);
  } else {
      digitalWrite(13,LOW);
  }
}
