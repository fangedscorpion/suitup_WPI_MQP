int pins[6] = {3,5,6,9,10,11};

void setup() {
  for(int i = 0; i< 6; i++){
    pinMode(pins[i], OUTPUT);  
  }
  Serial.begin(115200);
  
}
int tot = 0;
void loop() {
  
 for(int i = 0; i < 6; i++){
  int writeval = random(0,255);
  Serial.println("P"+String(i)+" " + writeval);
  analogWrite(pins[i], writeval);
  delay(50);
 }
}
