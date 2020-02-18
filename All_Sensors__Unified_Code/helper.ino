// Cleans and returns a valid percentage (0 to 100 %)
int cleanPercentage (int percentage){
    if(percentage >= 100){
        return 100;
    }
    if(percentage <=0){
        return 0;
    }
    
    return percentage;
}

// Read stats from adcPin with power_pin control
void readStats( int power_pin, int nave, float *ave, float *stdev, int adcPin) {
    int    i, n;
    float  sum;                       //  Use a float to prevent overflow
    float reading[BUFFER_LENGTH];     //  Array to store readings
    n = min( nave, BUFFER_LENGTH );   //  Make sure we don't over-run the data buffer
    
    // -- Store readings in an array
    for ( i=0; i<n; i++ ) {                  //  First array index is 0, last is n-1
    digitalWrite( power_pin, HIGH );       //  Supply power to the sensor
    delay(100);                            //  Wait for sensor to settle
    int16_t adc = ads1115.readADC_SingleEnded(adcPin);
    reading[i] = adc;  //  Add reading to the running sum
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