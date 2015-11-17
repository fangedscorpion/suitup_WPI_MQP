#define SSID        "modelbased"
#define PASSWORD    "modelbased"
#define HOST_NAME   "192.168.1.10"
#define HOST_PORT   (80)

//#include <SoftwareSerial.h>
//
//SoftwareSerial mySerial(3, 2); // RX, TX

void setup() {
  Serial1.begin(115200);
  Serial.begin(115200);

  Serial1.write("AT\r\n");
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  
  }
  Serial.println("Waiting for restart");
  delay(2000);

  Serial1.write("AT+CIPMUX=1\r\n");
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }
//  
//    Serial1.write("AT+CIPSTART=4,\"UDP\",\"192.168.1.10\",80,1112,0\r\n");
//  if (Serial1.available()) {
//    Serial.write(Serial1.read());
//  }
}

void loop() {
  if (Serial1.available()) {
    while(Serial1.available() > 0){
      Serial.write(Serial1.read());
    }
    Serial.println();
  }
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }

}
