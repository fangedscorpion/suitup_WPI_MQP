// The main code for the Teensy running on the WAG Band PCBs
// Written by Team Suit Up!
// 2015-2016 for MQP


#include "ESP8266Comms.h" //All the listener code for the ESP8266
#include "VibrationPattern.h" //Everything to run the motors for haptics
#include "BatteryMonitor.h" //Low battery averaging code

#include "MPU6050WAGWrapper.h" //All the boring MPU6050 stuff

ESP8266Comms esp8266; //Setup comms to ESP8266 module
VibrationPattern motorController; //To control motors
BatteryMonitor battMonitor; //Checking on low battery information
MPU6050WAGWrapper mpu6050Jawn;

// Friggin ISR for MPU6050 --> No good way to attach ISR in a class :(
void ISR_MPU6050(){
  mpu6050Jawn.dmpDataReady();
}



// ================================================================
// ===                      INITIAL SETUP                       ===
// ================================================================

void setup() {
    // initialize serial communication
    DEBUG_SERIAL.begin(115200);
    ESP8266_SERIAL.begin(115200); //ESP8266 breaks down at higher speeds?!?!

    ////////////////////////
    // Stupid MPU6050 crap because ISRs inside a class are janky
    int devStatus = mpu6050Jawn.beginConfigureMPU6050();
    if(devStatus == 0){
      // enable Arduino interrupt detection
      //DEBUG_SERIAL.println("Enabling interrupt detection (Teensy Pin 17)...");
      //pinMode(TEENSY_MPU_INTERRUPT_PIN,INPUT);
      //attachInterrupt(TEENSY_MPU_INTERRUPT_PIN, ISR_MPU6050, RISING); //actually attaches the MPU6050 interrupt
    
      mpu6050Jawn.finishMPU6050Setup();
    }
    else{
        DEBUG_SERIAL.print("DMP Initialization failed (code ");
        DEBUG_SERIAL.print(devStatus);
        while(1){
          DEBUG_SERIAL.println("DMP failed");
        }
    }
    ////////////////////////
    
    battMonitor.initLowBatteryInfo();

    mpu6050Jawn.loadAccelGyroOffsets(-3287, -4641, 1556, -14, -2, 5);//int xAccel, int yAccel, int zAccel, int xGyro, int yGyro, int zGyro);

    esp8266.zeroErrorCalculations();
}



// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================

void loop() {
    unsigned long time1 = millis();
    battMonitor.checkBattery(); 

    
//    mpu6050Jawn.extractMPU6050ValsDebug(esp8266.teapotPkt); //Extracts and packs them all in 1 --> For testing the device with the teapot demo to ensure good accelerometer data
//    esp8266.sendMsgToESP8266DebugMPU();  
    
    mpu6050Jawn.extractMPU6050Vals(esp8266.msgToESP8266); //Extracts and packs them all in 1
    
    if(battMonitor.hasLowBat()){
      esp8266.sendMsgToESP8266(ESP8266_CMD_MPU6050_DATA_LOW_BATT);  
    }
    else{
        esp8266.sendMsgToESP8266(ESP8266_CMD_MPU6050_DATA);  
    }

//    boolean readValues = esp8266.readFromESP8266();     
    boolean readValues = true;
    if(readValues){
      motorController.updateErrors(esp8266.RX_trans_angle, esp8266.RX_err_trans, esp8266.RX_err_rot);
    }
    motorController.performMotorCalculationsAndRunMotors();
    DEBUG_SERIAL.println(String("Tm:")+(millis()-time1));
}
