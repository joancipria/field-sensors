const byte pindelainterrupcion = 4;

#define    MPU9250_ADDRESS           0x68
#define    MAG_ADDRESS               0x0C

#define    ACC_FULL_SCALE_2_G        0x00  
#define    ACC_FULL_SCALE_4_G        0x08
#define    ACC_FULL_SCALE_8_G        0x10
#define    ACC_FULL_SCALE_16_G       0x18

#define    ACCEL_CONFIG2             0x0F
#define    LP_ACCEL_ODR              0x03
#define    WOM_THR                   0x0F
#define    INT_ENABLE                0x40
#define    MOT_DETECT_CTRL           0xC0
#define    PWR_MGMT_1                0x00
#define    PWR_MGMT_2                0x07
#define    PWR_MGMT_1_1              0X20
 
//Funcion auxiliar lectura
void lecturaI2C(uint8_t direccion, uint8_t registro, uint8_t bytes, uint8_t* Data)
{
   Wire.beginTransmission(direccion);
   Wire.write(registro);
   Wire.endTransmission();
 
   Wire.requestFrom(direccion, bytes);
   uint8_t index = 0;
   while (Wire.available())
      Data[index++] = Wire.read();
}

 
// Funcion auxiliar de escritura
void escrituraI2C(uint8_t direccion, uint8_t registro, uint8_t Data)
{
   Wire.beginTransmission(direccion);
   Wire.write(registro);
   Wire.write(Data);
   Wire.endTransmission();
}
void acelerometro(float *ax, float *ay, float *az){
   int FS_ACC = 16;
   // Convertir registros
   (*ax) = (*ax)*FS_ACC/32768-0.04 ;
   (*ay) = (*ay)*FS_ACC/32768-0.01 ;
   (*az) = (*az)*FS_ACC/32768-0.02 ;
  
  }
//funcion determinada
void handleInterrupt() {
  Serial.println("¡ALERTA!¡INTENTO DE ROBO!");
}


void initMPU(){
  // Configurar acelerometro
  escrituraI2C(MPU9250_ADDRESS, 28, ACC_FULL_SCALE_16_G);
  //Configuracion para el wake_on_motion
  escrituraI2C(MPU9250_ADDRESS, 0x6B, PWR_MGMT_1);
  escrituraI2C(MPU9250_ADDRESS, 0x6C, PWR_MGMT_2);
  escrituraI2C(MPU9250_ADDRESS, 0x1D, ACCEL_CONFIG2);
  escrituraI2C(MPU9250_ADDRESS, 0x38, INT_ENABLE);
  escrituraI2C(MPU9250_ADDRESS, 0x69, MOT_DETECT_CTRL);
  escrituraI2C(MPU9250_ADDRESS, 0x1F, WOM_THR);
  escrituraI2C(MPU9250_ADDRESS, 0x1E, LP_ACCEL_ODR);
  escrituraI2C(MPU9250_ADDRESS, 0x6B, PWR_MGMT_1_1);
  pinMode(pindelainterrupcion, INPUT_PULLUP);
  attachInterrupt(pindelainterrupcion, handleInterrupt, FALLING);
}
