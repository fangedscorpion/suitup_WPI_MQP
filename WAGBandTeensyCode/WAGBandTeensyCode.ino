// The main code for the Teensy running on the WAG Band PCBs
// Written by Team Suit Up!
// 2015-2016 for MQP

#include "WAGBandCommon.h"
#include "ESP8266Comms.h" //All the listener code for the ESP8266
#include "VibrationPattern.h" //Everything to run the motors for haptics
#include "BatteryMonitor.h" //Low battery averaging code

#include "MPU6050WAGWrapper.h" //All the boring MPU6050 stuff

ESP8266Comms esp8266; //Setup comms to ESP8266 module
VibrationPattern motorController; //To control motors
BatteryMonitor battMonitor; //Checking on low battery information

//OPTIONS FOR THE BAND ARE LISTED IN WAGBandCommon.h
// Numbers for bands in WAGBandCommon.h
MPU6050WAGWrapper mpu6050Jawn(LEFT_UPPER_ARM_BAND_NUM); // Pass this in so it loads the calibration automatically in MPU6050WAGWrapper.cpp 

// Friggin ISR for MPU6050 --> No good way to attach ISR in a class :(
void ISR_MPU6050(){
  mpu6050Jawn.dmpDataReady();
}


////////////////////////////////////////////////////////////
/*
 * Serial synchronization with ESP8266
 */

//Circular buffer for serial data
int serialDataHeadPointer = 0; //always one ahead of the data
int serialDataTailPointer = 0; // always on the data 
#define BUFFER_SIZE 2048
static uint8_t sbuf[BUFFER_SIZE];

#define PLAYBACK_MSG_SIZE 12
char playbackData[PLAYBACK_MSG_SIZE] = {0,0,0,0, 0,0,0,0, 0,0,0,0};
#define FEEDBACK_MSG_ALIGN_BYTES 4
#define FEEDBACK_MSG_DATA_BYTES 12
#define FEEDBACK_MSG_SIZE (FEEDBACK_MSG_ALIGN_BYTES + FEEDBACK_MSG_DATA_BYTES)

boolean isPlaybackMode = false;

int wrapCircularIndex(int index){
  if(index < 0){
    return (BUFFER_SIZE + index); 
  }
  else if(index > BUFFER_SIZE){
    return (index % 2048);
  }
  else if(index == BUFFER_SIZE){
    return 0;
  }
  return index;  
}

