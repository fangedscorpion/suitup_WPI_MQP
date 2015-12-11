/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "modelbased";
const char* password = "modelbased";

const char* host = "192.168.1.11";
const char* streamId   = "HELLO";

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
}

char value = 0;
char conn = 0;
WiFiClient client;

void loop() {
  delay(11);
  ++value % 100;
 
  if(conn == 0){
    Serial.print("connecting to ");
    Serial.println(host);
   
    // Use WiFiClient class to create TCP connections
    
    const int httpPort = 81;
    if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      conn = 0;
      return;
    }
    else{
      conn = 1;
    }
  }
  
  // This will send the request to the server
  client.print(String("_mpudta_") + value + String("!"));
//  client.print(String("GET ") + streamId + " HTTP/1.1\r\n" +
//               "Host: " + host + "\r\n" + "Connection: Keep-Alive\r\n\r\n");
  delay(10);
  
  // Read all the lines of the reply from server and print them to Serial
  if(client.available()>=10){
    char buf[10];
    client.readBytes(buf,10);
    Serial.print(buf);
  }
//  Serial.println();
  client.flush();
}

