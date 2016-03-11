#define MSG_TO_ESP8266_ALIGN_BYTES 4
#define MSG_TO_ESP8266_MSG_SIZE 8 //For sending back to the ESP8266
#define MSG_TO_ESP8266_TOTAL_SIZE (MSG_TO_ESP8266_ALIGN_BYTES + MSG_TO_ESP8266_MSG_SIZE)

#define ESP8266_START_BYTE 253

#define ESP8266_SERIAL Serial
#define DEBUG_SERIAL Serial
//Extra things to hold spots for enums when not being used for checking
#define BROKEN_PACKET 255
#define NOTHING_NEW 254

#define POSITION_DATA_BYTES 12
#define NON_POSITION_DATA_BYTES_MAX 4
#define RECORDING_MSG_SIZE 11

#define ESP8266_MIN_CMD_BYTE 192 //Minimum number to be sent to complete the packet
#define ESP8266_CMD_NO_AXN ESP8266_MIN_CMD_BYTE
//Commands are defined as follows (anything above the minimum value up to 253)
#define ESP8266_CMD_MPU6050_NO_DATA 205 //Sent to the ESP8266 from the teensy if there is no data
#define ESP8266_CMD_MPU6050_DATA 206 //Sent if there is data following
#define ESP8266_CMD_MPU6050_DATA_LOW_BATT  207 //Send if low battery
#define ESP8266_CMD_MPU6050_NO_DATA_LOW_BATT  208 //Send if low battery
#define COULD_NOT_ALIGN_START_BYTE 0x00


boolean lowBattery = false;

char mpu6050Data[MSG_TO_ESP8266_MSG_SIZE] = {0,0,0,0,0,0,0,0};

#define POSITION_DATA_BYTES 12
char positionalFBData[POSITION_DATA_BYTES];
char msgDataNonPosition[NON_POSITION_DATA_BYTES_MAX];

#define MPU_DATA_SIZE 14

int incomingByte;
boolean foundMPUDataStart = false;
char incomingDataMsg[MPU_DATA_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,'\r','\n'};
#define RECORDING_MSG_TYPE_POS 1    
char recordingMsg[RECORDING_MSG_SIZE] = {0x0A, 14, 0,0,0,0,0,0,0,0,'\n'};


