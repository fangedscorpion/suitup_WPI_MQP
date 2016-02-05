// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class
// 10/7/2011 by Jeff Rowberg <jeff@rowberg.net>
// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib
//
// Changelog:
//      2013-05-08 - added multiple output formats
//                 - added seamless Fastwire support
//      2011-10-07 - initial release

/* ============================================
I2Cdev device library code is placed under the MIT license
Copyright (c) 2011 Jeff Rowberg

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

// MANUAL CALIBRATION VALUES

    // ITG/MPU CALIBRATION (GREEN LIGHT CHIP)
//    mpu.setXAccelOffset(-3301);
//    mpu.setYAccelOffset(-4657);
//    mpu.setZAccelOffset(1548);
//    mpu.setXGyroOffset(-14);
//    mpu.setYGyroOffset(-1);
//    mpu.setZGyroOffset(2);

    // MPU-6050 CALIBRATION (RED LIGHT CHIP)
//    mpu.setXAccelOffset(-3365);
//    mpu.setYAccelOffset(-2685);
//    mpu.setZAccelOffset(1447);
//    mpu.setXGyroOffset(77);
//    mpu.setYGyroOffset(35);
//    mpu.setZGyroOffset(33);


// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
#include <math.h>

// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
// is used in I2Cdev.h
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
//    #include "Wire.h"
    #include <i2c_t3.h>
#endif

// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
MPU6050 mpu;
//MPU6050 mpu(0x69); // <-- use for AD0 high

int16_t ax, ay, az;
int16_t gx, gy, gz;

int16_t adjustCalibration(int16_t agAxis, int16_t delta);

#define LED_PIN 13 //Teensy3.2 using pin 4
bool blinkState = false;
#define AX 0
#define AY 1
#define AZ 2
#define GX 3
#define GY 4
#define GZ 5
#define WINDOW 100
int counter = 0;
int16_t calArray[6][WINDOW];
int32_t calAvg[6] = {0,0,0,0,0,0};
int16_t calAvgPrev[6] {32000,32000,32000,32000,32000,32000};
int16_t calGoals[6] = {0,0,16384,0,0,0};
int16_t calDiffs[6];
int16_t calCurr[6];
int16_t calFinal[6] = {0,0,0,0,0,0};
bool calFinalSet[6] = {false,false,false,false,false,false};
bool allCalFinalSet = false;

#define PRINT_DEBUG

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(38400);

    // initialize device
    Serial.println(F("Initializing I2C devices..."));
    mpu.initialize();

    // verify connection
    Serial.println(F("Testing device connections..."));
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));
    Serial.print("\n");

    // configure Arduino LED for
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    mpu.setXAccelOffset(0);
    mpu.setYAccelOffset(0);
    mpu.setZAccelOffset(0);
    mpu.setXGyroOffset(0);
    mpu.setYGyroOffset(0);
    mpu.setZGyroOffset(0);
    
}

void loop() {
    if (counter >= WINDOW){
        // reset counter
        counter = 0;
        
        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);

        // calculate the average values of each acc/gyro axis over the window
        for (int i = 0; i < 6; i++){
            // reset avg
            calAvg[i] = 0;
            // sum
            for (int j = 0; j < WINDOW; j++)
                calAvg[i] += calArray[i][j];
            // average
            calAvg[i] /= WINDOW;
        }

        // calculate the differences between avg actual values and goal values
        for (int i = 0; i < 6; i++){
            calDiffs[i] = calGoals[i] - calAvg[i];
        }

        //if calDiffs within a good tolerance, hold those calibration values as final
        for (int i = 0; i < 6; i++){
            if (abs(calDiffs[i]) < 5){
                calFinal[i] = calCurr[i];
                calFinalSet[i] = true;
            }
        }
        
        // adjust the calibration values
        for (int i = 0; i < 6; i++){
            // if calFinalSet, don't change the calibration value
            if (calFinalSet[i]){
                calCurr[i] = adjustCalibration(i,0);
            }
            // otherwise adjust the calibration value according to its error
            else {
                // large adjustments: adjust by error/10
                if (abs(calDiffs[i]) > 30){
                    calCurr[i] = adjustCalibration(i,calDiffs[i]/10);
                }
                // small adjustments: change by +/-1
                else {
                    calCurr[i] = adjustCalibration(i,copysign(1,calDiffs[i]));
                }
            }
        }

        // allCalFinalSet becomes false if any of calFinalSet are true
        allCalFinalSet = true;
        for (int i = 0; i < 6; i++){
            allCalFinalSet &= calFinalSet[i];
        }
        
        #ifdef PRINT_DEBUG
            Serial.println(allCalFinalSet);
    
            Serial.print("cal: \t");
            for (int i = 0; i < 6; i++){
                Serial.print(calCurr[i]);
                Serial.print("\t");
            }
            Serial.print("\n");
    
            Serial.print("avg: \t");
            for (int i = 0; i < 6; i++){
                Serial.print(calAvg[i]);
                Serial.print("\t");
            }
            Serial.print("\n");
    
            Serial.print("dif: \t");
            for (int i = 0; i < 6; i++){
                Serial.print(calDiffs[i]);
                Serial.print("\t");
            }
            Serial.print("\n");
    
            Serial.print("set: \t");
            for (int i = 0; i < 6; i++){
                Serial.print(calFinalSet[i]);
                Serial.print("\t");
            }
            Serial.print("\n\n");
        #endif // #ifdef PRINT_DEBUG

        if (allCalFinalSet){
            #ifdef PRINT_DEBUG
                Serial.print(F("---------------------------------------------\n"));
                Serial.print("\n");
            #endif // #ifdef PRINT_DEBUG
            for (int i = 0; i < 6; i++){
                Serial.print(calCurr[i]);
                Serial.print("\n");
            }
            Serial.print("\n");
            delay(1000);
            exit(0);
        }
    }


    delay(20);
    
    // read raw accel/gyro measurements from device
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    for (int i = 0; i < 6; i++){
        switch (i){
            case AX:
              calArray[i][counter] = ax;
              break;
            case AY:
              calArray[i][counter] = ay;
              break;
            case AZ:
              calArray[i][counter] = az;
              break;
            case GX:
              calArray[i][counter] = gx;
              break;
            case GY:
              calArray[i][counter] = gy;
              break;
            case GZ:
              calArray[i][counter] = gz;
              break;
        }
    }
    
    counter++;
}


int16_t adjustCalibration(int16_t agAxis, int16_t delta){
    int16_t val;
    switch (agAxis){
        case AX:
          val = delta + mpu.getXAccelOffset();
          mpu.setXAccelOffset(val);
          break;
        case AY:
          val = delta + mpu.getYAccelOffset();
          mpu.setYAccelOffset(val);
          break;
        case AZ:
          val = delta + mpu.getZAccelOffset();
          mpu.setZAccelOffset(val);
          break;
        case GX:
          val = delta + mpu.getXGyroOffset();
          mpu.setXGyroOffset(val);
          break;
        case GY:
          val = delta + mpu.getYGyroOffset();
          mpu.setYGyroOffset(val);
          break;
        case GZ:
          val = delta + mpu.getZGyroOffset();
          mpu.setZGyroOffset(val);
          break;
    }
    return val;
}

