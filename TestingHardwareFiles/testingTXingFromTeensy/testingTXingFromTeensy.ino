#define ESP8266_SERIAL Serial3
#define DEBUG_SERIAL Serial

#define MSG_TO_ESP8266_ALIGN_BYTES 4
#define MSG_TO_ESP8266_MSG_SIZE 8 //For sending back to the ESP8266
#define MSG_TO_ESP8266_TOTAL_SIZE (MSG_TO_ESP8266_ALIGN_BYTES + MSG_TO_ESP8266_MSG_SIZE)

#define ESP8266_START_BYTE 254

#define ESP8266_MIN_CMD_BYTE 192 //Minimum number to be sent to complete the packet
#define ESP8266_CMD_NO_AXN ESP8266_MIN_CMD_BYTE
//Commands are defined as follows (anything above the minimum value up to 253)
#define ESP8266_CMD_MPU6050_NO_DATA 205 //Sent to the ESP8266 from the teensy if there is no data
#define ESP8266_CMD_MPU6050_DATA 206 //Sent if there is data following
#define COULD_NOT_ALIGN_START_BYTE 0x00

uint8_t mpu6050Data[MSG_TO_ESP8266_MSG_SIZE] = {0,0,0,0,0,0,0,0};


void setup() {
  ESP8266_SERIAL.begin(9600);
  DEBUG_SERIAL.begin(115200);

}

char synchronizeSerialBeginning(){
  char foundCMD = COULD_NOT_ALIGN_START_BYTE;

  while(ESP8266_SERIAL.peek() != ESP8266_START_BYTE && ESP8266_SERIAL.peek() != -1){
      DEBUG_SERIAL.print(ESP8266_SERIAL.read()); //Throw out character
  }
  DEBUG_SERIAL.println();
  if(ESP8266_SERIAL.peek() == -1){
    return COULD_NOT_ALIGN_START_BYTE;
  }

  for(int i = 1; i <= 3; i++){
    if(ESP8266_SERIAL.peek() != ESP8266_START_BYTE){
      return COULD_NOT_ALIGN_START_BYTE;
    }
    DEBUG_SERIAL.print(ESP8266_SERIAL.read()); //Throw out char
  }
  DEBUG_SERIAL.println();
  
  if(ESP8266_SERIAL.peek() < ESP8266_MIN_CMD_BYTE){
    return COULD_NOT_ALIGN_START_BYTE;
  }

  foundCMD = ESP8266_SERIAL.read();
  DEBUG_SERIAL.println(String("CMD: ") + foundCMD);
  
  return foundCMD;
}

//Returns a true or false to indicate it received data properly or not
int readFromTeensy(){ 
    
    while(ESP8266_SERIAL.available() < MSG_TO_ESP8266_TOTAL_SIZE){
      delayMicroseconds(1);
    }
    
   char foundBeginningCMD = synchronizeSerialBeginning();
   if(foundBeginningCMD == COULD_NOT_ALIGN_START_BYTE){
      DEBUG_SERIAL.println("Could not find beginning");
      return false;
   }
   else { //Found the beginning! :D 
      switch(foundBeginningCMD){
        case ESP8266_CMD_NO_AXN: DEBUG_SERIAL.println("NO ACTION WAS FOUND IN readFromTeensy() FUNC"); return true; break;
        case ESP8266_CMD_MPU6050_NO_DATA: DEBUG_SERIAL.println("No data CMD from Teensy"); return true; break;
        case ESP8266_CMD_MPU6050_DATA: DEBUG_SERIAL.println("Data"); break;
        default: DEBUG_SERIAL.println("Unknown CMD"); return true; break;
      }
   }
   //Read the data! 
   while(ESP8266_SERIAL.available() < MSG_TO_ESP8266_MSG_SIZE){
      delayMicroseconds(1);
   }
   //Read bytes for plaback! 
   DEBUG_SERIAL.print("Data: ");
   for(int i = 0; i < MSG_TO_ESP8266_MSG_SIZE; i++){
      mpu6050Data[i] = ESP8266_SERIAL.read();
      DEBUG_SERIAL.print(char(mpu6050Data[i]));
   }
    DEBUG_SERIAL.println();
    
    return true;
}

void loop() {
  DEBUG_SERIAL.println(readFromTeensy());
}
