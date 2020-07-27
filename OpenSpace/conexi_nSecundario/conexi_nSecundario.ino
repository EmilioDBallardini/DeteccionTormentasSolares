#include <SoftwareSerial.h>

// Éste código va hacia el Arduino que recibe los datos de los N sensores (en éste primer caso 6, pero puede escalarse a muchos mas)
// Realice algunos cálculos que están pre-fijados para los sensores SSE49, pero puede cambiarse por cualquiera que sea lineal y se tenga disponible el cálculo necesario 
// para extrapolar los datos recibidos de forma analógica, a miliVolts.
// Como en el Arduino principal, el Pin 2 es Rx, y el Pin 3 es Tx
// Rx es por donde se reciben los datos, y Tx es por donde se envían.
// Deberían ver parpadear la luz "Tx" del Arduino cada vez que se envía algo.

SoftwareSerial serial_S = SoftwareSerial(2, 3); //RX entrada, TX salida

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
  
  // Se inician los arreglos de datos y se asignan los Pins analógicos de manera de Entrada
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

// Decidí comentar las salidas seriales que se comunican con la Pc o el host. Pero se usan para verificar que todo esté funcionando correctamente.

void loop()
{
  // Medimos 10 veces de cada sensor y luego hacemos una media entre dos que se encuentren enfrentados entre cara y espalda.
  // Esto lo pueden variar entre varios, o tomar los datos RAW de cada uno y enviarlo.
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
  /*
  Serial.print("Output Voltaje 7-6= ");
  Serial.print(salidas[2]);
  Serial.print(" mV     ");

  Serial.print("Output Voltaje 5-4= ");
  Serial.print(salidas[1]);
  Serial.print(" mV     ");

  Serial.print("Output Voltaje 3-2= ");
  Serial.print(salidas[0]);
  Serial.println(" mV     ");
  */
  
  //interpolacion a densidad de flujo magnético
  arrayFlujos[2] = flujoMagnetico(magneticFlux, salidas[2]);
  /*
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[2]);
  Serial.print(" mT   ");
  */
  //interpolacion a densidad de flujo magnético
  arrayFlujos[1] = flujoMagnetico(magneticFlux, salidas[1]);
  /*
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[1]);
  Serial.print(" mT   ");
  */
  //interpolacion a densidad de flujo magnético
  arrayFlujos[0] = flujoMagnetico(magneticFlux, salidas[0]);
  /* 
  Serial.print("Magnetic Flux Density = ");
  Serial.print(arrayFlujos[0]);
  Serial.println(" mT   ");
  */
  
  for (int i = 0; i < 3; i++) {
    if (serial_S.isListening()) {
      float enviar = arrayFlujos[i] * 10; 
      // Se multiplica x10 y en el principal se divide por el mismo número para aprovechar los decimales
      // Esto es porque la comunicación serial virtual trunca los decimales que enviemos
      serial_S.write(enviar); //Envía el dato procesado al master o principal
    }
  }
  delay(300);
}

// Recibe un flujo magnético, una salida ya procesada y devuelve ese flujo en un valor estandarizado
float flujoMagnetico(float magneticFlux, float out) {
  magneticFlux = (out * 53.33) - 133.3;
  magneticFlux /= 1000;
  return magneticFlux;
}
