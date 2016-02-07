// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added seamless Fastwire support
//                 - added note about gyro calibration
//      2012-06-21 - added note about Arduino 1.0.1 + Leonardo compatibility error
//      2012-06-20 - improved FIFO overflow handling and simplified read process
//      2012-06-19 - completely rearranged DMP initialization code and simplification
//      2012-06-13 - pull gyro and accel data from FIFO packet instead of reading directly
//      2012-06-09 - fix broken FIFO read sequence and change interrupt detection to RISING
//      2012-06-05 - add gravity-compensated initial reference frame acceleration output
//                 - add 3D math helper file to DMP6 example sketch
//                 - add Euler output and Yaw/Pitch/Roll output formats
//      2012-06-04 - remove accel offset clearing for better results (thanks Sungon Lee)
//      2012-06-01 - fixed gyro sensitivity to be 2000 deg/sec instead of 250
//      2012-05-30 - basic DMP initialization working

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2012 Jeff Rowberg

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
===============================================
*/

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"

#include "MPU6050_6Axis_MotionApps20.h"
//#include "MPU6050.h" // not necessary if using MotionApps include file

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    #include "Wire.h"
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu1;
MPU6050 mpu2;

/* =========================================================================
   NOTE: In addition to connection 3.3v, GND, SDA, and SCL, this sketch
   depends on the MPU-6050's INT pin being connected to the Arduino's
   external interrupt #0 pin. On the Arduino Uno and Mega 2560, this is
   digital I/O pin 2.
 * ========================================================================= */

/* =========================================================================
   NOTE: Arduino v1.0.1 with the Leonardo board generates a compile error
   when using Serial.write(buf, len). The Teapot output uses this method.
   The solution requires a modification to the Arduino USBAPI.h file, which
   is fortunately simple, but annoying. This will be fixed in the next IDE
   release. For more info, see these links:

   http://arduino.cc/forum/index.php/topic,109987.0.html
   http://code.google.com/p/arduino/issues/detail?id=958
 * ========================================================================= */



// uncomment "OUTPUT_QT_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) formatted for the Glove Motion Qt application
#define OUTPUT_QT_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_YAWPITCHROLL" if you want to see the yaw/
// pitch/roll angles (in degrees) calculated from the quaternions coming
// from the FIFO. Note this also requires gravity vector calculations.
// Also note that yaw/pitch/roll angles suffer from gimbal lock (for
// more info, see: http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_YAWPITCHROLL

// uncomment "OUTPUT_READABLE_QUATERNION" if you want to see the actual
// quaternion components in a [w, x, y, z] format (not best for parsing
// on a remote host such as Processing or something though)
//#define OUTPUT_READABLE_QUATERNION

// uncomment "OUTPUT_READABLE_EULER" if you want to see Euler angles
// (in degrees) calculated from the quaternions coming from the FIFO.
// Note that Euler angles suffer from gimbal lock (for more info, see
// http://en.wikipedia.org/wiki/Gimbal_lock)
//#define OUTPUT_READABLE_EULER

// uncomment "OUTPUT_READABLE_REALACCEL" if you want to see acceleration
// components with gravity removed. This acceleration reference frame is
// not compensated for orientation, so +X is always +X according to the
// sensor, just without the effects of gravity. If you want acceleration
// compensated for orientation, us OUTPUT_READABLE_WORLDACCEL instead.
//#define OUTPUT_READABLE_REALACCEL

// uncomment "OUTPUT_READABLE_WORLDACCEL" if you want to see acceleration
// components with gravity removed and adjusted for the world frame of
// reference (yaw is relative to initial orientation, since no magnetometer
// is present in this case). Could be quite handy in some cases.
//#define OUTPUT_READABLE_WORLDACCEL

// uncomment "OUTPUT_TEAPOT" if you want output that matches the
// format used for the InvenSense teapot demo
//#define OUTPUT_TEAPOT



#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful

uint8_t mpuIntStatus1;   // holds actual interrupt status byte from MPU
uint8_t mpuIntStatus2;

uint8_t devStatus1;      // return status after each device operation (0 = success, !0 = error)
uint8_t devStatus2;

uint16_t packetSize1;    // expected DMP packet size (default is 42 bytes)
uint16_t packetSize2;

uint16_t fifoCount1;     // count of all bytes currently in FIFO
uint16_t fifoCount2;

uint8_t fifoBuffer1[64]; // FIFO storage buffer
uint8_t fifoBuffer2[64];

// orientation/motion vars
Quaternion q1;           // [w, x, y, z]         quaternion container
Quaternion q2;

