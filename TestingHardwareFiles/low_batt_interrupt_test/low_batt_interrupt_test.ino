#define LOW_BATT_PIN_ACTIVE_LOW 12

volatile boolean printIntMsg = false;

uint8_t teapotPacket[14] = { '$', 0x02, 0,0, 0,0, 0,0, 0,0, 0x00, 0x00, '\r', '\n' };

void hasLowBatteryISR(){
  cli();
  printIntMsg = true;  
  sei();
}

#define NUM_LOW_BAT_AVGS  15
int lowBatArray[NUM_LOW_BAT_AVGS];
int lowBatIndex = 0;
int sum = 0;

void setup() {
  Serial.begin(115200);
  Serial3.begin(9600);
  printIntMsg = false;
  pinMode(LOW_BATT_PIN_ACTIVE_LOW, INPUT_PULLUP);
  //attachInterrupt(LOW_BATT_PIN_ACTIVE_LOW, hasLowBatteryISR, FALLING);
  int hasStartingLowBatt = digitalRead(LOW_BATT_PIN_ACTIVE_LOW);
  if(hasStartingLowBatt){
    Serial.println("Started with low battery");
  }
  
  lowBatIndex = 0;
  sum = 0;
}

void loop() {
  lowBatArray[lowBatIndex] = digitalRead(LOW_BATT_PIN_ACTIVE_LOW);
  sum += !digitalRead(LOW_BATT_PIN_ACTIVE_LOW);//lowBatArray[lowBatIndex];
  lowBatIndex++;
  if(lowBatIndex == NUM_LOW_BAT_AVGS){
    lowBatIndex = 0;
   
    Serial.println(String("Reached ") + NUM_LOW_BAT_AVGS + " sum is: " + sum);
    if(sum == NUM_LOW_BAT_AVGS){
      Serial.println("Low batt achieved");
    }
    sum = 0;
  }  
  
//  if(printIntMsg){
//    Serial.println("Got Low Battery");
//    printIntMsg = false; //no longer need this
//  }
  Serial3.write(teapotPacket,14);
  delay(10);
}
