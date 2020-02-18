#include <SFE_BMP180.h>
#include <Wire.h>

// Crear un objeto llamado Presión (pressure)

SFE_BMP180 pressure;

void setup()
{
  Serial.begin(9600);
  Serial.println("REBOOT");

  // Iniciar el sensor

  if (pressure.begin())
    Serial.println("BMP180 init success");
  else
  {
    // Si fallara el sensor se indicaría

    Serial.println("BMP180 init fail (disconnected?)\n\n");
    while(1); // OFF.
  }

  // Obtenemos presión base
  
  double baseline = getPressure();
  
  Serial.print("baseline pressure: ");
  Serial.print(baseline);
  Serial.println(" mb");  
}

void loop()
{
  double a,P;
  double base = 1045.9;
  
  // Nueva lectura de presión

  P = getPressure();

  //Mostrar la diferencia de altitud relativa 
  //respecto la primera y segunda lectura (presión)
  
  a = pressure.altitude(P,base);
  
  Serial.print("Presión: ");
  Serial.println(P);
  Serial.print("Altitud relativa: ");
  if (a >= 0.0) Serial.print(" "); // espacio para números positivos
  Serial.print(a,1);
  Serial.print(" metros, ");
  if (a >= 0.0) Serial.print(" "); // espacio para números negativos
  Serial.print(a*3.28084,0);
  Serial.println(" pies");
  
  delay(1000);
}


double getPressure()
{
  char status;
  double T,P,p0,a;

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

        status = pressure.getPressure(P,T);
        if (status != 0)
        {
          return(P);
        }
        else Serial.println("error retrieving pressure measurement\n");
      }
      else Serial.println("error starting pressure measurement\n");
    }
    else Serial.println("error retrieving temperature measurement\n");
  }
  else Serial.println("error starting temperature measurement\n");
}
