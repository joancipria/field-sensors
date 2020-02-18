/* -------------------
----- FUNCTIONS ------
---------------------*/

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
  Serial.println(adc);

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