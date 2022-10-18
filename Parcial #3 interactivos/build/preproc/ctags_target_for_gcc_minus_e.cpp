# 1 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"

# 3 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino" 2

# 5 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino" 2


# 8 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino" 2


MPU6050 mpu;





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
void dmpDataReady()
{
  mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup()
{


  Wire.begin();
  Wire.setClock(400000);





  Serial.begin(115200);
  while (!Serial)
    ;

  mpu.initialize();
  pinMode(2, 0x01);



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



    attachInterrupt((((2)<40)?(2):-1), dmpDataReady, 0x01);
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


    mpu.dmpGetQuaternion(&q, fifoBuffer);

    Serial.print(q.w);
    Serial.print(",");
    Serial.print(q.x);
    Serial.print(",");
    Serial.print(q.y);
    Serial.print(",");
    Serial.println(q.z);



    blinkState = !blinkState;
    digitalWrite(13, blinkState);

    // for unity
    delay(100);
  }
}
