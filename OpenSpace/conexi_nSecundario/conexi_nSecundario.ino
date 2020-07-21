#include <SoftwareSerial.h>

SoftwareSerial serial_S = SoftwareSerial(2, 3); //RX entrada, TX salida

//const int arrayPins[6] = {A2, A3, A4, A5, A6, A7}; //[0]:pin 2 [1]:pin 3 [2]:pin 4 [3]:pin 5 [4]:pin 6 [5]:pin 7
const int arrayPins[6] = {A0, A1, A2, A3, A5, A6}; //[0]:pin 2 [1]:pin 3 [2]:pin 4 [3]:pin 5 [4]:pin 6 [5]:pin 7
long arrayMedidas[6] = {0, 0, 0, 0, 0, 0}; //[0]:medida 2 [1]:medida 3 [2]:medida 4 [3]:medida 5 [4]:medida 6 [5]:medida 7
long arrayRestasMedidas[3] = {0, 0, 0}; //[0]:medida 32 [1]:medida 54 [2]:medida 76
float salidas[3] = {0.0, 0.0, 0.0}; //[0]:salida 32 [1]:salida 54 [2]:salida 76
float arrayFlujos[3] = {0.0, 0.0, 0.0}; //[0]:flujo 32 [1]:flujo 54 [2]:flujo 76
float magneticFlux = 0.0;

void setup() {
  Serial.begin(9600);
  serial_S.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);

  if (serial_S.isListening()) {
    serial_S.write("V");
  }
  else {
    serial_S.write("N");
  }


  for (int i = 0; i < 6; i++) {
    pinMode(arrayPins[i], INPUT);
    arrayMedidas[i] = 0;
  }
  for (int i = 0; i < 3; i++) {
    arrayRestasMedidas[i] = 0;
    salidas[i] = 0.0;
    arrayFlujos[i] = 0.0;
  }
}

//MODIFICAR

void loop()
{
  //media de 10 medidas para filtrar ruido
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 6; j++)
      arrayMedidas[j] += analogRead(arrayPins[j]);
  }
  for (int j = 0; j < 6; j++)
    arrayMedidas[j] /= 10;

  arrayRestasMedidas[0] = (arrayMedidas[1] + arrayMedidas[0]) / 2;
  arrayRestasMedidas[1] = (arrayMedidas[3] + arrayMedidas[2]) / 2;
  arrayRestasMedidas[2] = (arrayMedidas[5] + arrayMedidas[4]) / 2;

  for (int i = 0; i < 6; i++)
    salidas[i] = (arrayRestasMedidas[i] * 5000.0) / 1023;

  Serial.print("Output Voltaje 7-6= ");
  Serial.print(salidas[2]);
  Serial.print(" mV     ");

  Serial.print("Output Voltaje 5-4= ");
  Serial.print(salidas[1]);
  Serial.print(" mV     ");

  Serial.print("Output Voltaje 3-2= ");
  Serial.print(salidas[0]);
  Serial.println(" mV     ");

  //interpolacion a densidad de flujo magnético
  arrayFlujos[2] = flujoMagnetico(magneticFlux, salidas[2]);
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[2]);
  Serial.print(" mT   ");

  //interpolacion a densidad de flujo magnético
  arrayFlujos[1] = flujoMagnetico(magneticFlux, salidas[1]);
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[1]);
  Serial.print(" mT   ");

  //interpolacion a densidad de flujo magnético
  arrayFlujos[0] = flujoMagnetico(magneticFlux, salidas[0]);
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[0]);
  Serial.println(" mT   ");

  for (int i = 0; i < 3; i++) {
    if (serial_S.isListening()) {
      float enviar = arrayFlujos[i] * 10;
      serial_S.write(enviar); //Envía el dato procesado al master o principal
      //delay(500);
    }
  }
  delay(1000);
}

float flujoMagnetico(float magneticFlux, float out) {
  magneticFlux = (out * 53.33) - 133.3;
  magneticFlux /= 1000;
  return magneticFlux;
}
