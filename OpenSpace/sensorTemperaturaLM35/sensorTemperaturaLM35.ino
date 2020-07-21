int i;
int val;
int val2;
int tempPin=0;
int tempPin2=1;
float promedio=0.0;
float mV;
float cel;
float farH;
float kelvin;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  for(i=0 ; i<10 ; i++){
    val=analogRead(tempPin);
    val2=analogRead(tempPin2);
    promedio = (val + val2)/2;
    mV = ( promedio / 1024.0) * 5000;
    cel = mV / 10;
    farH = (cel * 9)/5 + 32;
    kelvin = cel + 273.15;  
  }
  
  Serial.print("Temperatura en Celcius: ");
  Serial.print(cel);
  Serial.print("°C");
  Serial.print("       ");

  Serial.print("Temperatura en Farenheit: ");
  Serial.print(farH);
  Serial.print("°F");
  Serial.print("       ");
  
  Serial.print("Temperatura en Kelvin: ");
  Serial.print(kelvin);
  Serial.print("K");
  Serial.println();
  delay(1000);

  
}
