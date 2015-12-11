void setup() {
 Serial.begin(9600);
 pinMode(13, OUTPUT);

 //This is to factory reset the other BTLE device
 pinMode(12, OUTPUT);
}

void loop() {
  digitalWrite(12, HIGH);
  delay(50);
  digitalWrite(12, LOW);
  delay(50);
//  Serial.println("Hello World! I am a new bluetooth module. Please allow me to send a really long message such as this one!");
//  digitalWrite(13,HIGH); //For seeing when msg is sent
//  delay(4000);
//  digitalWrite(13,LOW);
//  delay(1000);
}