//#include <ESP8266WiFi.h>
//ADC_MODE(ADC_VCC);
//const char* ssid     = "MySSID";
//const char* password = "MyPassword";
//
//const char* host = "192.168.178.1";
//const char* streamId   = "....................";
//const char* privateKey = "....................";
//unsigned char reconnectCount = 0;
//
//WiFiClient client;
//
//void setup() {
//  Serial.begin(115200);
//  delay(10);
//  float batVolt=ESP.getVcc() / 1000.0;
//  Serial.println();
//  Serial.println();
//  Serial.print("Battery: ");
//  Serial.print(batVolt);
//  Serial.println("V");
//  // We start by connecting to a WiFi network
//
//  Serial.println();
//  Serial.print("Connecting to ");
//  Serial.println(ssid);
//
//  WiFi.begin(ssid, password);
//
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    if (reconnectCount >= 60) {
//      ESP.restart();
//    }
//    Serial.print(".");
//    reconnectCount++;
//  }
//  reconnectCount = 0;
//
//  Serial.println("");
//  Serial.println("WiFi connected");
//  Serial.println("IP address: ");
//  Serial.println(WiFi.localIP());
//
//  connectToServer();
//  client.println("ESP8266 Connected");
//}
//
//
//void loop() {
//  if (Serial.available()) {
//    size_t len = Serial.available();
//    uint8_t * sbuf = (uint8_t *)malloc(len);
//    Serial.readBytes(sbuf, len);
//    if (client.connected()) {
//      client.write((uint8_t *)sbuf, len);
//      yield();
//    } else {
//      delay(500);
//      if (reconnectCount >= 60) {
//        ESP.restart();
//      }
//      Serial.println("Connection lost!");
//      Serial.println("Try to reconnect...");
//      connectToServer();
//      reconnectCount++;
//    }
//    free(sbuf);
//  }
//
//  if (client.available()) {
//    char c = client.read();
//    Serial.print(c);
//  }
//}



char synchronizeSerialBeginning(){
  char foundCMD = COULD_NOT_ALIGN_START_BYTE;

  while(ESP8266_SERIAL.peek() != ESP8266_START_BYTE && ESP8266_SERIAL.peek() != -1){
      DEBUG_SERIAL.print(ESP8266_SERIAL.read()); //Throw out character
      DEBUG_SERIAL.print(" "); //Throw out character
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
int readFromTeensy(){ 
    
    while(ESP8266_SERIAL.available() < MSG_TO_ESP8266_TOTAL_SIZE){
      delayMicroseconds(1);
      //delay(1);
    }
    
   char foundBeginningCMD = synchronizeSerialBeginning();
   if(foundBeginningCMD == COULD_NOT_ALIGN_START_BYTE){
      DEBUG_SERIAL.println("Could not find beginning");
      return false;
   }
   else { //Found the beginning! :D 
      switch(foundBeginningCMD){
        case ESP8266_CMD_NO_AXN: DEBUG_SERIAL.println("NO ACTION WAS FOUND IN readFromTeensy() FUNC"); return true; break;
        case ESP8266_CMD_MPU6050_NO_DATA: DEBUG_SERIAL.println("No data CMD from Teensy"); lowBattery = false; return true; break;
        case ESP8266_CMD_MPU6050_DATA: DEBUG_SERIAL.println("Data"); lowBattery = false; break;
        case ESP8266_CMD_MPU6050_DATA_LOW_BATT: DEBUG_SERIAL.println("Data & Low Batt"); lowBattery = true; break;
        case ESP8266_CMD_MPU6050_NO_DATA_LOW_BATT: DEBUG_SERIAL.println("No data & Low Batt"); lowBattery = true; return true; break;
        default: DEBUG_SERIAL.println("Unknown CMD"); return true; break;
      }
   }
   //Read the data! 
   while(ESP8266_SERIAL.available() < MSG_TO_ESP8266_MSG_SIZE){
      delayMicroseconds(1);
      //delay(1);
   }
   //Read bytes for plaback! 
   DEBUG_SERIAL.print("Data: ");
   for(int i = 0; i < MSG_TO_ESP8266_MSG_SIZE; i++){
      mpu6050Data[i] = ESP8266_SERIAL.read();
      recordingMsg[i+2] = mpu6050Data[i];
      DEBUG_SERIAL.print(char(mpu6050Data[i]));
   }
    DEBUG_SERIAL.println();
    
    return true;
}


void setup() {
  Serial.begin(115200);
  
}

static const size_t bufferSize = 128;
static uint8_t sbuf[bufferSize];

void loop() {
//  boolean tot = readFromTeensy();
//  delay(10);

  if (Serial.available() > (MSG_TO_ESP8266_TOTAL_SIZE-1)) {
    size_t len = Serial.available();
    Serial.print(len);
    while (len > 0) { // size_t is an unsigned type so >0 is actually redundent
      size_t will_copy = (len < bufferSize) ? len : bufferSize;
      Serial.readBytes(sbuf, will_copy);
      if(sbuf[0] == ESP8266_START_BYTE && sbuf[1] == ESP8266_START_BYTE && sbuf[2] == ESP8266_START_BYTE){
        
        Serial.print("CMD:");
        Serial.print(char(sbuf[MSG_TO_ESP8266_ALIGN_BYTES-1]));
        Serial.write(0x09);
        
        for(int i = MSG_TO_ESP8266_ALIGN_BYTES; i < MSG_TO_ESP8266_TOTAL_SIZE; i++){
          Serial.write(sbuf[i]);  
        }
        
        delay(0); // yield
      }
      len -= will_copy;
    }
    Serial.write(0x0A);
  }
}


