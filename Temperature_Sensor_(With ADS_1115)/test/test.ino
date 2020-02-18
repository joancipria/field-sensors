/* -------------------
----- LIBRARIES ------
---------------------*/
#include <Wire.h>
#include <Adafruit_ADS1015.h>

// Construct an ads1115 at address 0x48
Adafruit_ADS1115 ads1115(0x48);


/* -------------------------------
----- FUNCTIONS DECLARATION ------
---------------------------------*/



/* ---------------------
----- CALIBRATION ------
-----------------------*/

// Temperature
const int temperatureReadPin = 3;
const float b1[5] = {0.0307,0.0350,0.0360,0.0340,0.0330};
const float m[5] = {0.7965,0.7750,0.7600,0.8100,0.8450};
const float m1 = 676.087;
int i;

                             
/* ---------------
----- SETUP ------
-----------------*/
void setup(){
  Serial.begin(9600);
  ads1115.begin(); // Initialize ads1115
  ads1115.setGain(GAIN_ONE); // Setting Gain
}

/* --------------
----- LOOP ------
----------------*/
void loop(){

    // Get Temperature
    int degreesTemperature = calcTemperature(temperatureReadPin);
    
    // Print results
    Serial.print("Temperature: ");
    Serial.print(degreesTemperature);
    Serial.println("ºC");

  


  Serial.println("--------------");    
  delay(3000);
}

/* -------------------
----- FUNCTIONS ------
---------------------*/
// Read and calc Temperature
int calcTemperature (int adcPin){
  int16_t adc = ads1115.readADC_SingleEnded(adcPin);
  Serial.println(adc);

  if (adc <=8500){
    i = 0;
  }
  if (adc >8500 && adc <= 10117.45){
    i = 1;
  }
  if (adc >10117.45 && adc <= 12870.00){
    i = 2;
  }
  if (adc >12870.00 && adc <= 15785.00){
    i = 3;
  }
    if (adc >15785.00){
    i = 4;
  }
  
  float temp = (adc-m[i]*b1[i])/(m[i]*m1);

  return temp;
}