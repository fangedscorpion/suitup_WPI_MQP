/**
  EasyVR Tester

  Dump contents of attached EasyVR module
  and exercise it with playback and recognition.

  Serial monitor can be used to send a few basic commands:
  'm1' - set mic distance to HEADSET
  'm2' - set mic distance to ARMS_LENGTH (default)
  'm3' - set mic distance to FAR_MIC
  'w' - enter sleep mode without audio wakeup (any command interrupts)
  'ww' - enter sleep mode with whistle to wakeup
  'wl' - enter sleep mode with loudnoise to wakeup
  
  With EasyVR Shield, the green LED is ON while the module
  is listening (using pin IO1 of EasyVR).
  Successful recognition is acknowledged with a beep.
  Details are displayed on the serial monitor window.

**
  Example code for the EasyVR library v1.6
  Written in 2014 by RoboTech srl for VeeaR <http:://www.veear.eu>

  To the extent possible under law, the author(s) have dedicated all
  copyright and related and neighboring rights to this software to the
  public domain worldwide. This software is distributed without any warranty.

  You should have received a copy of the CC0 Public Domain Dedication
  along with this software.
  If not, see <http://creativecommons.org/publicdomain/zero/1.0/>.
*/

// Adopted from the above place by Chas Frick for Wearable Haptic Controller MQP
// Team Suit Up!
// Modified December 2015

// Modified again in March 2016 by Chas Frick
// The code is still as aggravating to work with, but it works with the chestpiece
// To get it to work, I had to change the bootloader for the ATMega328P so that it used the internal oscillator
// Since we are running it at 3.3V the max clock speed is 8MHz so we have to watch out for that
// I also had to solder an additional VCC pin on the bottom of the chestpiece on the Arduino because the package didn't connect it


///////////////////////////////////////
// Communications to EasyVR
#include "EasyVR.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(4,5); //RX, TX
#define EASYVR_SERIAL mySerial
EasyVR easyvr(EASYVR_SERIAL);

///////////////////////////////////////
// Communications from/To Teensy
// The interrupt line is pulled high from the teensy if something happens
// Otherwise the Arduino uses DEBUG_SERIAL to talk to Teensy
#define DEBUG_SERIAL Serial
#define ARDUINO_HAS_MAIL_PIN 0 // PIN 2 on the BOARD!
volatile boolean arduino_has_rxed_mail = false;
#define OUTPUT_INTERRUPT_PIN 7 //Interrupt to Teensy
#define HW_JUMPER_FOR_SERIAL_OUTPUT 6 //Stops the Arduino from printing anything but the essential serial messages for teensy testing
volatile char theMailByte;

//ISR to process incoming data from Teensy
void checkMail(){
  bool tmp = easyvr.stop();
  if(DEBUG_SERIAL.available()){
    theMailByte = DEBUG_SERIAL.read();
    DEBUG_SERIAL.flush(); 
  }
  arduino_has_rxed_mail = true;
}


// This is used to supress the print outs from the Arduino for testing the EasyVR
boolean PRINT_FLUFF_FOR_TESTING = false;
////////////////////////////////////////
//EasyVR settings 
int8_t bits = 4;
int8_t set = 0;
int8_t group = 1;// STOP and RUN in this group
uint32_t mask = 0;
uint8_t train = 0;
uint8_t grammars = 0;
int8_t lang = 0;
char name[33];
bool useCommands = true;
bool useTokens = false;
bool isSleeping = false;
////////////////////////////////////////

