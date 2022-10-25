#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 mpu;

#define OUTPUT_READABLE_QUATERNION

bool dmpReady = false;
uint8_t mpuIntStatus;
uint8_t devStatus;
uint16_t packetSize;
uint16_t fifoCount;
uint8_t fifoBuffer[64];

Quaternion q;


// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;
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
  mpu.initialize();

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
    uint8_t arr[16] = {0};
    mempcpy(arr, (uint8_t *)&q.w, 4);
    mempcpy(arr + 4, (uint8_t *)&q.x, 4);
    mempcpy(arr + 8, (uint8_t *)&q.y, 4);
    mempcpy(arr + 12, (uint8_t *)&q.z, 4);

    if (Serial.available() > 0)
    {
      String respuesta = Serial.readStringUntil('\n');

      if (respuesta == "s")
      {
        Serial.write(arr, 16);
      }
    }
   
#endif

   
  }
}