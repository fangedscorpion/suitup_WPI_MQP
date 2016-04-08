#include <pfodESP8266BufferedClient.h>
#include <pfodESP8266WiFi.h>

//#include <ESP8266WiFi.h> //The Arduino ESP8266 WiFi Core --> These guys are the real MVP

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
#define PLAYBACK 6
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
pfodESP8266BufferedClient bufferedClient; // http://www.forward.com.au/pfod/pfodParserLibraries/index.html

//////////////////////
// This section is for serial stuff
#define ESP8266_SERIAL Serial
#define DEBUG_SERIAL Serial

#define PRINT_DEBUGGING_MSGS // Comment this for no prints to the console screen for debugging

//Coming from the Teensy!
#define MSG_TO_ESP8266_ALIGN_BYTES 4
#define MSG_TO_ESP8266_DATA_BYTES 8 //For sending back to the ESP8266
#define MSG_TO_ESP8266_TOTAL_SIZE (MSG_TO_ESP8266_ALIGN_BYTES + MSG_TO_ESP8266_DATA_BYTES)
#define ESP8266_START_BYTE 254
#define ESP8266_MIN_CMD_BYTE 192 //Minimum number to be sent to complete the packet
#define ESP8266_CMD_NO_AXN ESP8266_MIN_CMD_BYTE
//Commands are defined as follows (anything above the minimum value up to 253)
#define ESP8266_CMD_MPU6050_DATA 206 //Sent if there is data following
#define ESP8266_CMD_MPU6050_DATA_LOW_BATT  207 //Send if low battery
#define ESP8266_CMD_VOICE_START 210
#define ESP8266_CMD_VOICE_START_LOW_BATT 211
#define ESP8266_CMD_VOICE_STOP 212
#define ESP8266_CMD_VOICE_STOP_LOW_BATT 213
#define ESP8266_CMD_VOICE_ACTION          216
#define ESP8266_CMD_VOICE_ACTION_LOW_BATT 217

#define CMD_SLOT 1
#define RECORDING_MSG_SIZE 11
#define MSG_TO_ESP8266_MSG_SIZE 8

char recordingMsg[RECORDING_MSG_SIZE] = {0x0A, BAND_POSITION_UPDATE, 0,0,0,0,0,0,0,0,'\n'};
uint8_t count = 0; //For delaying how many WiFi packets get sent

#define PLAYBACK_MSG_SIZE 11
char playbackMsg[PLAYBACK_MSG_SIZE] = {0x0A, BAND_POSITION_UPDATE, 0,0,0,0,0,0,0,0,'\n'};

//Voice control information to PC
#define VOICE_CONTROL_START 67 //bandmessage.h
#define VOICE_CONTROL_STOP 15 //bandmesssage.h
#define VOICE_CONTROL_CMD_BYTE 1
#define VOICE_CONTROL_DATA_BTYE 2
#define VOICE_CONTROL_LENGTH 4
char voiceControlMsg[VOICE_CONTROL_LENGTH] = {0x03, VOICE_CONTROL, VOICE_CONTROL_STOP, '\n'};  

#define FEEDBACK_MSG_ALIGN_BYTES 4
#define FEEDBACK_MSG_DATA_BYTES 12
#define FEEDBACK_MSG_SIZE (FEEDBACK_MSG_ALIGN_BYTES + FEEDBACK_MSG_DATA_BYTES)
#define ESP8266_CMD_CONTINUE_PLAYBACK 197

uint8_t feedbackToTeensyMsg[FEEDBACK_MSG_SIZE] = {ESP8266_START_BYTE, ESP8266_START_BYTE, ESP8266_START_BYTE, ESP8266_CMD_CONTINUE_PLAYBACK, 0,0,0,0,0,0,0,0,0,0,0,0}; 
uint8_t msgDataNonPosition[20];

//Circular buffer for serial data
int serialDataHeadPointer = 0; //always one ahead of the data
int serialDataTailPointer = 0; // always on the data 
#define BUFFER_SIZE 2048
static uint8_t sbuf[BUFFER_SIZE];

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

