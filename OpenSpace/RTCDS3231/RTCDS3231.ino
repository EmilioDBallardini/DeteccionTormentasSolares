#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>

File logFile;

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup()
{
  Serial.begin(9600);

  pinMode(4, OUTPUT);
  //digitalWrite(4, HIGH);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // If the RTC have lost power it will sets the RTC to the date & time this sketch was compiled in the following line
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

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


// Funcion que simula la lectura de un sensor
int readSensor()
{
  return 0;
}

void logValue(DateTime date, int value)
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
  logFile.print(". Valor: ");
  logFile.print(value);
  logFile.print(". Temperature: ");
  logFile.print(rtc.getTemperature());
  logFile.print(" °C");
}

void serialDate(DateTime date)
{
  Serial.print(date.year(), DEC);
  Serial.print('/');
  Serial.print(date.month(), DEC);
  Serial.print('/');
  Serial.print(date.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[date.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(date.hour(), DEC);
  Serial.print(':');
  Serial.print(date.minute(), DEC);
  Serial.print(':');
  Serial.print(date.second(), DEC);
  Serial.print(" ");
  Serial.print("Temperature: ");
  Serial.print(rtc.getTemperature());
  Serial.println(" °C");
}

void loop()
{
  // Abrir archivo y escribir valor
  logFile = SD.open("datos.txt", FILE_WRITE);

  if (logFile) {
    //int value = readSensor();
    DateTime now = rtc.now();
    serialDate(now);
    logValue(now, 0);
    logFile.close();
  }
  else {
    Serial.println(F("Error al abrir el archivo"));
  }

  delay(1000);
}
