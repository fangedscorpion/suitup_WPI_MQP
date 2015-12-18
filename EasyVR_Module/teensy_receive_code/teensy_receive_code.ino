#define INTERRUPT_FROM_ARDUINO 2
#define ARDUINO_SERIAL Serial1
#define SERIAL_TO_PC Serial

volatile char receivedCmdFromArduino; //To receive in the interrupt
volatile boolean heardArduino = false; //flag for testing printing

void listenToArduino(){
  if(ARDUINO_SERIAL.available()){
    receivedCmdFromArduino = ARDUINO_SERIAL.read();
    heardArduino = true;
  }
}

void setup() {
  pinMode(INTERRUPT_FROM_ARDUINO, INPUT);
  attachInterrupt(INTERRUPT_FROM_ARDUINO, listenToArduino, RISING);  
  ARDUINO_SERIAL.begin(115200);
  SERIAL_TO_PC.begin(115200);
  heardArduino = false;
}

void loop() {
  if(heardArduino){
    SERIAL_TO_PC.println(receivedCmdFromArduino);
    heardArduino = false;
  }
  delay(1);
}
