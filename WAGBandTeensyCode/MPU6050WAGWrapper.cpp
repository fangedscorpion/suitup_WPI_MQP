/*

	Everything for the MPU6050 with all crap not exposed 'cuz we don't need that you hoser

*/
#include "MPU6050WAGWrapper.h"

#include "MPU6050_6Axis_MotionApps20.h"

MPU6050WAGWrapper::MPU6050WAGWrapper(){
	mpu = new MPU6050();
}

MPU6050WAGWrapper::~MPU6050WAGWrapper(){
	
}

void MPU6050WAGWrapper::dmpDataReady() {
	    mpuInterrupt = true;
}

void MPU6050WAGWrapper::loadAccelGyroOffsets(int xAccel, int yAccel, int zAccel, int xGyro, int yGyro, int zGyro){
	mpu->setXAccelOffset(xAccel);
	mpu->setYAccelOffset(yAccel);
	mpu->setZAccelOffset(zAccel);
	mpu->setXGyroOffset(xGyro);
	mpu->setYGyroOffset(yGyro);
	mpu->setZGyroOffset(zGyro);
}

//returns the devStatus for checking if DMP worked
int MPU6050WAGWrapper::beginConfigureMPU6050(){ 
  	// join I2C bus (I2Cdev library doesn't do this automatically)
    #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
        Wire.begin();
        #ifdef __AVR__
          TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)
        #endif
        #ifdef __ARM__ //Had to add this as the Teensy3.2 is ARM based not AVR :( but it compiles now!
          uint8_t twbrback = TWI_CWGR_CKDIV;
          TWI_CWGR_CKDIV(twbrback/4); //400 kHz
        #endif
    #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
        Fastwire::setup(400, true);
    #endif
    

    // initialize device
    DEBUG_SERIAL.println(F("Initializing I2C devices..."));
    mpu->initialize();

    // verify connection
    DEBUG_SERIAL.println(F("Testing device connections..."));
    DEBUG_SERIAL.println(mpu->testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    DEBUG_SERIAL.println(F("Initializing DMP..."));
    devStatus = mpu->dmpInitialize();
    
    this->loadAccelGyroOffsets(-5363, -4791, 1338, -1064, 468, 10);
//     mpu.setXAccelOffset(-5363);
//     mpu.setYAccelOffset(-4791);
//     mpu.setZAccelOffset(1338);
//     mpu.setXGyroOffset(-1064);
//     mpu.setYGyroOffset(468);
//     mpu.setZGyroOffset(10);

    // make sure it worked (returns 0 if so)
    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        DEBUG_SERIAL.println(F("Enabling DMP..."));
        mpu->setDMPEnabled(true);
    }
    else {
        // ERROR!
        // 1 = initial memory load failed
        // 2 = DMP configuration updates failed
        // (if it's going to break, usually the code will be 1)
        DEBUG_SERIAL.print(F("DMP Initialization failed (code "));
        DEBUG_SERIAL.print(devStatus);
        DEBUG_SERIAL.println(F(")"));
    }

    return devStatus;
}

void MPU6050WAGWrapper::finishMPU6050Setup(){ //Only called if works on MPU6050
	mpuIntStatus = mpu->getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    DEBUG_SERIAL.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;

    // get expected DMP packet size for later comparison
    packetSize = mpu->dmpGetFIFOPacketSize();

    // configure LED for output
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);
}


void MPU6050WAGWrapper::extractMPU6050ValsAndSendToESP8266(){
  
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU interrupt or extra packet(s) available
    while (!mpuInterrupt && fifoCount < packetSize) {
    }

    // reset interrupt flag and get INT_STATUS byte
    mpuInterrupt = false;
    mpuIntStatus = mpu->getIntStatus();

    // get current FIFO count
    fifoCount = mpu->getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu->resetFIFO();
        DEBUG_SERIAL.println(F("FIFO overflow!"));

    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu->getFIFOCount();

        // read a packet from FIFO
        mpu->getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;
        
        // display quaternion values in InvenSense Teapot demo format:
        teapotPacket[2] = fifoBuffer[0];
        teapotPacket[3] = fifoBuffer[1];
        teapotPacket[4] = fifoBuffer[4];
        teapotPacket[5] = fifoBuffer[5];
        teapotPacket[6] = fifoBuffer[8];
        teapotPacket[7] = fifoBuffer[9];
        teapotPacket[8] = fifoBuffer[12];
        teapotPacket[9] = fifoBuffer[13];
        //DEBUG_SERIAL.write(teapotPacket, 14);
        ESP8266_SERIAL.write(teapotPacket,14);
        teapotPacket[11]++; // packetCount, loops at 0xFF on purpose
        delayMicroseconds(100);//Trying to make this robust here as the ESP8266 chip can die

        // blink LED to indicate activity
        blinkState = !blinkState;
        digitalWrite(LED_PIN, blinkState);
    }
}