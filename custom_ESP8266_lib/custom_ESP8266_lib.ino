#define SSID        "modelbased"
#define PASSWORD    "modelbased"
#define HOST_NAME   "192.168.1.10"
#define HOST_PORT   (80)

#include <SoftwareSerial.h>

SoftwareSerial mySerial(3, 2); // RX, TX

void setup() {
  mySerial.begin(115200);
  Serial.begin(115200);

  mySerial.write("AT\r\n");
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  
  }
  Serial.println("Waiting for restart");
  delay(2000);

  mySerial.write("AT+CIPMUX=1\r\n");
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
  
    mySerial.write("AT+CIPSTART=4,\"UDP\",\"192.168.1.10\",80,1112,0\r\n");
  if (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

void loop() {
  if (mySerial.available()) {
    while(mySerial.available() > 0){
      Serial.write(mySerial.read());
    }
    Serial.println();
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }

}
