#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

//Como la conexión UART del Arduino va a ser usada para comunicarse con el host, se utiliza una comunicación serial virtual
//Para esto es necesario establecer una conexión física entre los Arduinos, yo elegí los Pins 2(Rx) y 3(Tx)
//La numeración y la asignación con "pinMode(x, X)" tiene que coincidir en ambos
//Solo hay que intercambiar la conexión física entre cables, es decir, el cable que sale del Pin 2(Rx) del arduino principal, tiene que ir al Pin 3(Tx) del secundario.
//En éste modelo se usa un módulo SD externo, en caso de que se use una placa que incluya lectora de SD como la de ethernet, hay que cambiar el código.

SoftwareSerial serial_P = SoftwareSerial(2, 3); //RX entrada, TX salida

File logFile; //Asignamos una variable de tipo archivo.

RTC_DS3231 rtc; //La variable del reloj, el mismo puede ir variando ej. "RTC_DS1703.."

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char aLimpiar = ' '; //Se usa para limpiar el Buffer de entrada del maestro cuando no hay intercambio de datos requeridos
float datos[3] = {0.0, 0.0, 0.0};
String cadena;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT);
  pinMode(3, OUTPUT);
  serial_P.begin(9600);
  Serial.println("Se inició la variable de serial por software.");
  pinMode(4, OUTPUT);  //Se establece el Pin 4 para la salida de datos hacia la SD. Ésto de nuevo es elección personal, pero se suele usar éste o el Pin 10

  while (!serial_P) {
    //Mientras no se haya iniciado la conexión serial virtual, no se hace nada
  }
  Serial.println(serial_P.read());
  Serial.println("Se inicio la comunicación serial");

  if (! rtc.begin()) {
    Serial.println("No se encontró ningún RTC");
    while (1);
  }

  while (serial_P.available()) {
    aLimpiar = serial_P.read();
  }
  serial_P.flush();

  if (rtc.lostPower()) {
    Serial.println("El RTC perdió energía, se reestablecerá la fecha");
    // Esto es para actualizar el RTC a la hora actual, cuando el HOST reestablece la conexión.
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // También es posible fijar la fecha por alguna a elección con el código siguiente
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  
  // Inicializamos el arreglo de que va a recibir los datos en 0.0
  for (int i = 0; i < 3; i++)
    datos[i] = 0.0;

  Serial.print(F("Iniciando SD ..."));
  if (!SD.begin(4))
  {
    Serial.println(F("Error al iniciar"));
    return;
  }
  Serial.println(F("Iniciado correctamente"));
  
  // Acá pueden cambiar el nombre del archivo a guardar y el tipo.
  // En la SD va a aparecer como "DATOS", o el nombre que ustedes elijan pero en mayúsculas y sin la extensión
  logFile = SD.open("datos.txt", FILE_WRITE);//abrimos  el archivo
  if (logFile) {
    Serial.print("Se abrió exitosamente..");
  } else {
    Serial.println("Error al abrir el archivo");
  }
  logFile.close(); //cerramos el archivo
}

// Módulo para guardar la fecha, hora y datos en el archivo.
// La lógica de la variable "Baja" es para establecer cuando hay que bajar de línea.
// Esto fue pensado para que se reciban 3 datos desde el secundario y se guarden en el archivo en 3 colúmnas con sus respectivas fechas y valores.
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
  serial_P.listen(); // La conexión principal se pone en modo escucha
  // Si fue exitosa entonces comienza a operar
  if (serial_P.isListening()) {
    logFile = SD.open("datos.txt", FILE_WRITE); // Leemos el archivo en modo lectura y escritura
    if (logFile) {
      logFile.println("Se abrió el archivo");
      datos[0] = serial_P.read() / 10 ; //Guardamos en la variable dato el valor leido
      datos[1] = serial_P.read() / 10; //Guardamos en la variable dato el valor leido
      datos[2] = serial_P.read() / 10; //Guardamos en la variable dato el valor leido
      logValue(rtc.now(), datos[0], false);
      logValue(rtc.now(), datos[1], false);
      logValue(rtc.now(), datos[2], true);
    }
    else {
      break;
    }
    logFile.close(); // Cerramos el archivo.
  }
  if ( serial.isAvailable() > 0 ){
    logFile = SD.open("datos.txt", FILE_WRITE);
    if ( logFile ) {
      while ! ( eof(logFile) ) 
        Serial.print(logFile.read());
      logFile.close();
    }
  }
  delay(300);
}
