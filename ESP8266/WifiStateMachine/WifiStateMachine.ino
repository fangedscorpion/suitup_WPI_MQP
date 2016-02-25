#include <ESP8266WiFi.h> //The Arduino ESP8266 WiFi Core --> These guys are the real MVP
#include <WiFiUDP.h>
#define DEBUG //Debugging information printed to Serial. This clogs up the Teensy port
#include "WiFiMsgTypes.h" //Lists all the message types for our communication link

////////////////////////////////////
// States for the ESP8266 State machine
// 
#define POWER_ON 1
#define CONNECTION 2
#define IDLE_CONNECTED_TO_AP 3
#define LOST_CONNECTION 4
#define RECORDING 5
#define FEEDBACK 6
#define ERROR_STATE -1
#define CONNECTING_TO_HOST 8
#define IDLE_CONNECTED_TO_HOST 9
#define FIND_HOST 10

int state; //Holds the state
///////////////////////////////////


///////////////////////////////////
// Error conditions -- hopefully only a few
#define TIMEOUT_ON_CONN 1
///////////////////////////////////

void GoToErrorState(int error_num){ // Does anything involving errors
  // Flip Error pin to Teensy
  //TODO

  WiFi.disconnect();
  switch(error_num){
    case TIMEOUT_ON_CONN:
      WiFi.disconnect();
      Serial.println("Could not connect to AP! Test Wifi Router");
      delay(3000);
      Serial.println("Retrying router");
    break;

    default:
      Serial.println("An error occurred :(");
    break;
  }
}

///////////////////////////////////
// Time out information for WiFi Connection
#define TIMEOUT_RETRIES 30
int timeoutCounter = 0;
///////////////////////////////////
///////////////////////////////////
//WiFi information
const char* ssid     = "modelbased";
const char* password = "modelbased";

char value = 0; //Test value to send
boolean conn = false; //If connected to WiFi computer host
IPAddress hostPCAddr; //determined from incoming TCP connection

#define TCP_PORT 12941 //TCP connection to/from PC

WiFiServer bandServerToPC(TCP_PORT);
boolean hasHostPCIP = false;
WiFiClient hostPC;

#define POSITION_DATA_BYTES 12
char positionalFBData[POSITION_DATA_BYTES];
char msgDataNonPosition[NON_POSITION_DATA_BYTES_MAX];

#define MPU_DATA_SIZE 14

int incomingByte;
boolean foundMPUDataStart = false;
char incomingDataMsg[MPU_DATA_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,'\r','\n'};    
char recordingMsg[RECORDING_MSG_SIZE] = {0x0A, BAND_POSITION_UPDATE, 0,0,0,0,0,0,0,0,'\n'};

////////////////////////////////////
//Returns true if could connect
//FALSE if connection failed
boolean GoToStateConnection(){ //Connects to WiFi
  timeoutCounter = 0;
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    if(timeoutCounter >= TIMEOUT_RETRIES){
      Serial.print("FAILED!!! after ");
      Serial.print(timeoutCounter + 1);
      Serial.println(" tries");
      return false;
    }
    else{
      delay(250);     
      Serial.print(".");
      timeoutCounter++; 
    }
  }
  
  Serial.print("Connected after ");
  Serial.print(timeoutCounter);
  Serial.println(" tries");

  Serial.println("");
  delay(100);
  Serial.println("WiFi connected");  
  Serial.println("BAND IP ADDR: ");
  Serial.println(WiFi.localIP());

  bandServerToPC.begin();
  Serial.println("Listener started for PC");

  return true;
}

char printRXPacket(String msg){
  char msgType = char(BROKEN_PACKET);
  if(msg.length() >= 2){  //Normal message
            int msgLength = int(msg[0]);
            Serial.println(String("Received: ") + msgLength + " bytes");
            
            Serial.print("Message type: ");
            msgType = msg[1];
            switch(msgType){ //Comparing to the values from enum in header
              case COMPUTER_INITIATE_CONNECTION:  Serial.println("COMPUTER_INITIATE_CONNECTION"); break;
              case BAND_CONNECTING:  Serial.println("BAND_CONNECTING"); break;
              case COMPUTER_PING:  Serial.println("COMPUTER_PING"); break;
              case BAND_PING:  Serial.println("BAND_PING"); break;
              case BAND_POSITION_UPDATE:  Serial.println("BAND_POSITION_UPDATE"); break;
              case POSITION_ERROR:  Serial.println("POSITION_ERROR"); break;
              case START_RECORDING:  Serial.println("START_RECORDING"); break;
              case STOP_RECORDING:  Serial.println("STOP_RECORDING"); break;
              case START_PLAYBACK:  Serial.println("START_PLAYBACK"); break;
              case STOP_PLAYBACK:  Serial.println("STOP_PLAYBACK"); break;
              case VOICE_CONTROL:  Serial.println("VOICE_CONTROL"); break;
              case LOW_BATTERY_UPDATE: Serial.println("LOW_BATTERY_UPDATE"); break;
              default: Serial.println("Unrecognized format"); break;
            }
            
            if(msgLength > 2){ //Normal message
              Serial.print("Data: ");
              
              memset(msgDataNonPosition, 0, sizeof(msgDataNonPosition)); //clear out non-position array
              
              for(int j = 2; j < (msgLength-1); j++){ //print data from msg
                Serial.print(msg[j]);
                
                if(msgType == POSITION_ERROR){//copy into the proper feedback array  
                    positionalFBData[j-2] = msg[j]; 
                }
                else{
                  msgDataNonPosition[j-2] = msg[j];
                }
                
              }
              Serial.println(msg[msgLength-1]);  
            }
          }
  else{ //Broken packet thing
    Serial.println(String("Received MALFORMED packet of size: ") + msg.length());
  }
  return msgType;
}

