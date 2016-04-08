// The main code for the Teensy running on the WAG Chestpiece PCBs
// Written by Team Suit Up!
// 2015-2016 for MQP


#include "ESP8266Comms.h" //All the listener code for the ESP8266
#include "BatteryMonitor.h" //Low battery averaging code
#include "MPU6050WAGWrapper.h" //All the boring MPU6050 stuff


/* 
 *  EasyVR code and interface to the Arduino
 */
#define INTERRUPT_FROM_ARDUINO 3
#define RST_ACTIVE_LOW 2
#define ARDUINO_SERIAL Serial1
volatile char receivedCmdFromArduino; //To receive in the interrupt
volatile boolean heardArduino = false; //flag for testing printing

void listenToArduino(){ // ISR for arduino
    heardArduino = true;
}

void enableVoiceControl(){
  pinMode(INTERRUPT_FROM_ARDUINO, INPUT);
  digitalWrite(INTERRUPT_FROM_ARDUINO, HIGH); //Pull-ups
  attachInterrupt(INTERRUPT_FROM_ARDUINO, listenToArduino, FALLING);  
  ARDUINO_SERIAL.begin(9600);
  heardArduino = false;
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);
  pinMode(RST_ACTIVE_LOW, OUTPUT);
  digitalWrite(RST_ACTIVE_LOW,HIGH);
}

ESP8266Comms esp8266; //Setup comms to ESP8266 module
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

    esp8266.zeroErrorCalculations();

    enableVoiceControl();
}

#define NUMBER_OF_STOP_MSGS_TO_SEND 8
#define NUMBER_OF_START_MSGS_TO_SEND 5

// ================================================================
// ===                    MAIN PROGRAM LOOP                     ===
// ================================================================
void loop() {
    unsigned long time1 = millis();

    battMonitor.checkBattery(); 

    boolean gotCMD = false;
    if(heardArduino){ //Had a voice control message come in
      
      if(ARDUINO_SERIAL.available()){
        receivedCmdFromArduino = ARDUINO_SERIAL.read();
        DEBUG_SERIAL.print("Arduino cmd: ");
        DEBUG_SERIAL.println(receivedCmdFromArduino);
        gotCMD = true;
      }
      
      heardArduino = false;
    }
    
    mpu6050Jawn.extractMPU6050Vals(esp8266.msgToESP8266); //Extracts and packs them all in 1

    if(battMonitor.hasLowBat()){
      if(gotCMD && receivedCmdFromArduino == 'S'){ // Stop
        DEBUG_SERIAL.println("LB_STOP");
        for(int i = 0; i < NUMBER_OF_STOP_MSGS_TO_SEND; i++){
          esp8266.sendMsgToESP8266(ESP8266_CMD_VOICE_STOP_LOW_BATT); //So much serial data already into ESP 
          delay(1); 
        }
      }
      else if(gotCMD && receivedCmdFromArduino == 'R'){ // RUN
        DEBUG_SERIAL.println("LB_RUN");
        for(int i = 0; i < NUMBER_OF_START_MSGS_TO_SEND; i++){
          esp8266.sendMsgToESP8266(ESP8266_CMD_VOICE_START_LOW_BATT); //So much serial data already into ESP
          delay(1);  
        }
      }
      else if(gotCMD && receivedCmdFromArduino == 'A'){ // ACTION
        DEBUG_SERIAL.println("LB_ACTION");
        for(int i = 0; i < NUMBER_OF_START_MSGS_TO_SEND; i++){
          esp8266.sendMsgToESP8266(ESP8266_CMD_VOICE_ACTION_LOW_BATT); //So much serial data already into ESP
          delay(1);  
        }
      }
      else{ //Either didn't get command or wasn't a valid command
        esp8266.sendMsgToESP8266(ESP8266_CMD_MPU6050_DATA_LOW_BATT);  
      }
    }
    else{
      if(gotCMD && receivedCmdFromArduino == 'S'){ // Stop
        DEBUG_SERIAL.println("STOP");
        for(int i = 0; i < NUMBER_OF_STOP_MSGS_TO_SEND; i++){
          esp8266.sendMsgToESP8266(ESP8266_CMD_VOICE_STOP); //So much serial data already into ESP
          delay(1);  
        }
      }
      else if(gotCMD && receivedCmdFromArduino == 'R'){ // RUN
        DEBUG_SERIAL.println("RUN");
        for(int i = 0; i < NUMBER_OF_START_MSGS_TO_SEND; i++){
          esp8266.sendMsgToESP8266(ESP8266_CMD_VOICE_START); //So much serial data already into ESP 
          delay(1); 
        }
      }
      else if(gotCMD && receivedCmdFromArduino == 'A'){ // ACTION
        DEBUG_SERIAL.println("ACTION");
        for(int i = 0; i < NUMBER_OF_START_MSGS_TO_SEND; i++){
          esp8266.sendMsgToESP8266(ESP8266_CMD_VOICE_ACTION); //So much serial data already into ESP 
          delay(1); 
        }
      }
      else{ //Either didn't get command or wasn't a valid command
        esp8266.sendMsgToESP8266(ESP8266_CMD_MPU6050_DATA);  
      }
    }
    
    gotCMD = false;
    unsigned long timePassed = (millis()-time1);
    //DEBUG_SERIAL.println(String("Tm:")+timePassed);
    if(timePassed < 5){ // Fill out time until the 6ms mark to make serial more usable
      delay(5-timePassed);
    }
    DEBUG_SERIAL.println(String("Tm:")+(millis() - time1));
}
