#define INTERRUPT_FROM_ARDUINO 3
#define ARDUINO_SERIAL Serial1
#define SERIAL_TO_PC Serial

//#include <SPI.h>

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
  digitalWrite(INTERRUPT_FROM_ARDUINO, HIGH); //Pull-ups
  attachInterrupt(INTERRUPT_FROM_ARDUINO, listenToArduino, FALLING);  
  ARDUINO_SERIAL.begin(115200);
  SERIAL_TO_PC.begin(115200);
  heardArduino = false;
  pinMode(5,OUTPUT);
  digitalWrite(5,HIGH);

 

  //SPI.begin();
}

void loop() {
  if(heardArduino){
    SERIAL_TO_PC.println(receivedCmdFromArduino);
    heardArduino = false;
  }
  if(SERIAL_TO_PC.available()){
    char byteToSend = SERIAL_TO_PC.read();
    SERIAL_TO_PC.print(byteToSend);

    //SPI was considered but tossed out because the Teensy is not meant as an SPI slave although there are libraries out there
    //SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    //SPI.transfer(byteToSend);
    //SPI.transfer('\n');
    //SPI.endTransaction();
    
    //This was old UART stuff
    ARDUINO_SERIAL.print(byteToSend);
    delay(1);
    digitalWrite(5, LOW);//Active low to arduino
    delay(1);
    digitalWrite(5,HIGH);

    SERIAL_TO_PC.flush();//Get rid of everything else in the Serial Buffer
  }
  delay(1);
}
