/*
           _____   _____    _____                               
     /\   |  __ \ / ____|  / ____|                              
    /  \  | |  | | (___   | (___   ___ _ __  ___  ___  _ __ ___ 
   / /\ \ | |  | |\___ \   \___ \ / _ \ '_ \/ __|/ _ \| '__/ __|
  / ____ \| |__| |____) |  ____) |  __/ | | \__ \ (_) | |  \__ \
 /_/    \_\_____/|_____/  |_____/ \___|_| |_|___/\___/|_|  |___/

 A library for managing multiple resistive sensors with ADS 1115
                                          
  ---
  Board: SparkFun ESP8266 Thing Dev (with With ADS 1115)
  Sensors: Humidity, Salinity, Temperature, Lighting, Pressure
  Code Version: v0.1
  ---

*/

/* -------------------
----- LIBRARIES ------
---------------------*/
#include <SFE_BMP180.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

// Construct an ads1115 at address 0x48
Adafruit_ADS1115 ads1115(0x48);
SFE_BMP180 pressure;

/* ---------------------
----- CALIBRATION ------
-----------------------*/

// Humidity
const int humidityReadPin = 0;
const int airValue = 19704;  // Air Value
const int waterValue = 9865; // Water Value

// Salinity
const int salinityReadPin = 1;
const int salinity_power_pin = 4; //  Digital I/O pin, Global variable
const float pureWater = 7100.35;
const float saltyWater = 19655.40;

// Lighting
const int lightingReadPin = 2;
const int lightValue = 27000; // Valor máximo de luz, menor resistencia
const int darkValue = 80;     // Valor mínimo de luz, mayor resistencia

// pressure
double altitude, P;
double base = 1052.25;

// Temperature
const int temperatureReadPin = 3;
const float b1[5] = {0.0307, 0.0350, 0.0360, 0.0340, 0.0330};
const float m[5] = {0.7965, 0.7750, 0.7600, 0.8100, 0.8450};
const float m1 = 676.087;
int i;

// Read Pin Values
int nave = 20;
float ave, stdev;
#define BUFFER_LENGTH 100 //  Size of array to store readings for computation of ave and stdev, reduce BUFFER_LENGTH to save memory if statistics are OK with smaller sample size

/* -------------------
----- FUNCTIONS ------
---------------------*/

// Cleans and returns a valid percentage (0 to 100 %)
int cleanPercentage(int percentage)
{
  if (percentage >= 100)
  {
    return 100;
  }
  if (percentage <= 0)
  {
    return 0;
  }

  return percentage;
}

// Read stats from adcPin with power_pin control
void readStats(int power_pin, int nave, float *ave, float *stdev, int adcPin)
{
  int i, n;
  float sum;                    //  Use a float to prevent overflow
  float reading[BUFFER_LENGTH]; //  Array to store readings
  n = min(nave, BUFFER_LENGTH); //  Make sure we don't over-run the data buffer

  // -- Store readings in an array
  for (i = 0; i < n; i++)
  {                                //  First array index is 0, last is n-1
    digitalWrite(power_pin, HIGH); //  Supply power to the sensor
    delay(100);                    //  Wait for sensor to settle
    int16_t adc = ads1115.readADC_SingleEnded(adcPin);
    reading[i] = adc;             //  Add reading to the running sum
    digitalWrite(power_pin, LOW); //  Turn off power to the sensor
    delay(10);                    //  wait between readings
  }
  // -- Compute average and standard deviation.
  for (sum = 0.0, i = 0; i < n; i++)
  {
    sum += reading[i];
  }
  *ave = sum / float(nave);
  for (sum = 0.0, i = 0; i < n; i++)
  {
    sum += pow(reading[i] - *ave, 2);
  }
  *stdev = sqrt(sum / float(n - 1));
}

// Read and calc Humidity
int calcHumidity(int adcPin)
{
  int16_t adc = ads1115.readADC_SingleEnded(adcPin);
  //Serial.println(adc);
  int humidity = cleanPercentage(100 * airValue / (airValue - waterValue) - adc * 100 / (airValue - waterValue));
  return humidity;
}

// Read and calc Salinity
int calcSalinity(int adcPin)
{
  readStats(salinity_power_pin, nave, &ave, &stdev, adcPin);
  //Serial.println(ave);
  int salinity = cleanPercentage(100 * pureWater / (pureWater - saltyWater) - ave * 100 / (pureWater - saltyWater));
  return salinity;
}

// Read and calc Lighting
float calcLighting(int adcPin)
{
  int16_t adc = ads1115.readADC_SingleEnded(adcPin);
  //Serial.println(adc);
  float lighting = cleanPercentage(100 * darkValue / (darkValue - lightValue) - adc * 100 / (darkValue - lightValue));
  return lighting;
}

double calcPressure()
{
  char status;
  double T, P, p0, a;

  // Lectura de temperatura para mejorar la lectura de presión

  // Iniciar medida de temperatura
  // En caso de iniciarse, se devuelve un 0

  status = pressure.startTemperature();
  if (status != 0)
  {
    // Esperamos a la medida

    delay(status);

    status = pressure.getTemperature(T);
    if (status != 0)
    {

      status = pressure.startPressure(14);
      //14 es el pin SDA donde se recoje la medida
      if (status != 0)
      {
        delay(status);

        status = pressure.getPressure(P, T);
        if (status != 0)
        {
          return (P);
        }
        else
          Serial.println("error retrieving pressure measurement\n");
      }
      else
        Serial.println("error starting pressure measurement\n");
    }
    else
      Serial.println("error retrieving temperature measurement\n");
  }
  else
    Serial.println("error starting temperature measurement\n");
}

// Read and calc Temperature
int calcTemperature(int adcPin)
{
  int16_t adc = ads1115.readADC_SingleEnded(adcPin);
  //Serial.println(adc);

  if (adc <= 8500)
  {
    i = 0;
  }
  if (adc > 8500 && adc <= 10117.45)
  {
    i = 1;
  }
  if (adc > 10117.45 && adc <= 12870.00)
  {
    i = 2;
  }
  if (adc > 12870.00 && adc <= 15785.00)
  {
    i = 3;
  }
  if (adc > 15785.00)
  {
    i = 4;
  }

  float temp = (adc - m[i] * b1[i]) / (m[i] * m1);

  return temp;
}

void initADS()
{
  ads1115.begin();                     // Initialize ads1115
  ads1115.setGain(GAIN_ONE);           // Setting Gain
  pinMode(salinity_power_pin, OUTPUT); // Config Salinity Power Pin

  // Init Pressure (Sensor BMP180)
  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Si fallara el sensor se indicaría

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while (1)
      ; // OFF.
  }

  // Obtenemos presión base

  double baseline = calcPressure();

  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");
}
