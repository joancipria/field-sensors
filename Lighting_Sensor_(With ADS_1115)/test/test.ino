#include <Wire.h>
#include <Adafruit_ADS1015.h>

/* Functions declaration */
float calcLighting(int adc0);

Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x49

void setup() { 
  Serial.begin(9600); // Open serial port and set the baud rate as 9600 bps
  ads1115.begin(); // Initialize ads1115
  ads1115.setGain(GAIN_ONE);   // Setting Gain
}

void loop() {
  
  int16_t adc0;
  adc0 = ads1115.readADC_SingleEnded(0);
  //Serial.println(adc0);
  int light = calcLighting(adc0);
  Serial.print(light);
  Serial.println("%");
  delay(5000);
}

// Calc humidity %
float calcLighting(int adc0){
float iluminacion;
  const int luz =27000 ;  // Valor máximo de luz, menor resistencia
  const int oscuro = 80;  // Valor mínimo de luz, mayor resistencia
  iluminacion = 100*oscuro/(oscuro-luz)-adc0*100/(oscuro-luz); //cálculo
  return iluminacion;

}