void readTeensySerialSendPkt(boolean printStuff){
  size_t len = ESP8266_SERIAL.available();
  if (len >= (MSG_TO_ESP8266_TOTAL_SIZE)) {
  
    if(printStuff){
      ESP8266_SERIAL.print(len);
    }
     
    if(serialDataHeadPointer + len >= BUFFER_SIZE){
        size_t writeLen = BUFFER_SIZE - serialDataHeadPointer; //figure out how long until the end of the array
        
        ESP8266_SERIAL.readBytes(&sbuf[serialDataHeadPointer], writeLen);
        if(printStuff){
            ESP8266_SERIAL.print("WR_WR1:");
            for(int i = serialDataHeadPointer; i < serialDataHeadPointer+writeLen; i++){
                  ESP8266_SERIAL.print(char(sbuf[i]));
            }
        }
        
        serialDataHeadPointer = 0; // Wrap around
  
        len -= writeLen; 
  
        if(len > 0){ //Read anything beyond our buffer
            ESP8266_SERIAL.readBytes(&sbuf[serialDataHeadPointer], len);
            
            if(printStuff){
                ESP8266_SERIAL.print("WR_WR2:");
                for(int i = serialDataHeadPointer; i < serialDataHeadPointer+len; i++){
                  ESP8266_SERIAL.print(char(sbuf[i]));
                }
            }
            
            
            serialDataHeadPointer += len;
        }
  
        if(printStuff){
          ESP8266_SERIAL.print("WR_");
          ESP8266_SERIAL.print(serialDataHeadPointer); 
          ESP8266_SERIAL.println(":"); 
        }
    }
      else{ //within bounds of buffer
        ESP8266_SERIAL.readBytes(&sbuf[serialDataHeadPointer], len);
        serialDataHeadPointer += len;
        if(printStuff){
          ESP8266_SERIAL.print("WR_WR:");
          for(int i = serialDataHeadPointer; i < serialDataHeadPointer+len; i++){
            ESP8266_SERIAL.print(char(sbuf[i]));
          }
          
            ESP8266_SERIAL.print("WR_");
            ESP8266_SERIAL.print(serialDataHeadPointer); 
            ESP8266_SERIAL.println(":"); 
        }
         
      }
    
    //Reading mode
    //Do a quick search backward to find latest data
    serialDataTailPointer = wrapCircularIndex(serialDataHeadPointer-MSG_TO_ESP8266_TOTAL_SIZE);
    if(printStuff){
        ESP8266_SERIAL.print("RD_");
        ESP8266_SERIAL.print(serialDataTailPointer);
        ESP8266_SERIAL.print(":");
        ESP8266_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer)]));
        ESP8266_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+1)]));
        ESP8266_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+2)]));
        ESP8266_SERIAL.print("END_RD");
      }

      while(sbuf[wrapCircularIndex(serialDataTailPointer)] != ESP8266_START_BYTE || sbuf[wrapCircularIndex(serialDataTailPointer+1)] != ESP8266_START_BYTE || sbuf[wrapCircularIndex(serialDataTailPointer+2)] != ESP8266_START_BYTE){
         serialDataTailPointer = wrapCircularIndex(serialDataTailPointer-1);
      }//This should always find data even if it's a bit old :/ but we shall see

      if(printStuff){
        ESP8266_SERIAL.print("RD_");
        ESP8266_SERIAL.print(serialDataTailPointer);
        ESP8266_SERIAL.print(":");
        ESP8266_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer)]));
        ESP8266_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+1)]));
        ESP8266_SERIAL.print(char(sbuf[wrapCircularIndex(serialDataTailPointer+2)]));
        ESP8266_SERIAL.print("END_RD");
      }

        char theCMD = sbuf[wrapCircularIndex(serialDataTailPointer+3)];
        if(printStuff){
          ESP8266_SERIAL.print("CMD:");
          ESP8266_SERIAL.print(char(theCMD));
        }
          serialDataTailPointer = wrapCircularIndex(serialDataTailPointer+4);


          uint8_t voiceCmd = false;
          
          switch(theCMD){ //Tell what type of cmd to add
              case ESP8266_CMD_MPU6050_DATA: 
                  recordingMsg[CMD_SLOT] = BAND_POSITION_UPDATE;
                  break;
              case ESP8266_CMD_MPU6050_DATA_LOW_BATT: 
                  recordingMsg[CMD_SLOT] = BAND_POSITION_UPDATE_LOW_BATT;
                  break;
              case ESP8266_CMD_VOICE_START:
                  voiceControlMsg[VOICE_CONTROL_CMD_BYTE] = VOICE_CONTROL;
                  voiceControlMsg[VOICE_CONTROL_DATA_BTYE] = VOICE_CONTROL_START;
                  voiceCmd = true;
              break;
              case ESP8266_CMD_VOICE_START_LOW_BATT:
                  voiceControlMsg[VOICE_CONTROL_CMD_BYTE] = VOICE_CONTROL_LOW_BATT;
                  voiceControlMsg[VOICE_CONTROL_DATA_BTYE] = VOICE_CONTROL_START;
                  voiceCmd = true;
              break;
              case ESP8266_CMD_VOICE_STOP:
                  voiceControlMsg[VOICE_CONTROL_CMD_BYTE] = VOICE_CONTROL;
                  voiceControlMsg[VOICE_CONTROL_DATA_BTYE] = VOICE_CONTROL_STOP;
                  voiceCmd = true;
              break;
              case ESP8266_CMD_VOICE_STOP_LOW_BATT:
                  voiceControlMsg[VOICE_CONTROL_CMD_BYTE] = VOICE_CONTROL_LOW_BATT;
                  voiceControlMsg[VOICE_CONTROL_DATA_BTYE] = VOICE_CONTROL_STOP;
                  voiceCmd = true;
              break;         
              case ESP8266_CMD_VOICE_ACTION:
                  voiceControlMsg[VOICE_CONTROL_CMD_BYTE] = VOICE_CONTROL;
                  voiceControlMsg[VOICE_CONTROL_DATA_BTYE] = ESP8266_CMD_VOICE_ACTION;
                  voiceCmd = true;
              break;
              case ESP8266_CMD_VOICE_ACTION_LOW_BATT:
                  voiceControlMsg[VOICE_CONTROL_CMD_BYTE] = VOICE_CONTROL_LOW_BATT;
                  voiceControlMsg[VOICE_CONTROL_DATA_BTYE] = ESP8266_CMD_VOICE_ACTION_LOW_BATT;
                  voiceCmd = true;
              break;  
              default:
                  
              break;
           }

           if(printStuff){
              ESP8266_SERIAL.write(0x09);
            }
            if(!voiceCmd){
              if(serialDataTailPointer + MSG_TO_ESP8266_DATA_BYTES >= BUFFER_SIZE){
                  size_t readLen = BUFFER_SIZE - serialDataTailPointer; //figure out how long until the end of the array
                  
                  memcpy( &recordingMsg[CMD_SLOT+1], &sbuf[serialDataTailPointer], readLen); //Only copy over the data bytes
                  serialDataTailPointer = 0; // Wrap around

                  int remainingBytes = MSG_TO_ESP8266_DATA_BYTES - readLen;
               
                  if(remainingBytes > 0){ //Copy anything beyond our buffer
                      memcpy( &recordingMsg[CMD_SLOT+1 + readLen], &sbuf[serialDataTailPointer], remainingBytes); //Only copy over the data bytes
                  }

              }
              else{ // Don't have to wrap around the buffer
                 memcpy( &recordingMsg[CMD_SLOT+1], &sbuf[serialDataTailPointer], MSG_TO_ESP8266_DATA_BYTES); //Only copy over the data bytes
              }
    
              if(printStuff){
                  ESP8266_SERIAL.print("RCD:");  //Write the packet to the PC
                  for(int i = 0; i < RECORDING_MSG_SIZE; i++){
                    ESP8266_SERIAL.write(recordingMsg[i]);  //Write the packet to the PC
                    
                  }
                  ESP8266_SERIAL.println("E_RCD");  //Write the packet to the PC
              }
            }

            
            //Actually send out WiFi packets for checking stuff
            if(state == RECORDING || state == PLAYBACK){ //Check that in proper state to send data!!!
              if(count == 50){
               bufferedClient.write((const uint8_t *)recordingMsg, RECORDING_MSG_SIZE);
               count = 0;
              }
              count++;
            }
            
            if(voiceCmd){ //Voice control packet 
              if(printStuff){
                
                ESP8266_SERIAL.write(voiceControlMsg, VOICE_CONTROL_LENGTH);  //Write the packet to the PC  
              }
              
              bufferedClient.write((const uint8_t *)voiceControlMsg, VOICE_CONTROL_LENGTH);
            }
  }
            
}
//////////////////////

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
              case COMPUTER_INITIATE_CONNECTION:  ESP8266_SERIAL.println("COMPUTER_INITIATE_CONNECTION"); break;
              case BAND_CONNECTING:  ESP8266_SERIAL.println("BAND_CONNECTING"); break;
              case COMPUTER_PING:  ESP8266_SERIAL.println("COMPUTER_PING"); break;
              case BAND_PING:  ESP8266_SERIAL.println("BAND_PING"); break;
              case BAND_POSITION_UPDATE:  ESP8266_SERIAL.println("BAND_POSITION_UPDATE"); break;
              case POSITION_ERROR:  ESP8266_SERIAL.println("POSITION_ERROR"); break;
              case START_RECORDING:  ESP8266_SERIAL.println("START_RECORDING"); break;
              case STOP_RECORDING:  ESP8266_SERIAL.println("STOP_RECORDING"); break;
              case START_PLAYBACK:  ESP8266_SERIAL.println("START_PLAYBACK"); break;
              case STOP_PLAYBACK:  ESP8266_SERIAL.println("STOP_PLAYBACK"); break;
              case VOICE_CONTROL:  ESP8266_SERIAL.println("VOICE_CONTROL"); break;
              case LOW_BATTERY_UPDATE: ESP8266_SERIAL.println("LOW_BATTERY_UPDATE"); break;
              default: ESP8266_SERIAL.println("Unrecognized format"); break;
            }
            
            if(msgLength > 2){ //Normal message
              ESP8266_SERIAL.print("Data: ");
                            
              for(int j = 2; j < (msgLength-1); j++){ //print data from msg
              
                if(msgType == POSITION_ERROR){//copy into the proper feedback array  
                    feedbackToTeensyMsg[FEEDBACK_MSG_ALIGN_BYTES + (j-2)] = msg[j];
                    ESP8266_SERIAL.print(char(feedbackToTeensyMsg[FEEDBACK_MSG_ALIGN_BYTES + (j-2)]));
                }
                else{
                  msgDataNonPosition[j-2] = msg[j];
                }
                
              }
              ESP8266_SERIAL.println();
            }
          }
  else{ //Broken packet thing
    ESP8266_SERIAL.println(String("Received MALFORMED packet of size: ") + msg.length());
  }
  return msgType;
}

