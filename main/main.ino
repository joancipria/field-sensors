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
#include "ADS.h"
#include "GPS.h"
#include "MPU.h"
#include "sendData.h"




/* ---------------
----- SETUP ------
-----------------*/
void setup()
{
  Serial.begin(9600);
  initADS();
  initGPS();
  initMPU();
  initSendData();
}

/* --------------
----- LOOP ------
----------------*/
void loop()
{
  //ACELEROMETRO//
  uint8_t Buf[14]; //lectura
  float x = (Buf[0] << 8 | Buf[1]);
  float y = (Buf[2] << 8 | Buf[3]);
  float z = Buf[4] << 8 | Buf[5];
  lecturaI2C(MPU9250_ADDRESS, 0x3B, 14, Buf);
  acelerometro(&x, &y, &z);

  
  int data [8];
  
  // Get Humidity
  int perHumidity = calcHumidity(humidityReadPin);
  data[1] = perHumidity;

  // Get Salinity
  float perSalinity = calcSalinity(salinityReadPin);
  data[2] = perSalinity;

  //Get Lighting
  float perLighting = calcLighting(lightingReadPin);
  data[3] = perLighting;


  // Get Pressure
  P = calcPressure();
  data[4] = P;

  // Get Altitude
  altitude = pressure.altitude(P, base);
  data[5] = altitude;

  // Get Temperature
  int degreesTemperature = calcTemperature(temperatureReadPin);
  data[6] = degreesTemperature;

  // Get Location 
  data[7] = getLocation();

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

  Serial.print("GPS: ");
  Serial.print(data[7]);
  Serial.println("");

  Serial.println("--------------");send(data);
  delay(3000);
}
