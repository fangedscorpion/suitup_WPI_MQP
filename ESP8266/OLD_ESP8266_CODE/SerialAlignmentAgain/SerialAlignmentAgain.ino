#define MSG_TO_ESP8266_ALIGN_BYTES 4
#define MSG_TO_ESP8266_MSG_SIZE 8 //For sending back to the ESP8266
#define MSG_TO_ESP8266_TOTAL_SIZE (MSG_TO_ESP8266_ALIGN_BYTES + MSG_TO_ESP8266_MSG_SIZE)

#define ESP8266_START_BYTE 253

#define ESP8266_SERIAL Serial
#define DEBUG_SERIAL Serial

////Extra things to hold spots for enums when not being used for checking
//#define BROKEN_PACKET 255
//#define NOTHING_NEW 254
//
//#define POSITION_DATA_BYTES 12
//#define NON_POSITION_DATA_BYTES_MAX 4
//#define RECORDING_MSG_SIZE 11
//
//#define ESP8266_MIN_CMD_BYTE 192 //Minimum number to be sent to complete the packet
//#define ESP8266_CMD_NO_AXN ESP8266_MIN_CMD_BYTE
////Commands are defined as follows (anything above the minimum value up to 253)
//#define ESP8266_CMD_MPU6050_NO_DATA 205 //Sent to the ESP8266 from the teensy if there is no data
//#define ESP8266_CMD_MPU6050_DATA 206 //Sent if there is data following
//#define ESP8266_CMD_MPU6050_DATA_LOW_BATT  207 //Send if low battery
//#define ESP8266_CMD_MPU6050_NO_DATA_LOW_BATT  208 //Send if low battery
//#define COULD_NOT_ALIGN_START_BYTE 0x00


//boolean lowBattery = false;
//
//char mpu6050Data[MSG_TO_ESP8266_MSG_SIZE] = {0,0,0,0,0,0,0,0};
//
//#define POSITION_DATA_BYTES 12
//char positionalFBData[POSITION_DATA_BYTES];
//char msgDataNonPosition[NON_POSITION_DATA_BYTES_MAX];
//
//#define MPU_DATA_SIZE 14
//
//int incomingByte;
//boolean foundMPUDataStart = false;
//char incomingDataMsg[MPU_DATA_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,'\r','\n'};
//#define RECORDING_MSG_TYPE_POS 1    
//char recordingMsg[RECORDING_MSG_SIZE] = {0x0A, 14, 0,0,0,0,0,0,0,0,'\n'};
//
//
//char synchronizeSerialBeginning(){
//  char foundCMD = COULD_NOT_ALIGN_START_BYTE;
//
//  while(ESP8266_SERIAL.peek() != ESP8266_START_BYTE && ESP8266_SERIAL.peek() != -1){
//      DEBUG_SERIAL.print(ESP8266_SERIAL.read()); //Throw out character
//      DEBUG_SERIAL.print(" "); //Throw out character
//  }
//  DEBUG_SERIAL.println();
//  if(ESP8266_SERIAL.peek() == -1){
//    return COULD_NOT_ALIGN_START_BYTE;
//  }
//
//  for(int i = 1; i <= 3; i++){
//    if(ESP8266_SERIAL.peek() != ESP8266_START_BYTE){
//      return COULD_NOT_ALIGN_START_BYTE;
//    }
//    DEBUG_SERIAL.print(ESP8266_SERIAL.read()); //Throw out char
//  }
//  DEBUG_SERIAL.println();
//  
//  if(ESP8266_SERIAL.peek() < ESP8266_MIN_CMD_BYTE){
//    return COULD_NOT_ALIGN_START_BYTE;
//  }
//
//  foundCMD = ESP8266_SERIAL.read();
//  DEBUG_SERIAL.println(String("CMD: ") + foundCMD);
//  
//  return foundCMD;
//}
//
////Returns a true or false to indicate it received data properly or not
//int readFromTeensy(){ 
//    
//    while(ESP8266_SERIAL.available() < MSG_TO_ESP8266_TOTAL_SIZE){
//      delayMicroseconds(1);
//      //delay(1);
//    }
//    
//   char foundBeginningCMD = synchronizeSerialBeginning();
//   if(foundBeginningCMD == COULD_NOT_ALIGN_START_BYTE){
//      DEBUG_SERIAL.println("Could not find beginning");
//      return false;
//   }
//   else { //Found the beginning! :D 
//      switch(foundBeginningCMD){
//        case ESP8266_CMD_NO_AXN: DEBUG_SERIAL.println("NO ACTION WAS FOUND IN readFromTeensy() FUNC"); return true; break;
//        case ESP8266_CMD_MPU6050_NO_DATA: DEBUG_SERIAL.println("No data CMD from Teensy"); lowBattery = false; return true; break;
//        case ESP8266_CMD_MPU6050_DATA: DEBUG_SERIAL.println("Data"); lowBattery = false; break;
//        case ESP8266_CMD_MPU6050_DATA_LOW_BATT: DEBUG_SERIAL.println("Data & Low Batt"); lowBattery = true; break;
//        case ESP8266_CMD_MPU6050_NO_DATA_LOW_BATT: DEBUG_SERIAL.println("No data & Low Batt"); lowBattery = true; return true; break;
//        default: DEBUG_SERIAL.println("Unknown CMD"); return true; break;
//      }
//   }
//   //Read the data! 
//   while(ESP8266_SERIAL.available() < MSG_TO_ESP8266_MSG_SIZE){
//      delayMicroseconds(1);
//      //delay(1);
//   }
//   //Read bytes for plaback! 
//   DEBUG_SERIAL.print("Data: ");
//   for(int i = 0; i < MSG_TO_ESP8266_MSG_SIZE; i++){
//      mpu6050Data[i] = ESP8266_SERIAL.read();
//      recordingMsg[i+2] = mpu6050Data[i];
//      DEBUG_SERIAL.print(char(mpu6050Data[i]));
//   }
//    DEBUG_SERIAL.println();
//    
//    return true;
//}

