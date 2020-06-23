const int pinHall = A0;

void setup(){ 
  pinMode(pinHall, INPUT);
  Serial.begin(9600);
}

//MODIFICAR

void loop()
{ 
  //media de 10 medidas para filtrar ruido
  long measure = 0;
  for(int i = 0; i < 10; i++){
      int value = 
      measure += analogRead(pinHall);
  }
  measure /= 10;
  
  //calculo del voltaje en mV
  float outputV = measure * 5000.0 / 1023;
  Serial.print("Output Voltaje = ");
  Serial.print(outputV);
  Serial.println(" mV   ");
  
  //interpolacion a densidad de flujo magnético
  float magneticFlux =  outputV * 53.33 - 133.3;
  float flujo = magneticFlux / 1000;
  Serial.print("Magnetic Flux Density = ");
  Serial.print(flujo);
  Serial.println(" T");
  Serial.write(flujo); //Envía el dato procesado al master o principal
  
  delay(75);
}
