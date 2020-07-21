const int arrayPins[6] = {A0, A1, A2, A3, A5, A6}; //[0]:pin 2 [1]:pin 3 [2]:pin 4 [3]:pin 5 [4]:pin 6 [5]:pin 7
// const int arrayPins[6] = {A2, A3, A4, A5, A6, A7}; //[0]:pin 2 [1]:pin 3 [2]:pin 4 [3]:pin 5 [4]:pin 6 [5]:pin 7
//const int pinHall7 = A7;
//const int pinHall6 = A6;
//const int pinHall5 = A5;
//const int pinHall4 = A4;
//const int pinHall3 = A3;
//const int pinHall2 = A2;

long arrayMedidas[6]; //[0]:medida 2 [1]:medida 3 [2]:medida 4 [3]:medida 5 [4]:medida 6 [5]:medida 7
//float measure7 = 0;
//float measure6 = 0;
//float measure5 = 0;
//float measure4 = 0;
//float measure3 = 0;
//float measure2 = 0;

long arrayRestasMedidas[3]; //[0]:medida 32 [1]:medida 54 [2]:medida 76
//float measure76 = 0;
//float measure54 = 0;
//float measure32 = 0;

float salidas[3]; //[0]:salida 32 [1]:salida 54 [2]:salida 76
//float output76;
//float output54;
//float output32;

float arrayFlujos[3]; //[0]:flujo 32 [1]:flujo 54 [2]:flujo 76
//float flujo76;
//float flujo54;
//float flujo32;

float magneticFlux = 0.0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 6; i++) {
    pinMode(arrayPins[i], INPUT);
    arrayMedidas[i] = 0;
  }
  for (int i = 0; i < 3; i++) {
    arrayRestasMedidas[i] = 0;
    salidas[i] = 0.0;
    arrayFlujos[i] = 0.0;
  }
  //  pinMode(pinHall7, INPUT);
  //  pinMode(pinHall6, INPUT);
  //  pinMode(pinHall5, INPUT);
  //  pinMode(pinHall4, INPUT);
  //  pinMode(pinHall3, INPUT);
  //  pinMode(pinHall2, INPUT);

}

void loop() {

  //media de 10 medidas para filtrar ruido
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 6; j++)
      arrayMedidas[j] += analogRead(arrayPins[j]);
    //    measure7 += analogRead(pinHall7);
    //    measure7 += analogRead(pinHall6);
    //    measure7 += analogRead(pinHall5);
    //    measure7 += analogRead(pinHall4);
    //    measure7 += analogRead(pinHall3);
    //    measure7 += analogRead(pinHall2);
  }
  for (int j = 0; j < 6; j++)
    arrayMedidas[j] /= 10;
  //  measure7 /= 10;
  //  measure6 /= 10;
  //  measure5 /= 10;
  //  measure4 /= 10;
  //  measure3 /= 10;
  //  measure2 /= 10;

  //  for (int i = 1; i < 7; i += 2) {
  //    arrayRestasMedidas[i] = (arrayMedidas[i] + arrayMedidas[i - 1]) / 2;
  //  }
  arrayRestasMedidas[0] = (arrayMedidas[1] + arrayMedidas[0]) / 2;
  arrayRestasMedidas[1] = (arrayMedidas[3] + arrayMedidas[2]) / 2;
  arrayRestasMedidas[2] = (arrayMedidas[5] + arrayMedidas[4]) / 2;
  //  measure76 = measure7 - measure6;
  //  measure54 = measure5 - measure4;
  //  measure32 = measure3 - measure2;

  //calculo del voltaje en mV
  for (int i = 0; i < 6; i++)
    salidas[i] = (arrayRestasMedidas[i] * 5000.0) / 1023;
  // output76 = measure76 * 5000.0 / 1023;
  // output54 = measure76 * 5000.0 / 1023;
  // output32 = measure76 * 5000.0 / 1023;

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
  Serial.print(" T   ");

  //interpolacion a densidad de flujo magnético
  arrayFlujos[1] = flujoMagnetico(magneticFlux, salidas[1]);
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[1]);
  Serial.print(" T   ");

  //interpolacion a densidad de flujo magnético
  arrayFlujos[0] = flujoMagnetico(magneticFlux, salidas[0]);
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[0]);
  Serial.println(" T   ");

  delay(750);
}


float flujoMagnetico(float magneticFlux, float out) {
  magneticFlux = (out * 53.33) - 133.3;
  magneticFlux /= 1000;
  return magneticFlux;
}
