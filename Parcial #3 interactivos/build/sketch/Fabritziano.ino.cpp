#include <Arduino.h>
#line 1 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"

#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;

#define OUTPUT_READABLE_QUATERNION

#define INTERRUPT_PIN 2 
#define LED_PIN 13      
bool blinkState = false;


bool dmpReady = false; 
uint8_t mpuIntStatus;  
uint8_t devStatus;    
uint16_t packetSize;    
uint16_t fifoCount;     
uint8_t fifoBuffer[64]; 


Quaternion q;        
VectorInt16 aa;      
VectorInt16 aaReal;  
VectorInt16 aaWorld; 
VectorFloat gravity; 
float euler[3];      
float ypr[3];        


uint8_t teapotPacket[14] = {'$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n'};

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false; 
#line 43 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"
void dmpDataReady();
#line 52 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"
void setup();
#line 109 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"
void loop();
#line 43 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"
void dmpDataReady()
{
  mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup()
{

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
  Wire.setClock(400000); 
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif

 
  Serial.begin(115200);
  while (!Serial)
    ; 

  mpu.initialize();
  pinMode(INTERRUPT_PIN, INPUT);

  

  devStatus = mpu.dmpInitialize();

  
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); 

 
  if (devStatus == 0)
  {
    
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
   

    mpu.setDMPEnabled(true);

 

    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), dmpDataReady, RISING);
    mpuIntStatus = mpu.getIntStatus();



    dmpReady = true;

   
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop()
{

  if (!dmpReady)
    return;

  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
  { 
#ifdef OUTPUT_READABLE_QUATERNION
   
    mpu.dmpGetQuaternion(&q, fifoBuffer);
  
    Serial.print(q.w);
    Serial.print(",");
    Serial.print(q.x);
    Serial.print(",");
    Serial.print(q.y);
    Serial.print(",");
    Serial.println(q.z);
#endif


    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);

    // for unity
    delay(100);
  }
}
