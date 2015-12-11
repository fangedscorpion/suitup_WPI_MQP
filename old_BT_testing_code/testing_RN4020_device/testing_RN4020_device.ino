#define pinLEDBlink 7

void setup(){
  //////////////////
  // From: http://forum.arduino.cc/index.php?topic=287457.0
    Serial.begin(115200);
    delay(1000);
    Serial.println("SR,20000000"); //Set device in peripheral mode and set turn on auto advertising
    Serial.println("R,1"); //Reboot
    delay(500);
    Serial.println("A"); //Start advertising
    ////////////////
    pinMode(pinLEDBlink, OUTPUT); //Debug pin
}

void loop() {
  digitalWrite(pinLEDBlink, HIGH);
  delay(500);
  digitalWrite(pinLEDBlink, LOW);
  delay(500);

}