void replyToPCInitiation(){ //Reply to initiation packet
   Serial.println("Sending response to COMPUTER_INITIATE_CONNECTION");
   char msg[3] = {2,1,'\n'};
   hostPC.flush(); // This might take a while
   hostPC.print(msg);
}

void replyToPCPing(){
   Serial.println("Sending response to COMPUTER_PING");
   char msg[3] = {2,1,'\n'};
   hostPC.flush(); // This might take a while
   hostPC.print(msg);
}

void replyToRXPosError(){
   Serial.println("Sending response to COMPUTER_PING");
   char msg[3] = {2,1,'\n'};
   hostPC.flush(); // This might take a while
   hostPC.print(msg);
}

void giveMotorsFeedback(){
  //Send the positional data to the Teensy
//  
//  for(int j = 0; j < POSITION_DATA_BYTES; j++){ //print data from msg
//     Serial.print(positionalFBData[j]);               
//   }
}

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
      delayMicroseconds(200);
    }
    else{
      
    }
  }
}

void sendDataToComputer(){
  for(int i = 2; i < 10; i ++){
    recordingMsg[i] = incomingDataMsg[i]; 
  }
  
  hostPC.print(recordingMsg;);
}

void GoToStateFindHost(){
    // Check if a client has connected
    hostPC = bandServerToPC.available();
    if (!hostPC) {
      return;
    }
    int counter = 0;
    // Wait until the client sends some data
    Serial.println("Host PC available");
    while(!hostPC.available()){
      delay(1);
      counter++;
      if(counter > 1500){
        Serial.println("No host data");
        counter = 0;
      }
    }
    Serial.print("hostPC IP: ");
    hostPCAddr = hostPC.remoteIP();
    Serial.println(hostPCAddr);
    hasHostPCIP = true;
    Serial.println("Connected to PC!");
    state = IDLE_CONNECTED_TO_HOST;
}

void setup() {
  state = POWER_ON; 
  Serial.begin(9600);
  delay(10);
  Serial.println("Hi!");
  
  state = CONNECTION;
  boolean goodConnect = GoToStateConnection();
  while(!goodConnect){
    state = ERROR_STATE;
    GoToErrorState(TIMEOUT_ON_CONN);
    goodConnect = GoToStateConnection();
  }

  state = IDLE_CONNECTED_TO_AP;
}

boolean listenForSpecificPacket(char specificPacket){
      char msgTypeRXed = NOTHING_NEW;
      if(hostPC.connected()){
        
        // Read all the lines of the reply from server and print them to Serial
        while(hostPC.available()){
          String line = hostPC.readStringUntil('\n');
          msgTypeRXed = printRXPacket(line);
        }

        if(msgTypeRXed == specificPacket){
          return true;
        }
      }
      else{
        Serial.println("HOST PC DISCONNECTED!");
        state = FIND_HOST;
      }
      return false;
}

void listenForPackets(){
      char msgTypeRXed = NOTHING_NEW;
      if(hostPC.connected()){
        
        // Read all the lines of the reply from server and print them to Serial
        while(hostPC.available()){
          String line = hostPC.readStringUntil('\n');
          msgTypeRXed = printRXPacket(line);
        }

        switch(msgTypeRXed){
              case COMPUTER_INITIATE_CONNECTION:  replyToPCInitiation();  break;
              case BAND_CONNECTING:  Serial.println("Doing nothing for BAND_CONNECTING"); break;
              case COMPUTER_PING:  replyToPCPing(); break;
              case BAND_PING:  Serial.println("Doing nothing for BAND_PING"); break;
              case BAND_POSITION_UPDATE:  Serial.println("Doing nothing for BAND_POSITION_UPDATE"); break;
              case POSITION_ERROR:  
                    giveMotorsFeedback();
                    replyToRXPosError();
              break;
              case START_RECORDING:  
                    state = RECORDING;
              break;
              case STOP_RECORDING:
                  state = IDLE_CONNECTED_TO_HOST;
                    //tellTeensyStopRecording();
              break;
              case START_PLAYBACK:  
                    //tellTeensyStartPlayback();
              break;
              case STOP_PLAYBACK: 
                    //tellTeensyStopPlayback();
              break;
              case VOICE_CONTROL:  Serial.println("Doing nothing for VOICE_CONTROL"); break;
              case LOW_BATTERY_UPDATE: Serial.println("Doing nothing for LOW_BATTERY_UPDATE"); break;
              case NOTHING_NEW: break;
              default: Serial.println("Unrecognized format: "); break;
        }
      }
      else{
        Serial.println("HOST PC DISCONNECTED!");
        state = FIND_HOST;
      }
}



void loop() {
  switch(state){
    case IDLE_CONNECTED_TO_AP:
        state = FIND_HOST;
    break;
    
    case FIND_HOST:
        GoToStateFindHost();
    break;

    case IDLE_CONNECTED_TO_HOST:
        listenForPackets();
    break;

    case RECORDING:
        boolean gotStopRecording = listenForSpecificPacket(STOP_RECORDING);
        if(gotStopRecording){
          state = IDLE_CONNECTED_TO_HOST;
        }
        else{
          readTeensySerialData();
          sendDataToComputer();
        }
    break;
  }
}

