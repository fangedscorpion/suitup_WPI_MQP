void setup() {
 Serial3.begin(9600);
}

//Sending -0.5, 0.75, -1
char TXpkt[16] = {254, 254, 254, 197, 0,0,0,0xBF,0,0,0x40,0x3F,0,0,0x80,0xBF}; 

void loop() {
  for(int i = 0; i < 16; i++){
    Serial3.write(TXpkt[i]);
    Serial.print(TXpkt[i]);
  }
  Serial.println();
  delay(10);
}
