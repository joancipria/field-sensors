/* CALIBRATION CODE FOR SKU:SEN0193 SENSOR WITH ADS1115 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>

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
  Serial.println(adc0);
  delay(5000);
}