VectorInt16 aa1;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal1;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld1;    // [x, y, z]            world-frame accel sensor measurements
VectorInt16 aa2;
VectorInt16 aaReal2;
VectorInt16 aaWorld2; 

VectorFloat gravity1;    // [x, y, z]            gravity vector
VectorFloat gravity2;

float euler1[3];         // [psi, theta, phi]    Euler angle container
float euler2[3];

float ypr1[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector
float ypr2[3];

// packet structure for InvenSense teapot demo
uint8_t teapotPacket1[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };
uint8_t teapotPacket2[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };



// ================================================================
// ===           MPU6050 SELECTION VIA DEMULTIPLEXER            ===
// ================================================================

void setDemux (int i)
{
  if (i == 1)
  {
      digitalWrite(10, LOW);   
      digitalWrite(11, HIGH);  
      digitalWrite(12, LOW);   
      delay(2);  
  }
  
  if (i == 2)
  {
      digitalWrite(10, LOW);   
      digitalWrite(11, LOW);  
      digitalWrite(12, HIGH);   
      delay(2);  
  }
}



// ================================================================
// ===               INTERRUPT DETECTION ROUTINE                ===
// ================================================================

volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high
void dmpDataReady() {
    mpuInterrupt = true;
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
  
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
  
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (115200 chosen because it is required for Teapot Demo output, but it's
    // really up to you depending on your project)
    Serial.begin(115200);
    while (!Serial); // wait for Leonardo enumeration, others continue immediately

    // NOTE: 8MHz or slower host processors, like the Teensy @ 3.3v or Ardunio
    // Pro Mini running at 3.3v, cannot handle this baud rate reliably due to
    // the baud timing being too misaligned with processor ticks. You must use
    // 38400 or slower in these cases, or use some kind of external separate
    // crystal solution for the UART timer.

    // initialize device
    #ifndef OUTPUT_QT_YAWPITCHROLL
    Serial.println(F("Initializing I2C devices..."));
    #endif
    setDemux(1);
    mpu1.initialize();
    setDemux(2);
    mpu2.initialize();

    // verify connection
    #ifndef OUTPUT_QT_YAWPITCHROLL
    Serial.println(F("Testing device connections..."));
    setDemux(1);
    Serial.println(mpu1.testConnection() ? F("MPU6050 1 connection successful") : F("MPU6050 1 connection failed"));
    setDemux(2);
    Serial.println(mpu2.testConnection() ? F("MPU6050 2 connection successful") : F("MPU6050 2 connection failed"));
    #endif
    
    // wait for ready
    while (Serial.available() && Serial.read()); // empty buffer

    // load and configure the DMP
    #ifndef OUTPUT_QT_YAWPITCHROLL
    Serial.println(F("Initializing DMP..."));
    #endif
    setDemux(1);
    devStatus1 = mpu1.dmpInitialize();
    setDemux(2);
    devStatus2 = mpu2.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity
    setDemux(1);
    mpu1.setXGyroOffset(220);
    mpu1.setYGyroOffset(76);
    mpu1.setZGyroOffset(-85);
    mpu1.setZAccelOffset(1788); // 1688 factory default for my test chip
    
    setDemux(2);
    mpu2.setXGyroOffset(220);
    mpu2.setYGyroOffset(76);
    mpu2.setZGyroOffset(-85);
    mpu2.setZAccelOffset(1788);

    // make sure it worked (returns 0 if so)
    if (devStatus1 == 0 || devStatus2 == 0) {
        // turn on the DMP, now that it's ready
        #ifndef OUTPUT_QT_YAWPITCHROLL
        Serial.println(F("Enabling DMP..."));
        #endif
        setDemux(1);
        mpu1.setDMPEnabled(true);
        setDemux(2);
        mpu2.setDMPEnabled(true);

        // enable Arduino interrupt detection
        #ifndef OUTPUT_QT_YAWPITCHROLL
        Serial.println(F("Enabling interrupt detection (Arduino external interrupt 0)..."));
        #endif
        attachInterrupt(0, dmpDataReady, RISING);
        setDemux(1);
        mpuIntStatus1 = mpu1.getIntStatus();
        setDemux(2);
        mpuIntStatus2 = mpu2.getIntStatus();

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        #ifndef OUTPUT_QT_YAWPITCHROLL
        Serial.println(F("DMP ready! Waiting for first interrupt..."));
        #endif
        dmpReady = true;

        // get expected DMP packet size for later comparison
        setDemux(1);
        packetSize1 = mpu1.dmpGetFIFOPacketSize();
        setDemux(2);
        packetSize2 = mpu2.dmpGetFIFOPacketSize();
        
    } else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        #ifndef OUTPUT_QT_YAWPITCHROLL
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus1);
        Serial.print(" ");
        Serial.print(devStatus2);
        Serial.println(F(")"));
        #endif
    }

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    //while (!mpuInterrupt && fifoCount < packetSize) {
        // other program behavior stuff here
        // .
        // .
        // .
        // if you are really paranoid you can frequently test in between other
        // stuff to see if mpuInterrupt is true, and if so, "break;" from the
        // while() loop to immediately process the MPU data
        // .
        // .
        // .
    //}

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    setDemux(1);
    mpuIntStatus1 = mpu1.getIntStatus();
    setDemux(2);
    mpuIntStatus2 = mpu2.getIntStatus();

    // get current FIFO count
    setDemux(1);
    fifoCount1 = mpu1.getFIFOCount();
    setDemux(2);
    fifoCount2 = mpu2.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus1 & 0x10) || (mpuIntStatus2 & 0x10) || fifoCount1 == 1024 || fifoCount2 == 1024) {
        // reset so we can continue cleanly
        setDemux(1);
        mpu1.resetFIFO();
        setDemux(2);
        mpu2.resetFIFO();
        //#ifndef OUTPUT_QT_YAWPITCHROLL
        //Serial.println(F("FIFO overflow!"));
        //#endif

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if ((mpuIntStatus1 & 0x02) || (mpuIntStatus2 & 0x02)) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount1 < packetSize1 && fifoCount2 < packetSize2)  {
          setDemux(1);
          fifoCount1 = mpu1.getFIFOCount();
          setDemux(2);
          fifoCount2 = mpu2.getFIFOCount();
        }

        // read a packet from FIFO
        setDemux(1);
        mpu1.getFIFOBytes(fifoBuffer1, packetSize1);
        setDemux(2);
        mpu2.getFIFOBytes(fifoBuffer2, packetSize2);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        setDemux(1);
        fifoCount1 -= packetSize1;
        setDemux(2);
        fifoCount2 -= packetSize2;
        
        #ifdef OUTPUT_QT_YAWPITCHROLL
            // display Euler angles in degrees
            mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
            mpu1.dmpGetGravity(&gravity1, &q1);
            mpu1.dmpGetYawPitchRoll(ypr1, &q1, &gravity1);
            Serial.print(ypr1[0] * 180/M_PI);
            Serial.print(";");
            Serial.print(ypr1[1] * 180/M_PI);
            Serial.print(";");
            Serial.print(ypr1[2] * 180/M_PI);
            Serial.print(";");
            
            mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
            mpu2.dmpGetGravity(&gravity2, &q2);
            mpu2.dmpGetYawPitchRoll(ypr2, &q2, &gravity2);
            Serial.print(ypr2[0] * 180/M_PI);
            Serial.print(";");
            Serial.print(ypr2[1] * 180/M_PI);
            Serial.print(";");
            Serial.print(ypr2[2] * 180/M_PI);
            Serial.print("\n");
        #endif
        
        #ifdef OUTPUT_READABLE_QUATERNION
            // display quaternion values in easy matrix form: w x y z
            mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
            Serial.print("quat 1\t");
            Serial.print(q1.w);
            Serial.print("\t");
            Serial.print(q1.x);
            Serial.print("\t");
            Serial.print(q1.y);
            Serial.print("\t");
            Serial.print(q1.z);
            Serial.print("\t");
            
            mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
            Serial.print("quat 2\t");
            Serial.print(q2.w);
            Serial.print("\t");
            Serial.print(q2.x);
            Serial.print("\t");
            Serial.print(q2.y);
            Serial.print("\t");
            Serial.println(q2.z);
        #endif

        #ifdef OUTPUT_READABLE_EULER
            // display Euler angles in degrees
            mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
            mpu1.dmpGetEuler(euler1, &q1);
            Serial.print("euler 1\t");
            Serial.print(euler1[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler1[1] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler1[2] * 180/M_PI);
            Serial.print("\t");
            
            mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
            mpu2.dmpGetEuler(euler2, &q2);
            Serial.print("euler 2\t");
            Serial.print(euler2[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(euler2[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(euler2[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_YAWPITCHROLL
            // display Euler angles in degrees
            mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
            mpu1.dmpGetGravity(&gravity1, &q1);
            mpu1.dmpGetYawPitchRoll(ypr1, &q1, &gravity1);
            Serial.print("ypr 1\t");
            Serial.print(ypr1[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr1[1] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr1[2] * 180/M_PI);
            Serial.print("\t");
            
            mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
            mpu2.dmpGetGravity(&gravity2, &q2);
            mpu2.dmpGetYawPitchRoll(ypr2, &q2, &gravity2);
            Serial.print("ypr 2\t");
            Serial.print(ypr2[0] * 180/M_PI);
            Serial.print("\t");
            Serial.print(ypr2[1] * 180/M_PI);
            Serial.print("\t");
            Serial.println(ypr2[2] * 180/M_PI);
        #endif

        #ifdef OUTPUT_READABLE_REALACCEL
            // display real acceleration, adjusted to remove gravity
            mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
            mpu1.dmpGetAccel(&aa1, fifoBuffer1);
            mpu1.dmpGetGravity(&gravity1, &q1);
            mpu1.dmpGetLinearAccel(&aaReal1, &aa1, &gravity1);
            Serial.print("areal 1\t");
            Serial.print(aaReal1.x);
            Serial.print("\t");
            Serial.print(aaReal1.y);
            Serial.print("\t");
            Serial.print(aaReal1.z);
            Serial.print("\t");
            
            mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
            mpu2.dmpGetAccel(&aa2, fifoBuffer2);
            mpu2.dmpGetGravity(&gravity2, &q2);
            mpu2.dmpGetLinearAccel(&aaReal2, &aa2, &gravity2);
            Serial.print("areal 2\t");
            Serial.print(aaReal2.x);
            Serial.print("\t");
            Serial.print(aaReal2.y);
            Serial.print("\t");
            Serial.println(aaReal2.z);
        #endif

        #ifdef OUTPUT_READABLE_WORLDACCEL
            // display initial world-frame acceleration, adjusted to remove gravity
            // and rotated based on known orientation from quaternion
            mpu1.dmpGetQuaternion(&q1, fifoBuffer1);
            mpu1.dmpGetAccel(&aa1, fifoBuffer1);
            mpu1.dmpGetGravity(&gravity1, &q1);
            mpu1.dmpGetLinearAccel(&aaReal1, &aa1, &gravity1);
            mpu1.dmpGetLinearAccelInWorld(&aaWorld1, &aaReal1, &q1);
            Serial.print("aworld 1\t");
            Serial.print(aaWorld1.x);
            Serial.print("\t");
            Serial.print(aaWorld1.y);
            Serial.print("\t");
            Serial.print(aaWorld1.z);
            Serial.print("\t");
            
            mpu2.dmpGetQuaternion(&q2, fifoBuffer2);
            mpu2.dmpGetAccel(&aa2, fifoBuffer2);
            mpu2.dmpGetGravity(&gravity2, &q2);
            mpu2.dmpGetLinearAccel(&aaReal2, &aa2, &gravity2);
            mpu2.dmpGetLinearAccelInWorld(&aaWorld2, &aaReal2, &q2);
            Serial.print("aworld 2\t");
            Serial.print(aaWorld2.x);
            Serial.print("\t");
            Serial.print(aaWorld2.y);
            Serial.print("\t");
            Serial.println(aaWorld2.z);
        #endif
    
        #ifdef OUTPUT_TEAPOT
            // display quaternion values in InvenSense Teapot demo format:
            teapotPacket1[2] = fifoBuffer1[0];
            teapotPacket1[3] = fifoBuffer1[1];
            teapotPacket1[4] = fifoBuffer1[4];
            teapotPacket1[5] = fifoBuffer1[5];
            teapotPacket1[6] = fifoBuffer1[8];
            teapotPacket1[7] = fifoBuffer1[9];
            teapotPacket1[8] = fifoBuffer1[12];
            teapotPacket1[9] = fifoBuffer1[13];
            Serial.write(teapotPacket1, 14);
            teapotPacket1[11]++; // packetCount, loops at 0xFF on purpose
            
            teapotPacket2[2] = fifoBuffer2[0];
            teapotPacket2[3] = fifoBuffer2[1];
            teapotPacket2[4] = fifoBuffer2[4];
            teapotPacket2[5] = fifoBuffer2[5];
            teapotPacket2[6] = fifoBuffer2[8];
            teapotPacket2[7] = fifoBuffer2[9];
            teapotPacket2[8] = fifoBuffer2[12];
            teapotPacket2[9] = fifoBuffer2[13];
            Serial.write(teapotPacket2, 14);
            teapotPacket2[11]++;
        #endif

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}