void setup() {
    // setup EasyVR serial port (high speed)
    EASYVR_SERIAL.begin(9600);
    

    // Setup interrupt on interrupt pin from Teesny
    // Set up comms to Teensy
    DEBUG_SERIAL.begin(9600);
    pinMode(ARDUINO_HAS_MAIL_PIN+2, INPUT);
    digitalWrite(ARDUINO_HAS_MAIL_PIN+2, HIGH); // Pull up resistors
    attachInterrupt(ARDUINO_HAS_MAIL_PIN, checkMail, FALLING);
    pinMode(9, OUTPUT);
    arduino_has_rxed_mail = false;
        
    //pinMode(HW_JUMPER_FOR_SERIAL_OUTPUT, INPUT);
    //PRINT_FLUFF_FOR_TESTING = digitalRead(HW_JUMPER_FOR_SERIAL_OUTPUT); //Read jumper (This was to enable a hardware pin for testing, but not used)
    //PRINT_FLUFF_FOR_TESTING = false;
    
    //Setup the interrupt output to the Teensy
    pinMode(OUTPUT_INTERRUPT_PIN, OUTPUT);
    digitalWrite(OUTPUT_INTERRUPT_PIN, HIGH); 

///////////////////////////////////////
    // All this crap is to setup the EasyVR
    // initialize EasyVR  
  while (!easyvr.detect())
  {
    if(PRINT_FLUFF_FOR_TESTING){
      DEBUG_SERIAL.println(F("EasyVR not detected!"));
    }
    delay(1000);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  easyvr.setKnob(EasyVR::STRICT); // Set the EasyVR to have the second strictest interpretation of the words
  if(PRINT_FLUFF_FOR_TESTING){
    DEBUG_SERIAL.print(F("EasyVR detected, version "));
    DEBUG_SERIAL.println(easyvr.getID());
  }
  easyvr.setTimeout(5);
  
  lang = EasyVR::ENGLISH;
  easyvr.setLanguage(lang);
/////////////////////////////////////////// 
}

/////////////////////////////////////////////////////////////////////
bool checkMonitorInput() //From TestEasyVR code. Stripped down quite a bit
{
  //PRINT_FLUFF_FOR_TESTING = digitalRead(HW_JUMPER_FOR_SERIAL_OUTPUT); //Read jumper
  
  if (DEBUG_SERIAL.available() <= 0)
    return false;

  // check console commands
  int16_t rx = DEBUG_SERIAL.read();
  if (isSleeping)
  {
    // any character received will exit sleep
    isSleeping = false;
    easyvr.stop();
    if(PRINT_FLUFF_FOR_TESTING){
      DEBUG_SERIAL.println(F("Forced wake-up!"));
    }
    return true;
  }
  
  if (rx == 'm')
  {
    int16_t num = 0;
    delay(5);
    while ((rx = DEBUG_SERIAL.read()) >= 0)
    {
      delay(5);
      if (isdigit(rx))
        num = num * 10 + (rx - '0');
      else
        break;
    }
    if(PRINT_FLUFF_FOR_TESTING){
      DEBUG_SERIAL.print(F("Mic distance "));
      DEBUG_SERIAL.println(num);
    }
    easyvr.stop();
    easyvr.setMicDistance(num);
  }
  if (rx == 'w')
  {
    int8_t mode = 0;
    delay(5);
    while ((rx = DEBUG_SERIAL.read()) >= 0)
    {
      delay(5);
      if (rx == 'w')
        mode = EasyVR::WAKE_ON_WHISTLE;
      if (rx == 'l')
        mode = EasyVR::WAKE_ON_LOUDSOUND;
    }
    if(PRINT_FLUFF_FOR_TESTING){
      DEBUG_SERIAL.print(F("Sleep mode "));
      DEBUG_SERIAL.println(mode);
    }
    easyvr.stop();
    easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off
    isSleeping = easyvr.sleep(mode);
    return true;
  }

  if (rx >= 0)
  {
    easyvr.stop();
    DEBUG_SERIAL.flush();
    return true;
  }
  return false;
}

//////////////////////////////////////////////////////////////////////
///Word groups
const char* ws0[] =
{
  "ROBOT",
};
const char* ws1[] =
{
  "ACTION",
  "MOVE",
  "TURN",
  "RUN",
  "LOOK",
  "ATTACK",
  "STOP",
  "HELLO",
};
const char* ws2[] =
{
  "LEFT",
  "RIGHT",
  "UP",
  "DOWN",
  "FORWARD",
  "BACKWARD",
};
const char* ws3[] =
{
  "ZERO",
  "ONE",
  "TWO",
  "THREE",
  "FOUR",
  "FIVE",
  "SIX",
  "SEVEN",
  "EIGHT",
  "NINE",
  "TEN",
};
const char** ws[] = { ws0, ws1, ws2, ws3 };
//////////////////////////////////////////////////////////////////////

/////
// Function to setup voice recognition of a word
void setupVR(){
  if(PRINT_FLUFF_FOR_TESTING){ 
  DEBUG_SERIAL.println(F("Starting to wait for word: "));
 }
 
 easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)
 easyvr.recognizeWord(group); //STOP and RUN is in first group --> the module will be busy until it finds the command
}


