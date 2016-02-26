//This file is used to send test motor data to a WAGBand Teensy over the ESP8266 socket

void setup() {
 Serial3.begin(9600);
}

//Sending -0.5, 0.75, -1
//char TXpkt[16] = {254, 254, 254, 197, 0,0,0,0xBF,0,0,0x40,0x3F,0,0,0x80,0xBF}; 

//Sending pi/6, 0, 1
//char TXpkt[16] = {254, 254, 254, 197, 0x92, 0x0A, 0x06, 0x3F, 0,0,0,0, 0x00, 0x00, 0x80, 0x3F}; 

//Sending pi/6, 0.5, 0.5
//char TXpkt[16] = {254, 254, 254, 197, 0x92, 0x0A, 0x06, 0x3F, 0,0,0,0x3F, 0x00, 0x00, 0x00, 0x3F}; 

// Out moded way of doing things! ^^^

// God mode below thanks to Matt vvvvv

float thing[3] = {PI,0,-1}; //Change these floats in the range -pi to pi, 0 to 1 and -1 to 1
char* pkt = reinterpret_cast<char*>(thing);

char TXpkt[16] = {254, 254, 254, 197, pkt[0], pkt[1], pkt[2], pkt[3], pkt[4], pkt[5], pkt[6], pkt[7], pkt[8], pkt[9], pkt[10], pkt[11]};

void loop() {
  for(int i = 0; i < 16; i++){
    Serial3.write(TXpkt[i]);
    
    Serial.print(TXpkt[i]);
  }
  Serial.println();
  delay(10);
}
