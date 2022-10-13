# 1 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"

# 3 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino" 2

# 5 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino" 2


# 8 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino" 2


MPU6050 mpu;





bool blinkState = false;

// MPU control/status vars
bool dmpReady = false; // set true if DMP init was successful
uint8_t mpuIntStatus; // holds actual interrupt status byte from MPU
uint8_t devStatus; // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize; // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount; // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q; // [w, x, y, z]         quaternion container
VectorInt16 aa; // [x, y, z]            accel sensor measurements
VectorInt16 aaReal; // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld; // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity; // [x, y, z]            gravity vector
float euler[3]; // [psi, theta, phi]    Euler angle container
float ypr[3]; // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector

// packet structure for InvenSense teapot demo
uint8_t teapotPacket[14] = {'$', 0x02, 0, 0, 0, 0, 0, 0, 0, 0, 0x00, 0x00, '\r', '\n'};

// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false; // indicates whether MPU interrupt pin has gone high
void dmpDataReady()
{
  mpuInterrupt = true;
}

// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup()
{
// join I2C bus (I2Cdev library doesn't do this automatically)

  Wire.begin();
  Wire.setClock(400000); // 400kHz I2C clock. Comment this line if having compilation difficulties




  // initialize serial communication
  // (115200 chosen because it is required for Teapot Demo output, but it's
  // really up to you depending on your project)
  Serial.begin(115200);
  while (!Serial)
    ; // wait for Leonardo enumeration, others continue immediately

  // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3V or Arduino
  // Pro Mini running at 3.3V, cannot handle this baud rate reliably due to
  // the baud timing being too misaligned with processor ticks. You must use
  // 38400 or slower in these cases, or use some kind of external separate
  // crystal solution for the UART timer.

  // initialize device

  mpu.initialize();
  pinMode(2 /* use pin 2 on Arduino Uno & most boards*/, 0x01);

  // verify connection

  //    // wait for ready
  //    Serial.println(F("\nSend any character to begin DMP programming and demo: "));
  //    while (Serial.available() && Serial.read()); // empty buffer
  //    while (!Serial.available());                 // wait for data
  //    while (Serial.available() && Serial.read()); // empty buffer again

  // load and configure the DMP

  devStatus = mpu.dmpInitialize();

  // supply your own gyro offsets here, scaled for min sensitivity
  mpu.setXGyroOffset(220);
  mpu.setYGyroOffset(76);
  mpu.setZGyroOffset(-85);
  mpu.setZAccelOffset(1788); // 1688 factory default for my test chip

  // make sure it worked (returns 0 if so)
  if (devStatus == 0)
  {
    // Calibration Time: generate offsets and calibrate our MPU6050
    mpu.CalibrateAccel(6);
    mpu.CalibrateGyro(6);
    mpu.PrintActiveOffsets();
    // turn on the DMP, now that it's ready

    mpu.setDMPEnabled(true);

    // enable Arduino interrupt detection

    attachInterrupt((((2 /* use pin 2 on Arduino Uno & most boards*/)<40)?(2 /* use pin 2 on Arduino Uno & most boards*/):-1), dmpDataReady, 0x01);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it

    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
  }
}

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop()
{
  // if programming failed, don't try to do anything
  if (!dmpReady)
    return;
  // read a packet from FIFO
  if (mpu.dmpGetCurrentFIFOPacket(fifoBuffer))
  { // Get the Latest packet

    // display quaternion values in easy matrix form: w x y z
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    //  Serial.print("quat\t");
    Serial.print(q.w);
    Serial.print(",");
    Serial.print(q.x);
    Serial.print(",");
    Serial.print(q.y);
    Serial.print(",");
    Serial.println(q.z);
# 217 "/home/pop-os/Documents/sf1-unidad3-ev-2022-20-frabritzianos/Parcial #3 interactivos/Fabritziano.ino"
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(13 /* (Arduino is 13, Teensy is 11, Teensy++ is 6)*/, blinkState);

    // for unity
    delay(100);
  }
}
