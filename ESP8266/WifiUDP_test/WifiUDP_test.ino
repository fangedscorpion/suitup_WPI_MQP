#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

char ssid[] = "modelbased";  //  your network SSID (name)
char pword[] = "modelbased";       // your network password

#define UDP_BAND_PORT 12941 // local port to listen for UDP packets --> Unique to each band
#define UDP_SERVER_PORT 14273 //Server port on the computer --> common among all

const int REGISTER_PKT_SIZE = 2; // This is a simple 'HI' msg content to have it register on the network

byte packetBuffer[REGISTER_PKT_SIZE]; //buffer to hold incoming and outgoing packets

// A UDP instance to let us send and receive packets over UDP
WiFiUDP udp;

boolean no_host = true; // Used to tell if the host has been received or not
IPAddress hostAddr; // Holds the host address in the future after it has been found

void setup()
{
  Serial.begin(115200);

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pword);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");
  
  Serial.println("WiFi connected");
  Serial.println("Band IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  udp.begin(UDP_BAND_PORT); // Start listening on this port
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
  no_host = true;
}

char pktNum;
void loop()
{
  if(no_host){
      Serial.println("sending UDP packet...");
      // set all bytes in the buffer to 0
      memset(packetBuffer, 0, REGISTER_PKT_SIZE);
      
      packetBuffer[0] = byte('H');
      packetBuffer[1] = byte('I');
    
      udp.beginPacket("192.168.1.255", UDP_SERVER_PORT); //Broadcast to the computer port
      udp.write(packetBuffer, REGISTER_PKT_SIZE);
      udp.endPacket();
      
      // wait to see if a reply is available
      delay(10);
      
      int cb = udp.parsePacket();
      if (!cb) {
        Serial.println("no packet yet");
      }
      else {
        Serial.print("packet received, length=");
        Serial.print(cb);
        Serial.print(" HOST IP: ");
        hostAddr = udp.remoteIP();
        Serial.println(hostAddr);
        no_host = false;
        // We've received a packet, read the data from it
        udp.read(packetBuffer, REGISTER_PKT_SIZE); // read the packet into the buffer
      }
      // wait before asking for the time again
      delay(200);
  }
  else{
      pktNum = (++pktNum)% 10;
      Serial.print("connecting to ");
      Serial.println(hostAddr);
      
      // Use WiFiClient class to create TCP connections
      WiFiClient client;
      const int httpPort = 1995;
      if (!client.connect(hostAddr, httpPort)) {
        Serial.println("connection failed");
        return;
      }
      
      // This will send the request to the server
      client.print(String("HI ") + pktNum + " X");
      delay(20);
      
      // Read all the lines of the reply from server and print them to Serial
      while(client.available()){
        String line = client.readStringUntil('\r');
        Serial.print(line);
      }
      
      Serial.println();
      Serial.println("closing connection");
  }
}