void replyToPCInitiation(){ //Reply to initiation packet
   ESP8266_SERIAL.println("Sending response to COMPUTER_INITIATE_CONNECTION");
   uint8_t msg[3] = {2,BAND_CONNECTING,'\n'};
   
   bufferedClient.write((const uint8_t *)msg, 3); //Non blocking :D ?
}

void replyToPCPing(){
   ESP8266_SERIAL.println("Sending response to COMPUTER_PING");
   uint8_t msg[3] = {2,BAND_PING,'\n'};
   
    bufferedClient.write((const uint8_t *)msg, 3); //Non blocking :D ?
}

void GoToStateFindHost(){
    // Check if a client has connected
    hostPC = bandServerToPC.available();
    if (!hostPC) {
      return;
    }
    int counter = 0;
    // Wait until the client sends some data
    #ifdef PRINT_DEBUGGING_MSGS
      ESP8266_SERIAL.println("Host PC available");
    #endif
    
    while(!hostPC.available()){
      delay(1);
      counter++;
      if(counter > 1500){
        ESP8266_SERIAL.println("No host data");
        counter = 0;
      }
    }
    
    bufferedClient.connect(&hostPC); //Buffer this like in the SimplifiedUARTtoWiFiBridge library
    #ifdef PRINT_DEBUGGING_MSGS
      ESP8266_SERIAL.print("hostPC IP: ");
    #endif
    hostPCAddr = hostPC.remoteIP();
    
    #ifdef PRINT_DEBUGGING_MSGS
      ESP8266_SERIAL.println(hostPCAddr);
    #endif
    
    hasHostPCIP = true;

    #ifdef PRINT_DEBUGGING_MSGS
      ESP8266_SERIAL.println("Connected to PC!");
    #endif 
    
    state = IDLE_CONNECTED_TO_HOST;
}

