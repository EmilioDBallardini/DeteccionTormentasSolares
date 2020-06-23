
#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"
 
File logFile;
 
//RTC_DS1307 rtc;
RTC_DS3231 rtc;
 
void setup()
{
   Serial.begin(9600);
   Serial.print(F("Iniciando SD ..."));
   if (!SD.begin(10))
   {
      Serial.println(F("Error al iniciar"));
      return;
   }
   Serial.println(F("Iniciado correctamente"));
   
   logFile = SD.open("archivo.txt");//abrimos  el archivo 
   if (logFile) {
     Serial.println("archivo.txt:");
     while (logFile.available()) {
      Serial.write(logFile.read());
     }
      logFile.close(); //cerramos el archivo
   } else {
      Serial.println("Error al abrir el archivo");
  }
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
   logFile.print(" ");
   logFile.print(date.hour(), DEC);
   logFile.print(':');
   logFile.print(date.minute(), DEC);
   logFile.print(':');
   logFile.print(date.second(), DEC);
   logFile.print(" ");
   logFile.println(value);
}
 
 
void loop()
{
   // Abrir archivo y escribir valor
   logFile = SD.open("archivo.txt");
 
   if (logFile) {
      //int value = readSensor();
      DateTime now = rtc.now();
 
      logValue(now, 0);
      logFile.close();
 
   }
   else {
      Serial.println(F("Error al abrir el archivo"));
   }
   delay(10000);
}