boolean readESP8266SerialSendPkt(boolean printStuff){
  size_t len = ESP8266_SERIAL.available();
  if (len >= (FEEDBACK_MSG_SIZE)) {
  
    if(printStuff){
      DEBUG_SERIAL.print(len);
    }
     
    if(serialDataHeadPointer + len >= BUFFER_SIZE){
        size_t writeLen = BUFFER_SIZE - serialDataHeadPointer; //figure out how long until the end of the array
        
        ESP8266_SERIAL.readBytes(&sbuf[serialDataHeadPointer], writeLen);
        if(printStuff){
            DEBUG_SERIAL.print("WR_WR1:");
            for(int i = serialDataHeadPointer; i < serialDataHeadPointer+writeLen; i++){
                  DEBUG_SERIAL.print(char(sbuf[i]));
            }
        }
        
        serialDataHeadPointer = 0; // Wrap around
  
        len -= writeLen; 
  
        if(len > 0){ //Read anything beyond our buffer
            ESP8266_SERIAL.readBytes(&sbuf[serialDataHeadPointer], len);
            
            if(printStuff){
                DEBUG_SERIAL.print("WR_WR2:");
                for(int i = serialDataHeadPointer; i < serialDataHeadPointer+len; i++){
                  DEBUG_SERIAL.print(char(sbuf[i]));
                }
            }
            
            
            serialDataHeadPointer += len;
        }
  
        if(printStuff){
          DEBUG_SERIAL.print("WR_");
          DEBUG_SERIAL.print(serialDataHeadPointer); 
          DEBUG_SERIAL.println(":"); 
        }
    }
      else{ //within bounds of buffer
        ESP8266_SERIAL.readBytes(&sbuf[serialDataHeadPointer], len);
        serialDataHeadPointer += len;
        if(printStuff){
          DEBUG_SERIAL.print("WR_WR:");
          for(int i = serialDataHeadPointer; i < serialDataHeadPointer+len; i++){
            DEBUG_SERIAL.print(char(sbuf[i]));
          }
          
            DEBUG_SERIAL.print("WR_");
            DEBUG_SERIAL.print(serialDataHeadPointer); 
            DEBUG_SERIAL.println(":"); 
        }
         
      }
    
    //Reading mode
    //Do a quick search backward to find latest data
    serialDataTailPointer = wrapCircularIndex(serialDataHeadPointer-FEEDBACK_MSG_SIZE);
    if(printStuff){
        DEBUG_SERIAL.print("RD_");
        DEBUG_SERIAL.print(serialDataTailPointer);
        DEBUG_SERIAL.print(":");
        DEBUG_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer)]));
        DEBUG_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+1)]));
        DEBUG_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+2)]));
        DEBUG_SERIAL.print("END_RD");
      }

      int count = 0;
      while(sbuf[wrapCircularIndex(serialDataTailPointer)] != ESP8266_START_BYTE || sbuf[wrapCircularIndex(serialDataTailPointer+1)] != ESP8266_START_BYTE || sbuf[wrapCircularIndex(serialDataTailPointer+2)] != ESP8266_START_BYTE){
         serialDataTailPointer = wrapCircularIndex(serialDataTailPointer-1);
         ++count;
         if(count > 32){
          return false;
         }
      }//This should always find data even if it's a bit old :/ but we shall see

      if(printStuff){
        DEBUG_SERIAL.print("RD_");
        DEBUG_SERIAL.print(serialDataTailPointer);
        DEBUG_SERIAL.print(":");
        DEBUG_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer)]));
        DEBUG_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+1)]));
        DEBUG_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+2)]));
        DEBUG_SERIAL.print("END_RD");
      }

        char theCMD = sbuf[wrapCircularIndex(serialDataTailPointer+3)];
        if(printStuff){
          DEBUG_SERIAL.print("CMD:");
          DEBUG_SERIAL.print(char(theCMD));
        }
          serialDataTailPointer = wrapCircularIndex(serialDataTailPointer+4);


          switch(theCMD){ //Tell what type of cmd to add
              case ESP8266_CMD_CONTINUE_PLAYBACK:
                
              break;
              case ESP8266_CMD_START_PLAYBACK:
                  isPlaybackMode = true;
              break;
              case ESP8266_CMD_STOP_PLAYBACK:
                  isPlaybackMode = false;
              break;
              default:
                  
              break;
           }

           if(printStuff){
              DEBUG_SERIAL.write(0x09);
            }
           if(serialDataTailPointer + FEEDBACK_MSG_DATA_BYTES >= BUFFER_SIZE){
                  size_t readLen = BUFFER_SIZE - serialDataTailPointer; //figure out how long until the end of the array
                  
                  memcpy( &playbackData[0], &sbuf[serialDataTailPointer], readLen); //Only copy over the data bytes
                  serialDataTailPointer = 0; // Wrap around

                  int remainingBytes = FEEDBACK_MSG_DATA_BYTES - readLen;
               
                  if(remainingBytes > 0){ //Copy anything beyond our buffer
                      memcpy( &playbackData[readLen], &sbuf[serialDataTailPointer], remainingBytes); //Only copy over the data bytes
                  }

            }
            else{ // Don't have to wrap around the buffer
                 memcpy( &playbackData[0], &sbuf[serialDataTailPointer], FEEDBACK_MSG_DATA_BYTES); //Only copy over the data bytes
            }

            if(printStuff){
              DEBUG_SERIAL.print("FB_DATA: ");
              for(int i = 0; i < FEEDBACK_MSG_DATA_BYTES; i++){
                 DEBUG_SERIAL.print(playbackData[i], HEX);
              }
              DEBUG_SERIAL.println();
            }
             

              

            //Copy over the errors
            for(int i = 0; i < FEEDBACK_MSG_DATA_BYTES; i++){
                if(i < 4){
                  esp8266.TransAngleUnion.bytes[i] = playbackData[i]; 
                  if(printStuff){
                    DEBUG_SERIAL.println(esp8266.TransAngleUnion.bytes[i]);
                  }
                }
                else if(i < 8){
                  esp8266.TransErrorUnion.bytes[i-4] = playbackData[i]; 
                  if(printStuff){
                    DEBUG_SERIAL.println(esp8266.TransErrorUnion.bytes[i-4]);  
                  }
                }
                else{
                  esp8266.RotatErrorUnion.bytes[i-8] = playbackData[i];
                  if(printStuff){
                    DEBUG_SERIAL.println(esp8266.RotatErrorUnion.bytes[i-8]);
                  }  
                }
            }


   }
   if(isPlaybackMode){
    esp8266.RX_trans_angle = esp8266.TransAngleUnion.f;
    esp8266.RX_err_trans = esp8266.TransErrorUnion.f;
    esp8266.RX_err_rot = esp8266.RotatErrorUnion.f;
    if(printStuff){
      DEBUG_SERIAL.println("Float errors");
      DEBUG_SERIAL.println(esp8266.RX_trans_angle);
      DEBUG_SERIAL.println(esp8266.RX_err_trans);
      DEBUG_SERIAL.println(esp8266.RX_err_rot); 
    }
   }
    return (isPlaybackMode);
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

    boolean readValues = readESP8266SerialSendPkt(false);
    if(readValues){
      motorController.updateErrors(esp8266.RX_trans_angle, esp8266.RX_err_trans, esp8266.RX_err_rot);
      motorController.performMotorCalculationsAndRunMotors();
    }
    else{
      motorController.stopAllMotors();
      esp8266.zeroErrorCalculations();
    }
    //DEBUG_SERIAL.println(String("Tm:")+(millis()-time1));
}
