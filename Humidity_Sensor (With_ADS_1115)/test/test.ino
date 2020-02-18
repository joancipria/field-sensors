/* TEST CODE FOR SKU:SEN0193 SENSOR WITH ADS1115 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>

/* Functions declaration */
int calcHumidity(int adc0);

Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x49
const int AirValue = 20243;  // Air Value
const int WaterValue = 10260;  // Water Value

void setup() { 
  Serial.begin(9600); // Open serial port and set the baud rate as 9600 bps
  ads1115.begin(); // Initialize ads1115
  ads1115.setGain(GAIN_ONE);   // Setting Gain
}

void loop() {
  
  int16_t adc0;
  adc0 = ads1115.readADC_SingleEnded(0);

  int humidity = calcHumidity(adc0);
  Serial.print(humidity);
  Serial.println("%");
  delay(5000);
}

// Calc humidity %
int calcHumidity(int adc0){
  int humidity = 100*AirValue/(AirValue-WaterValue)-adc0*100/(AirValue-WaterValue);
  return humidity;
}

