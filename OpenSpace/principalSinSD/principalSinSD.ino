#include <SPI.h>
#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

SoftwareSerial serial_P = SoftwareSerial(2, 3); //RX entrada, TX salida

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char aLimpiar = ' ';
float datos[3] = {0.0, 0.0, 0.0};
float dato = 0.0;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  serial_P.begin(9600);
  Serial.println("Se inició la variable de serial por software.");

  while (!serial_P) {

  }
  delay(1000);
  Serial.println("Se inicio la comunicación serial: ");
  Serial.println(serial_P.read());

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  while (serial_P.available()) {
    aLimpiar = serial_P.read();
  }
  serial_P.flush();

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // If the RTC have lost power it will sets the RTC to the date & time this sketch was compiled in the following line
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  for (int i = 0; i < 3; i++)
    datos[i] = 0.0;

}


void loop() {
  serial_P.listen();
  dato = 0.0;
  if (serial_P.isListening()) {
    dato += serial_P.read() / 10;
    dato += serial_P.read() / 10;
    dato += serial_P.read() / 10;
    dato /= 3;
    if (( dato > 25 ) and ( dato < 65)) {
      Serial.print("Corresponde a un valor promedio del campo magnético: ");
      Serial.println(dato);
    }
    else {
      Serial.print("El valor obtenido está fuera del promedio: ");
      Serial.println(dato);
    }
    //    Serial.print("Dato promediado: ");
    //    Serial.println(dato);
    //    datos[0] = serial_P.read() / 10 ; //Guardamos en la variable dato el valor leido
    //    datos[1] = serial_P.read() / 10; //Guardamos en la variable dato el valor leido
    //    datos[2] = serial_P.read() / 10; //Guardamos en la variable dato el valor leido
    //    Serial.print("Dato");
    //    Serial.print(0);
    //    Serial.print(": ");
    //    Serial.print(datos[0], 4);
    //    Serial.print(" - ");
    //
    //    Serial.print("Dato");
    //    Serial.print(1);
    //    Serial.print(": ");
    //    Serial.print(datos[1], 4);
    //    Serial.print(" - ");
    //
    //    Serial.print("Dato");
    //    Serial.print(2);
    //    Serial.print(": ");
    //    Serial.print(datos[2], 4);
    //    Serial.println(" - ");


  }
  delay(1000);
}
