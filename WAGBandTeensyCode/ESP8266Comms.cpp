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

void ESP8266Comms::copyMPU6050DataIntoMsg(uint8_t* teapotPkt){
    for(int i = MSG_TO_ESP8266_ALIGN_BYTES; i < MSG_TO_ESP8266_TOTAL_SIZE; i++){
        this->msgToESP8266[i] = teapotPkt[i-2];
    }
}

void ESP8266Comms::setCommand(char cmd){ //Try to enforce that commands should be between MSGESP8266_MIN_CMD_BYTE and 253
  if(cmd >= ESP8266_MIN_CMD_BYTE && cmd <= 253){
      this->msgToESP8266[MSG_TO_ESP8266_ALIGN_BYTES-1] = cmd; 
  }
  else{
      this->msgToESP8266[MSG_TO_ESP8266_ALIGN_BYTES-1] = ESP8266_CMD_MPU6050_NO_DATA; 
  }  
}


//This draws from the ESP8266 msg
void ESP8266Comms::sendMsgToESP8266(char cmd, uint8_t* teaPkt){
    this->setCommand(cmd); //Set the proper cmd in our pkt

    if(cmd == ESP8266_CMD_MPU6050_DATA || cmd == ESP8266_CMD_MPU6050_DATA_LOW_BATT){
      
      //this->copyMPU6050DataIntoMsg(teaPkt);

      for(int i = 0; i < MSG_TO_ESP8266_ALIGN_BYTES; i++){

          ESP8266_SERIAL.print(char(this->msgToESP8266[i]));

      }
      for(int i = 0; i < MSG_TO_ESP8266_MSG_SIZE; i++){ 

          ESP8266_SERIAL.print(char(teaPkt[i+2])); //Data starts at 2 in the packet

      }

    }
    else{

      for(int i = 0; i < MSG_TO_ESP8266_ALIGN_BYTES; i++){
          ESP8266_SERIAL.print(char(this->msgToESP8266[i]));
      }   

    }

}

//This draws from the ESP8266 msg which should be prefilled from the extractMPU6050Vals(uint8_t* espMsg)
void ESP8266Comms::sendMsgToESP8266(char cmd){
    this->setCommand(cmd); //Set the proper cmd in our pkt

    if(cmd == ESP8266_CMD_MPU6050_DATA || cmd == ESP8266_CMD_MPU6050_DATA_LOW_BATT){
      
      for(int i = 0; i < MSG_TO_ESP8266_TOTAL_SIZE; i++){

          ESP8266_SERIAL.write(this->msgToESP8266[i]);
          DEBUG_SERIAL.print(char(this->msgToESP8266[i]));
      }
    }
    else{

      for(int i = 0; i < MSG_TO_ESP8266_ALIGN_BYTES; i++){
          ESP8266_SERIAL.print(char(this->msgToESP8266[i]));
      }   

    }

}  