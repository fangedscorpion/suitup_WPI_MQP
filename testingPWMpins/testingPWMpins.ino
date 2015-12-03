void setup() {
//  pinMode(23, OUTPUT);
//  pinMode(22, OUTPUT);
//  //pinMode(21, OUTPUT);/// This pin doesn't want to write PWM data
//  pinMode(20, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
}

void loop() {
  for(int i = 0; i<256; i++){
    //analogWrite(23, i);
    //analogWrite(22, i);
    analogWrite(6, i);
    analogWrite(5, i);
    analogWrite(4, i);
    analogWrite(3, i);
    analogWrite(10, i);
    analogWrite(9, i);
    
    //analogWrite(20, i);
    delay(100);
  }

}
