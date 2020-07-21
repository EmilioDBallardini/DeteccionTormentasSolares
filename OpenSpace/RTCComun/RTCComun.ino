#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include "RTClib.h"

File archivo_datos;
DateTime now;
//RTC_DS1307 rtc;
RTC_DS3231 rtc;

String daysOfTheWeek[7] = { "Domingo", "Lunes", "Martes", "Miercoles", "Jueves", "Viernes", "Sabado" };
String monthsNames[12] = { "Enero", "Febrero", "Marzo", "Abril", "Mayo",  "Junio", "Julio", "Agosto", "Septiembre", "Octubre", "Noviembre", "Diciembre" };



int segundo, minuto, hora, dia, mes;
long anio; //variable año
DateTime HoraFecha;

void setup () {
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  if (!rtc.begin()) { //Verificamos si se inicializa
    Serial.println(F("No se encontró reloj alguno"));

  }
  // Si se ha perdido la corriente, fijar fecha y hora
  if (rtc.lostPower()) {
    // Fijar a fecha y hora de compilacion
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // Fijar a fecha y hora específica. En el ejemplo, 21 de Enero de 2016 a las 03:00:00
    // rtc.adjust(DateTime(2016, 1, 21, 3, 0, 0));
  }
  Serial.print(F("Iniciando SD ..."));
  if (!SD.begin(4))
  {
    Serial.println(F("Error al iniciar"));
    return;
  }
  SD.mkdir("BBG/Datos");
  Serial.println(F("Iniciado correctamente"));
  if (!SD.exists("BBG/Datos/archivo_datos.txt")) {
    archivo_datos = SD.open("BBG/Datos/archivo_datos.txt");//abrimos  el archivo
    archivo_datos.close();
  }
}

void printDate(DateTime date)
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
  Serial.println();
}

void saveDate(DateTime date, float dato_normal)
{
  archivo_datos.print(date.year());
  archivo_datos.print('/');
  archivo_datos.print(date.month());
  archivo_datos.print('/');
  archivo_datos.print(date.day());
  archivo_datos.print(") ");
  archivo_datos.print(date.hour());
  archivo_datos.print(':');
  archivo_datos.print(date.minute());
  archivo_datos.print(':');
  archivo_datos.print(date.second());
  archivo_datos.print("");
}

//Reemplazar con el dato recibido del otro arduino
float readSensor() {
  return 1.0123;
}

void loop () {

  // Obtener fecha actual y mostrar por Serial

  //delay(3000);

  archivo_datos = SD.open("BBG/Datos/archivo_datos.txt", FILE_WRITE);

  if (archivo_datos) {
    float value = readSensor();
    now = rtc.now();
    printDate(now);
    saveDate(now, value);
    archivo_datos.close();
  }
  else {
    Serial.println(F("Error al abrir el archivo"));
  }

  archivo_datos.close();
  delay(1000);
}
