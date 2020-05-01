#include "Arduino.h"
#include "LED.h"

#define LEDB_PIN_VIN 3
#define LEDC_PIN_VIN 4

const int pinHall = A0;

LED ledB(LEDB_PIN_VIN);
LED ledC(LEDC_PIN_VIN);

void setup() {
  pinMode(pinHall, INPUT);
  Serial.begin(9600);
}
 
void loop() {
 
  //media de 10 medidas para filtrar ruido
  long measure = 0;
  for(int i = 0; i < 10; i++){
      int value = 
      measure += analogRead(pinHall);
  }
  measure /= 10;

  //calculo del voltaje en mV
  float outputV = measure * 5000.0 / 1023;
  Serial.print("Output Voltaje = ");
  Serial.print(outputV);
  Serial.println(" mV   ");
  
  if( measure > 863)
  {
    ledB.dim(255);
  }else
  {
    ledB.dim( measure - 526 );
  }
//  if( measure < 526)
//  {
//    if( measure <= 337 )
//    {
//      ledC.dim(255);
//    }else
//    {
//      long modulo = measure - 526;
//      abs(modulo);
//      ledC.dim(modulo);
//    }
//  }
 
  delay(250);
}