//////////////////////////////////////////////////////////////////////
//Main loop
void loop() {
 
 //PRINT_FLUFF_FOR_TESTING = digitalRead(HW_JUMPER_FOR_SERIAL_OUTPUT); //Read jumper --> Not used but could be added
 
 setupVR(); //Start recognition
 
 while(!easyvr.hasFinished()){
  
  checkMonitorInput();
  
  if(arduino_has_rxed_mail){
    break;
  }
  
  delay(1);
 }

 if(arduino_has_rxed_mail){
   //Do something useful with this information! This is a placeholder now
   if(theMailByte == 't'){
      
      digitalWrite(9,HIGH);
      delay(1000);
      digitalWrite(9,LOW);
   }
   DEBUG_SERIAL.print(theMailByte);
   arduino_has_rxed_mail = false;
 }
 else{

 easyvr.setPinOutput(EasyVR::IO1, LOW); // LED off

  // handle voice recognition from TestEasyVR example
  int16_t idx;
  idx = easyvr.getWord();
  if (idx >= 0)
  {
    if(PRINT_FLUFF_FOR_TESTING){
      DEBUG_SERIAL.print(F("Word: "));
      DEBUG_SERIAL.print(easyvr.getWord());
    }

    //////////////////////////////////
    // Communication with the Teensy
    // The Arduino hears the word and prints the letter to serial 
    // Then tells the Teensy to check for serial
    if(easyvr.getWord() == 6){ // STOP
      DEBUG_SERIAL.print(F("S")); //Put stop onto output
      delay(10);
      digitalWrite(OUTPUT_INTERRUPT_PIN, LOW);
      delay(1);
      digitalWrite(OUTPUT_INTERRUPT_PIN, HIGH);
    }
    if(easyvr.getWord() == 3){ //RUN
      DEBUG_SERIAL.print(F("R")); //Put run onto output serial
      delay(10);
      digitalWrite(OUTPUT_INTERRUPT_PIN, LOW);
      delay(1);
      digitalWrite(OUTPUT_INTERRUPT_PIN, HIGH);
    }
    //////////////////////////////////////////////////////////////////////
    
    if(PRINT_FLUFF_FOR_TESTING){
      DEBUG_SERIAL.print(F(" = "));
    }
    
    if (useCommands)
       if(PRINT_FLUFF_FOR_TESTING){
          DEBUG_SERIAL.println(ws[group][idx]);
       }
    // --- optional: builtin words can be retrieved from the module
    else if (set < 4)
      if(PRINT_FLUFF_FOR_TESTING){
          DEBUG_SERIAL.println(ws[set][idx]);
      }
    // ---
    else
    {
      uint8_t flags, num;
      if (easyvr.dumpGrammar(set, flags, num))
        while (idx-- >= 0)
        {
          if (!easyvr.getNextWordLabel(name))
            break;
        }
      if (idx < 0)
          if(PRINT_FLUFF_FOR_TESTING){
              DEBUG_SERIAL.println(name);
          }
      else
           if(PRINT_FLUFF_FOR_TESTING){
              DEBUG_SERIAL.println();
           }
     }
    
    // ok, let's try another set
    if (set < 4)
    {
      set++;
      if (set > 3)
        set = 0;
    }
    else
    {
      set++;
      if (set >= grammars)
        set = 4;
    }
    easyvr.playSound(0, EasyVR::VOL_FULL);
  }
  else
  {
    idx = easyvr.getCommand();
    if (idx >= 0)
    {
      if(PRINT_FLUFF_FOR_TESTING){
        DEBUG_SERIAL.print(F("Command: "));
        DEBUG_SERIAL.print(easyvr.getCommand());
      }
      if (easyvr.dumpCommand(group, idx, name, train))
      {
        if(PRINT_FLUFF_FOR_TESTING){
          DEBUG_SERIAL.print(F(" = "));
          DEBUG_SERIAL.println(name);
      }
      else
        if(PRINT_FLUFF_FOR_TESTING){
          DEBUG_SERIAL.println();
        }
      // ok, let's try another group
      do
      {
        group++;
        if (group > EasyVR::PASSWORD)
          group = 0;
      }
      while (!((mask >> group) & 1));
      easyvr.playSound(0, EasyVR::VOL_FULL);
    }
    else // errors or timeout
    {
      if (easyvr.isTimeout())
        if(PRINT_FLUFF_FOR_TESTING){
          DEBUG_SERIAL.println(F("Timed out, try again..."));
        }
      int16_t err = easyvr.getError();
      if (err >= 0)
      {
        if(PRINT_FLUFF_FOR_TESTING){
          DEBUG_SERIAL.print(F("Error 0x"));
          DEBUG_SERIAL.println(err, HEX);
        }
      }
    }
  }
  }
 }
}
