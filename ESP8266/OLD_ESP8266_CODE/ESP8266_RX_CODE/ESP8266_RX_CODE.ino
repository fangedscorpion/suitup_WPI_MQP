#define MPU_DATA_SIZE 14

int incomingByte;
boolean foundMPUDataStart = false;
char incomingDataMsg[MPU_DATA_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,'\r','\n'};    

void readTeensySerialData(){
  if (Serial.available() >= MPU_DATA_SIZE) {
    if(Serial.peek() != '$'){
        while(Serial.peek() != '$'){
          Serial.read(); //clear to start of packet
          if(Serial.peek() == -1){
            foundMPUDataStart = false;
            break; 
          }
          else if(Serial.peek() == '$'){
            incomingDataMsg[0] = Serial.read();
            if(Serial.peek() == 0x02){
              incomingDataMsg[1] = Serial.read();
              incomingDataMsg[2] = Serial.read(); //Probably reduce these in a loop to check values and avoid reading empty data --> avoid stace trace
              incomingDataMsg[3] = Serial.read();
              incomingDataMsg[4] = Serial.read();
              incomingDataMsg[5] = Serial.read();
              incomingDataMsg[6] = Serial.read();
              incomingDataMsg[7] = Serial.read();
              incomingDataMsg[8] = Serial.read();
              incomingDataMsg[9] = Serial.read();
              incomingDataMsg[10] = Serial.read();
              incomingDataMsg[11] = Serial.read();
              Serial.read();
              Serial.read();
              foundMPUDataStart = true;
            }
            else{ //Not start of msg
              foundMPUDataStart = false;
            }
          }
        }
    }
    else{
      incomingDataMsg[0] = Serial.read();
      if(Serial.peek() == 0x02){
              incomingDataMsg[1] = Serial.read();
              incomingDataMsg[2] = Serial.read(); 
              incomingDataMsg[3] = Serial.read();
              incomingDataMsg[4] = Serial.read();
              incomingDataMsg[5] = Serial.read();
              incomingDataMsg[6] = Serial.read();
              incomingDataMsg[7] = Serial.read();
              incomingDataMsg[8] = Serial.read();
              incomingDataMsg[9] = Serial.read();
              incomingDataMsg[10] = Serial.read();
              incomingDataMsg[11] = Serial.read();
              Serial.read();
              Serial.read();
              foundMPUDataStart = true;
      }
      else{
        foundMPUDataStart = false;
      }
    }
    if(foundMPUDataStart){
      Serial.write(incomingDataMsg,14);
      delayMicroseconds(100);
    }
    else{
      
    }
  }
}


void setup() {
	Serial.begin(9600);
}


void loop() {
	readTeensySerialData();
}