#define ESP8266_CMD_MPU6050_DATA 206 //Sent if there is data following
#define ESP8266_CMD_MPU6050_DATA_LOW_BATT  207 //Send if low battery
#define CMD_SLOT 1
#define RECORDING_MSG_SIZE 11
#define MSG_TO_ESP8266_MSG_SIZE 8
static const size_t bufferSize = 128;
static uint8_t sbuf[bufferSize];
static uint8_t recordingMsg[RECORDING_MSG_SIZE] = {0x0A, 0x00, 0,0,0,0,0,0,0,0,'\n'};

void readTeensySerial(){
  if (ESP8266_SERIAL.available() > (MSG_TO_ESP8266_TOTAL_SIZE-1)) {
    size_t len = Serial.available();
    ESP8266_SERIAL.print(len);
    while (len > 0) { // size_t is an unsigned type so >0 is actually redundent
      size_t will_copy = (len < bufferSize) ? len : bufferSize;
      ESP8266_SERIAL.readBytes(sbuf, will_copy);
      
      if(sbuf[0] == ESP8266_START_BYTE && sbuf[1] == ESP8266_START_BYTE && sbuf[2] == ESP8266_START_BYTE){ //Check for our packet from Teensy
        
        ESP8266_SERIAL.print("CMD:");
        ESP8266_SERIAL.print(char(sbuf[MSG_TO_ESP8266_ALIGN_BYTES-1]));
        
        boolean sendToPC = true;
        
        switch(sbuf[MSG_TO_ESP8266_ALIGN_BYTES-1]){ //Tell what type of cmd to add
            case ESP8266_CMD_MPU6050_DATA: 
                recordingMsg[CMD_SLOT] = 0x04;
                break;
            case ESP8266_CMD_MPU6050_DATA_LOW_BATT: 
                recordingMsg[CMD_SLOT] = 0x0E;
                break;
            default:
                sendToPC = false;
            break;
         }
        
        ESP8266_SERIAL.write(0x09);
        memcpy( &recordingMsg[CMD_SLOT+1], &sbuf[MSG_TO_ESP8266_ALIGN_BYTES], MSG_TO_ESP8266_MSG_SIZE); //Only copy over the data bytes
        
        for(int i = 0; i < MSG_TO_ESP8266_TOTAL_SIZE; i++){
          ESP8266_SERIAL.write(recordingMsg[i]);  //Write the packet to the PC  
        }
        
        delay(0); // yield
      }
      len -= will_copy;
    }
  }
}


void setup() {
  ESP8266_SERIAL.begin(115200);
  
}


void loop() {
  readTeensySerial();
}


