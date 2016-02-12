#include <ESP8266WiFi.h>
#include "WiFiMsgTypes.h" //Lists all the message types for our communication link

const char* ssid     = "modelbased";
const char* password = "modelbased";

IPAddress hostPCAddr; //determined from incoming TCP connection

#define TCP_PORT 12941 //TCP connection to/from PC

WiFiServer bandServerToPC(TCP_PORT);
boolean hasHostPCIP = false;
WiFiClient hostPC;

char positionalFBData[POSITION_DATA_BYTES];
char msgDataNonPosition[NON_POSITION_DATA_BYTES_MAX];

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  bandServerToPC.begin();
  Serial.println("Listener started for PC");
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
  
  for(int j = 0; j < POSITION_DATA_BYTES; j++){ //print data from msg
     Serial.print(positionalFBData[j]);               
   }
}

void loop() {
  if(!hasHostPCIP){ //Wait for PC to start the connection
        // Check if a client has connected
        hostPC = bandServerToPC.available();
        if (!hostPC) {
          return;
        }
        
        // Wait until the client sends some data
        Serial.println("Host PC avialable");
        while(!hostPC.available()){
          delay(1);
        }
        Serial.print("hostPC IP: ");
        hostPCAddr = hostPC.remoteIP();
        Serial.println(hostPCAddr);
        hasHostPCIP = true;
        Serial.println("Connected to PC!");
  }
  if(hasHostPCIP){
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
                    tellTeensyStartRecording();
                    sendGenericReply();
              break;
              case STOP_RECORDING:
                    tellTeensyStopRecording();
                    sendGenericReply();
              break;
              case START_PLAYBACK:  
                    tellTeensyStartPlayback();
                    sendGenericReply();
              break;
              case STOP_PLAYBACK: 
                    tellTeensyStopPlayback();
                    sendGenericReply();
              break;
              case VOICE_CONTROL:  Serial.println("Doing nothing for VOICE_CONTROL"); break;
              case LOW_BATTERY_UPDATE: Serial.println("Doing nothing for LOW_BATTERY_UPDATE"); break;
              default: Serial.println("Unrecognized format: "); break;
        }
        
        delay(10); //Try changing this to be lower
      }
      else{
        Serial.println("HOST PC DISCONNECTED!");
        hasHostPCIP = false;
      }
  }
}