void setup() {
  state = POWER_ON;
  ESP8266_SERIAL.begin(115200);
  delay(10);
  
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
      if(bufferedClient.connected()){
        
        // Read all the lines of the reply from server and print them to Serial
        if(bufferedClient.available()){
          String line = bufferedClient.readStringUntil('\n');
          msgTypeRXed = printRXPacket(line);
        }

        if(msgTypeRXed == specificPacket){
          return true;
        }
      }
      else{
        #ifdef PRINT_DEBUGGING_MSGS
          ESP8266_SERIAL.println("HOST PC DISCONNECTED!");
        #endif
        
        bufferedClient.stop(); // Close out old TCP stuff?
        
        state = FIND_HOST;
      }
      return false;
}

void listenForPackets(){
      char msgTypeRXed = NOTHING_NEW;
      if(bufferedClient.connected()){
        
        // Read all the lines of the reply from server and print them to Serial
        while(bufferedClient.available()){
          String line = bufferedClient.readStringUntil('\n');
          msgTypeRXed = printRXPacket(line);
        }

        switch(msgTypeRXed){
              case COMPUTER_INITIATE_CONNECTION:  replyToPCInitiation();  break;
              case BAND_CONNECTING:  break;
              case COMPUTER_PING:  replyToPCPing(); break;
              case BAND_PING:  break;
              case BAND_POSITION_UPDATE:  break;
              case POSITION_ERROR:  
              break;
              case START_RECORDING:  
                  state = RECORDING;
              break;
              case STOP_RECORDING:
                  state = IDLE_CONNECTED_TO_HOST;
              break;
              case START_PLAYBACK:  
                  state = PLAYBACK;
              break;
              case STOP_PLAYBACK: 
                  state = IDLE_CONNECTED_TO_HOST;
              break;
              case VOICE_CONTROL:  break;
              case LOW_BATTERY_UPDATE: break;
              case NOTHING_NEW: break;
              default: break;
        }
      }
      else{
        #ifdef PRINT_DEBUGGING_MSGS
          ESP8266_SERIAL.println("HOST PC DISCONNECTED!");
        #endif
        bufferedClient.stop(); // Close out old TCP stuff?
        state = FIND_HOST;
      }
}

int playBackPktCount = 0;
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
        readTeensySerialSendPkt(false);
    break;

    case RECORDING:
    {
        boolean gotStopRecording = listenForSpecificPacket(STOP_RECORDING);
        if(gotStopRecording){
          state = IDLE_CONNECTED_TO_HOST;
        }
        else{
          readTeensySerialSendPkt(false);
        }
    }
    break;

    case PLAYBACK:
    {
        boolean gotStopPlayback = listenForSpecificPacket(STOP_PLAYBACK);
        
        if(gotStopPlayback){
          state = IDLE_CONNECTED_TO_HOST;
        }
        else{
          readTeensySerialSendPkt(false); // Sends the packet 
        }
    }
    break;
  }
}

