#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

SoftwareSerial serial_P = SoftwareSerial(2, 3); //RX entrada, TX salida

File logFile;

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char aLimpiar = ' ';
float datos[3] = {0.0, 0.0, 0.0};

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  serial_P.begin(9600);
  Serial.println("Se inició la variable de serial por software.");
  pinMode(4, OUTPUT);

  while (!serial_P) {

  }
  Serial.println(serial_P.read());
  Serial.println("Se inicio la comunicación serial");



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

  Serial.print(F("Iniciando SD ..."));
  if (!SD.begin(4))
  {
    Serial.println(F("Error al iniciar"));
    return;
  }
  Serial.println(F("Iniciado correctamente"));

  logFile = SD.open("datos.txt", FILE_WRITE);//abrimos  el archivo
  if (logFile) {
    Serial.print("Se abrió exitosamente..");
  } else {
    Serial.println("Error al abrir el archivo");
  }
  logFile.close(); //cerramos el archivo
}


void logValue(DateTime date, float value, boolean baja)
{
  logFile.print(date.year(), DEC);
  logFile.print('/');
  logFile.print(date.month(), DEC);
  logFile.print('/');
  logFile.print(date.day(), DEC);
  logFile.print(" (");
  logFile.print(daysOfTheWeek[date.dayOfTheWeek()]);
  logFile.print(") ");
  logFile.print(date.hour(), DEC);
  logFile.print(':');
  logFile.print(date.minute(), DEC);
  logFile.print(':');
  logFile.print(date.second(), DEC);
  logFile.print(" ");
  logFile.print("Temperatura: ");
  logFile.print(rtc.getTemperature());
  logFile.print(" °C");
  logFile.print(" ");
  if (!baja) {
    logFile.print("Valor: ");
    logFile.print(" [");
    logFile.print(value, 4);
    logFile.print("] ");
  }
  else {
    logFile.print(value, 4);
    logFile.println(" > ");
  }
}


void loop() {
  serial_P.listen();
  if (serial_P.isListening()) {
    logFile = SD.open("datos.txt", FILE_WRITE);
    if (logFile) {
      Serial.println("Se abrió el archivo");
      datos[0] = serial_P.read() / 10 ; //Guardamos en la variable dato el valor leido
      datos[1] = serial_P.read() / 10; //Guardamos en la variable dato el valor leido
      datos[2] = serial_P.read() / 10; //Guardamos en la variable dato el valor leido
      logValue(rtc.now(), datos[0], false);
      logValue(rtc.now(), datos[1], false);
      logValue(rtc.now(), datos[2], true);
      Serial.print("Dato");
      Serial.print(0);
      Serial.print(": ");
      Serial.print(datos[0], 4);
      Serial.print(" - ");

      Serial.print("Dato");
      Serial.print(1);
      Serial.print(": ");
      Serial.print(datos[1], 4);
      Serial.print(" - ");

      Serial.print("Dato");
      Serial.print(2);
      Serial.print(": ");
      Serial.print(datos[2], 4);
      Serial.println(" - ");

      Serial.println("Se cerró el archivo");
    }
    else {
      Serial.println(F("Error al abrir el archivo"));
    }
    logFile.close();
  }
  delay(1000);
}
