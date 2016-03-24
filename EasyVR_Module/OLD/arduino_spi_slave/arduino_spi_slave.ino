//Adopted from Code by Nick Gammon

//Chas Frick
//Wearable Haptic Controller MQP


////////
//Arduino Pinout
// ARDUINO Pin      Teensy Pin
// Pin 13 (SCK)     13
// Pin 12 (MISO)    12  (Level shifted)
// Pin 11 (MOSI)    11
// Pin 10 (CS)      10

#include <SPI.h>

char buf[10];
volatile byte ind;
volatile boolean heard_teensy;

void setup() {
  Serial.begin(115200);

  //Setup pins
  pinMode(MISO,OUTPUT);

  //Put arduino into SPI mode
  SPCR |= _BV(SPE);

  ind = 0;
  heard_teensy = false;

  SPI.attachInterrupt();
}

//SPI interrupt
ISR(SPI_STC_vect){

  byte mail = SPDR; //SPI reg
  
  if(ind < sizeof(buf)){
    buf[ind++] = mail;
    
    if(mail == '\n'){
      heard_teensy = true;
    }
  }
}


void loop() {
  if(heard_teensy){
    buf[ind] = 0;
    Serial.println(buf);
    ind = 0;
    heard_teensy = false;
  }

}
