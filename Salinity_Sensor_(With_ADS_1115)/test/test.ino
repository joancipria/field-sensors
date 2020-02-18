/* TEST CODE FOR SALINITY SENSOR WITH ADS1115 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>

int salinity_power_pin = 4;   //  Digital I/O pin, Global variable
Adafruit_ADS1115 ads1115(0x48); // construct an ads1115 at address 0x49

#define  BUFFER_LENGTH  100   //  Size of array to store readings for computation of ave and stdev
                              //  Reduce BUFFER_LENGTH to save memory if statistics are OK
                              //  with smaller sample size
void setup()
{
  Serial.begin(9600);
  ads1115.begin(); // Initialize ads1115
  ads1115.setGain(GAIN_ONE); // Setting Gain
  pinMode(salinity_power_pin, OUTPUT);
}
void loop()
{
  int   nave = 20;
  float ave, stdev;
  salinity_reading_stats( salinity_power_pin, nave, &ave, &stdev);
  Serial.print(ave);  Serial.print(", ");   Serial.println(stdev);
  float pureWater = 8691.30;
  float saltyWater = 18551.90;
  
  float perSalinity = 100*pureWater/(pureWater-saltyWater)-ave*100/(pureWater-saltyWater);
  Serial.println(perSalinity);
}

void salinity_reading_stats( int power_pin, int nave, float *ave, float *stdev ) {
  int    i, n;
  float  sum;                       //  Use a float to prevent overflow
  float reading[BUFFER_LENGTH];     //  Array to store readings
  n = min( nave, BUFFER_LENGTH );   //  Make sure we don't over-run the data buffer
   int16_t adc0;
   
  // -- Store readings in an array
  for ( i=0; i<n; i++ ) {                  //  First array index is 0, last is n-1
    digitalWrite( power_pin, HIGH );       //  Supply power to the sensor
    delay(100);                            //  Wait for sensor to settle
    adc0 = ads1115.readADC_SingleEnded(0);
    reading[i] = adc0;  //  Add reading to the running sum
    digitalWrite( power_pin, LOW );        //  Turn off power to the sensor 
    delay(10);                             //  wait between readings
  }
  // -- Compute average and standard deviation.
  for ( sum=0.0, i=0; i<n; i++ )  {
    sum += reading[i];
  }
  *ave =  sum/float(nave);
  for ( sum=0.0, i=0; i<n; i++ ) {
    sum += pow(reading[i] - *ave, 2);
  }
  *stdev = sqrt( sum/float(n-1) );
}
