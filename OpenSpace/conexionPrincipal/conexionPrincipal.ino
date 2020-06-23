#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include "Sodaq_DS3231.h"


File logFile;
//AT24C32 EepromRTC;
float dato;

void setup(){ 
  Serial.begin(9600);
  if(logFile){
    logFile.println("Lectura correcta del archivo");
    logFile.print(F("Iniciando SD ..."));
    if (!SD.begin(4))
    {
      logFile.println(F("Error al iniciar"));
      return;
      
    }
    logFile.println(F("Iniciado correctamente"));  
  }
  
}


void logValue(DateTime date, int value)
{
   logFile.print(date.year(), DEC);
   logFile.print('/');
   logFile.print(date.month(), DEC);
   logFile.print('/');
   logFile.print(date.dayOfWeek(), DEC);
   logFile.print(" ");
   logFile.print(date.hour(), DEC);
   logFile.print(':');
   logFile.print(date.minute(), DEC);
   logFile.print(':');
   logFile.print(date.second(), DEC);
   logFile.print(" ");
   logFile.println(value);
}
 
//MODIFICAR 

void loop(){ 
  
 // EepromRTC.writeFloat(8,variable_float); // posiscion i: float ocupa 4 bytes de tamaño 
  dato= Serial.read();//Guardamos en la variable dato el valor leido

  /**
   * Abre el archivo y escribe el dato
   */
  logFile = SD.open("datalog.txt", FILE_WRITE);
  if (logFile) {
   DateTime now = rtc.now();
    logValue(now, dato);
   
   logFile.close();
   }
  else {
   Serial.println(F("Error al abrir el archivo"));
  }

  //Preparar una función anterior para que chequee que el valor sea correcto y no algún valor basura
  
  delay(50); 
}
