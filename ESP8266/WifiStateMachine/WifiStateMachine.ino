#include <ESP8266WiFi.h> //The Arduino ESP8266 WiFi Core --> These guys are the real MVP
#include <WiFiUDP.h>
#define DEBUG //Debugging information printed to Serial. This clogs up the Teensy port

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


void GoToStatePowerOn(){ //Anything to be done while powering up --> Maybe talk to Teensy?
 Serial.begin(115200);
 Serial.println("Hello world!");
}

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

const char* host_preprogrammed = "192.168.1.11"; 
const char* streamId   = "WAGBND";
#define UDP_BROADCAST_PORT 2016 //Used if a BAND doesn't know the HOST IP
#define UDP_BROADCAST_SIZE 2

char value = 0; //Test value to send
boolean conn = false; //If connected to WiFi computer host
WiFiClient client; 
WiFiUDP hostFinder;
////////////////////////////////////
//Returns true if could connect
//FALSE if connection failed
boolean GoToStateConnection(){ //Connects to WiFi
  timeoutCounter = 0;
  delay(10);
  // We start by connecting to a WiFi network
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
  #ifdef DEBUG
    Serial.println("DEBUG MODE");
  #endif
  return true;
}

void GoToStateFindHost(){ //Info from: https://github.com/PaulStoffregen/Time/blob/master/examples/TimeNTP_ESP8266WiFi/TimeNTP_ESP8266WiFi.ino
  hostFinder.begin(UDP_BROADCAST_PORT); //For any UDP connection info
  #ifdef DEBUG
      Serial.println("Sending ID msg for host");
      hostFinder.beginPacket("255.255.255.255", UDP_BROADCAST_PORT); //Send out an ID msg
      byte packetBuffer[UDP_BROADCAST_SIZE];
      packetBuffer[0] = byte('H');
      packetBuffer[1] = byte('I');

      int j = 0;
      while(j < 1000){
          hostFinder.write(packetBuffer, UDP_BROADCAST_SIZE);
          int success = hostFinder.endPacket();
          Serial.print("Successfully sent packet: ");
          Serial.println(success);

          delay(100);
          int noBytes = hostFinder.parsePacket();
          String received_command = "";
          
          if ( noBytes ) {//See http://www.esp8266.com/viewtopic.php?f=29&t=2222
            Serial.print(millis() / 1000);
            Serial.print(":Packet of ");
            Serial.print(noBytes);
            Serial.print(" received from ");
            Serial.print(hostFinder.remoteIP());
            Serial.print(":");
            Serial.println(hostFinder.remotePort());
            // We've received a packet, read the data from it
            hostFinder.read(packetBuffer,noBytes); // read the packet into the buffer
        
            // display the packet contents in HEX
            for (int i=1;i<=noBytes;i++)
            {
              Serial.print(packetBuffer[i-1],HEX);
              received_command = received_command + char(packetBuffer[i - 1]);
              if (i % 32 == 0)
              {
                Serial.println();
              }
              else Serial.print(' ');
            } // end for
          }
          Serial.println("End of Loop");
         j++;
      }
      
      //SOMETHING HERE FROM THIS PAGE: https://github.com/esp8266/Arduino/blob/master/libraries/ESP8266WiFi/src/WiFiUdp.h
  #endif

      

  
//      Serial.print("connecting to ");
//        Serial.println(host);
//       
//        // Use WiFiClient class to create TCP connections
//        
//        const int httpPort = 81;
//        if (!client.connect(host, httpPort)) {
//          Serial.println("connection failed");
//          conn = 0;
//          return;
//        }
//        else{
//          conn = 1;
//        }
   hostFinder.stop(); //End UDP stuff
}

void setup() {
  state = POWER_ON; 
  GoToStatePowerOn(); 
  
  state = CONNECTION;
  boolean goodConnect = GoToStateConnection();
  while(!goodConnect){
    state = ERROR_STATE;
    GoToErrorState(TIMEOUT_ON_CONN);
    goodConnect = GoToStateConnection();
  }

  state = IDLE_CONNECTED_TO_AP;
}



void loop() {
  switch(state){
    case IDLE_CONNECTED_TO_AP:
        state = FIND_HOST;
    break;
    
    case FIND_HOST:
        GoToStateFindHost();
    break;
  }
//  #ifndef DEBUG
//      delay(11);
//      ++value % 100;
//     
//      if(conn == 0){
//        Serial.print("connecting to ");
//        Serial.println(host);
//       
//        // Use WiFiClient class to create TCP connections
//        
//        const int httpPort = 81;
//        if (!client.connect(host, httpPort)) {
//          Serial.println("connection failed");
//          conn = 0;
//          return;
//        }
//        else{
//          conn = 1;
//        }
//      }
//      
//      // This will send the request to the server
//      client.print(String("_mpudta_") + value + String("!"));
//    //  client.print(String("GET ") + streamId + " HTTP/1.1\r\n" +
//    //               "Host: " + host + "\r\n" + "Connection: Keep-Alive\r\n\r\n");
//      delay(10);
//      
//      // Read all the lines of the reply from server and print them to Serial
//      if(client.available()>=10){
//        char buf[10];
//        client.readBytes(buf,10);
//        Serial.print(buf);
//      }
//    //  Serial.println();
//      client.flush();
//  #else
//      Serial.println("HELLO WORLD!");
//      delay(500);
//  #endif
}

