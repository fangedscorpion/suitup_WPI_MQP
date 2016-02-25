#include "ESP8266Comms.h"

ESP8266Comms::ESP8266Comms(){
	RX_trans_angle = 0;
    RX_err_trans = 0;
    RX_err_rot = 0;
}

ESP8266Comms::~ESP8266Comms(){
}

void ESP8266Comms::zeroErrorCalculations(){
	RX_trans_angle = 0;
    RX_err_trans = 0;
    RX_err_rot = 0;
}

char ESP8266Comms::synchronizeSerialBeginning(){
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
int ESP8266Comms::readFromESP8266(){ 
    
    while(ESP8266_SERIAL.available() < ESP8266_MSG_SIZE){
      delayMicroseconds(1);
    }
    
   char foundBeginningCMD = synchronizeSerialBeginning();
   if(foundBeginningCMD == COULD_NOT_ALIGN_START_BYTE){
      DEBUG_SERIAL.println("Could not find beginning");
      return false;
   }
   else { //Found the beginning! :D 
      switch(foundBeginningCMD){
        case ESP8266_CMD_NO_AXN: DEBUG_SERIAL.println("NO ACTION WAS FOUND IN readFromESP8266() FUNC"); return true; break;
        case ESP8266_CMD_START_RECORDING: DEBUG_SERIAL.println("Starting recording"); return true; break;
        case ESP8266_CMD_CONTINUE_RECORDING: DEBUG_SERIAL.println("Continuing recording"); return true; break;
        case ESP8266_CMD_STOP_RECORDING: DEBUG_SERIAL.println("Stopping recording"); return true; break;
        case ESP8266_CMD_START_PLAYBACK: DEBUG_SERIAL.println("Starting playback"); break; //Read bytes after
        case ESP8266_CMD_CONTINUE_PLAYBACK: DEBUG_SERIAL.println("CPB"); break; //Read bytes after
        case ESP8266_CMD_STOP_PLAYBACK: DEBUG_SERIAL.println("Stopping playback"); return true; break; //DONT read bytes after
      }
   }
   while(ESP8266_SERIAL.available() < 4){
      delayMicroseconds(1);
   }
   //Read bytes for plaback! 
   for(int i = 0; i < 4; i++){
        TransAngleUnion.bytes[i] = ESP8266_SERIAL.read();
        DEBUG_SERIAL.println(TransAngleUnion.bytes[i]);
   }
   
   while(ESP8266_SERIAL.available() < 4){
      delayMicroseconds(1);
   }   
   for(int i = 0; i < 4; i++){
        TransErrorUnion.bytes[i] = ESP8266_SERIAL.read();
        DEBUG_SERIAL.println(TransErrorUnion.bytes[i]);
   }
   
   while(ESP8266_SERIAL.available() < 4){ //If we do not wait enough we just receive 255 all the time which is -1!!!
      delayMicroseconds(1);
   }
   for(int i = 0; i < 4; i++){
        RotatErrorUnion.bytes[i] = ESP8266_SERIAL.read();
        DEBUG_SERIAL.println(RotatErrorUnion.bytes[i]);
   }
   
    DEBUG_SERIAL.println("Float errors");
    RX_trans_angle = TransAngleUnion.f;
    DEBUG_SERIAL.println(RX_trans_angle);
    RX_err_trans = TransErrorUnion.f;
    DEBUG_SERIAL.println(RX_err_trans);
    RX_err_rot = RotatErrorUnion.f;
    DEBUG_SERIAL.println(RX_err_rot);
    
    return true;
}