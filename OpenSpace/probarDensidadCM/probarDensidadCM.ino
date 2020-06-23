  const int pinHall = A0;
  float valor_Azul=0;
  float valor_Rojo=0;
  
void setup() {
  pinMode(pinHall, INPUT);
  Serial.begin(9600);
}
 
void loop() {
  long s_recib = 0;
  for(int i = 0; i < 10; i++){
     s_recib += analogRead(pinHall);
  }
  s_recib /= 10;
  Serial.print("mV = ");
  Serial.println(s_recib);
  if( s_recib < 526)
  {
    long valor_Rojo = s_recib + 526;
  }else
  {
    long valor_Azul = s_recib - 526;    
  }
  Serial.print("LED azul= ");
  Serial.println(valor_Azul);
  Serial.print("LED rojo = ");
  Serial.println(valor_Rojo);

  
  //long measure = 0;
  //for(int i = 0; i < 10; i++){
      //int value = 
     //measure += analogRead(pinHall);
  //}
  //measure /= 10;
  
  //calculo del voltaje en mV
  //float outputV = measure * 5000.0 / 1023;
  //Serial.print("Output Voltaje = ");
  //Serial.print(outputV);
  //Serial.println(" mV   ");
  
  //interpolacion a densidad de flujo magnético
  //float magneticFlux =  outputV * 53.33 - 133.3;
  //float flujo = magneticFlux / 1000;
  //Serial.print("Magnetic Flux Density = ");
  //Serial.print(flujo);
  //Serial.println(" T");
  
  delay(500);
}
