#ifndef MPU6050WAGWrapper_h
#define MPU6050WAGWrapper_h
/*

	Anything for the MPU6050. This includes a lot of prewritten stuff

*/
//#include <Arduino.h>
#include "WAGBandCommon.h"

/* 

	Below is a lot of licenses etc for I2C stuff

*/

	/////////////////////////////////////////////////////////////////////////////////

	// I2C device class (I2Cdev) demonstration Arduino sketch for MPU6050 class using DMP (MotionApps v2.0)
	// 6/21/2012 by Jeff Rowberg <jeff@rowberg.net>
	// Updates should (hopefully) always be available at https://github.com/jrowberg/i2cdevlib

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

	//#include "MPU6050_6Axis_MotionApps20.h"
	class MPU6050;

	// Arduino Wire library is required if I2Cdev I2CDEV_ARDUINO_WIRE implementation
	// is used in I2Cdev.h
	#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
	    #include <i2c_t3.h>
	#endif

#define TEENSY_MPU_INTERRUPT_PIN 17
#define LED_PIN 13 // (Arduino is 13, Teensy is 13, Teensy++ is 6)

class MPU6050WAGWrapper{
public:
	MPU6050WAGWrapper();
	~MPU6050WAGWrapper();

	// class default I2C address is 0x68
	// specific I2C addresses may be passed as a parameter here
	// AD0 low = 0x68 (default for SparkFun breakout and InvenSense evaluation board)
	// AD0 high = 0x69
	MPU6050* mpu;

	volatile bool mpuInterrupt = false;     // indicates whether MPU interrupt pin has gone high

	void dmpDataReady();

	void loadAccelGyroOffsets(int xAccel, int yAccel, int zAccel, int xGyro, int yGyro, int zGyro);

	bool blinkState = false;
	// MPU control/status vars
	bool dmpReady = false;  // set true if DMP init was successful
	uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
	uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
	uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
	uint16_t fifoCount;     // count of all bytes currently in FIFO
	uint8_t fifoBuffer[64]; // FIFO storage buffer
	// packet structure for InvenSense teapot demo
	
	uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

	// ================================================================
	// ===               INTERRUPT DETECTION ROUTINE                ===
	// ================================================================
	

	int beginConfigureMPU6050();

	void finishMPU6050Setup();

	void extractMPU6050ValsAndSendToESP8266();

};


#endif