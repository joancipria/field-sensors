/*
   _____                           _    _____          _      
  / ____|                         | |  / ____|        | |     
 | |  __  ___ _ __   ___ _ __ __ _| | | |     ___   __| | ___ 
 | | |_ |/ _ \ '_ \ / _ \ '__/ _` | | | |    / _ \ / _` |/ _ \
 | |__| |  __/ | | |  __/ | | (_| | | | |___| (_) | (_| |  __/
  \_____|\___|_| |_|\___|_|  \__,_|_|  \_____\___/ \__,_|\___|
 
  ---
  Board: SparkFun ESP8266 Thing Dev (with With ADS 1115)
  Sensors: Humidity, Salinity, Temperature, Lighting
  Code Version: v0.2
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

/* -------------------------------
----- FUNCTIONS DECLARATION ------
---------------------------------*/
int calcHumidity(int adcPin);
int calcSalinity(int adcPin);
void readStats(int power_pin, int nave, float *ave, float *stdev, int adcPin);

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
double base = 1045.9;

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

/* ---------------
----- SETUP ------
-----------------*/
void setup()
{
  Serial.begin(9600);
  ads1115.begin();           // Initialize ads1115
  ads1115.setGain(GAIN_ONE); // Setting Gain
  pinMode(salinity_power_pin, OUTPUT);

  // Iniciar el sensor

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

/* --------------
----- LOOP ------
----------------*/
void loop()
{

  // Get Humidity
  int perHumidity = calcHumidity(humidityReadPin);

  // Get Salinity
  float perSalinity = calcSalinity(salinityReadPin);

  //Get Lighting
  float perLighting = calcLighting(lightingReadPin);

  // Get Pressure
  P = calcPressure();

  // Get Altitude
  altitude = pressure.altitude(P, base);

  // Get Temperature
  int degreesTemperature = calcTemperature(temperatureReadPin);

  // Print results
  Serial.print("Humidity: ");
  Serial.print(perHumidity);
  Serial.println("%");

  Serial.print("Salinity: ");
  Serial.print(perSalinity);
  Serial.println("%");

  Serial.print("Lighting: ");
  Serial.print(perLighting);
  Serial.println("%");

  Serial.print("Pressure: ");
  Serial.println(P);

  Serial.print("Relative Altitude: ");
  if (altitude >= 0.0)
    Serial.print(" "); // espacio para números positivos
  Serial.print(altitude, 1);
  Serial.print(" meters, ");
  if (altitude >= 0.0)
    Serial.print(" "); // espacio para números negativos
  Serial.print(altitude * 3.28084, 0);
  Serial.println(" feet");

  Serial.print("Temperature: ");
  Serial.print(degreesTemperature);
  Serial.println("ºC");

  Serial.println("--------------");
  delay(3000);
}
