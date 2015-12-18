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

#include <SoftwareSerial.h>

// Adopted from the above place by Chas Frick for Wearable Haptic Controller MQP
// Team Suit Up!
// Modified December 2015

SoftwareSerial mySerial(10,11); //RX, TX

#define EASYVR_SERIAL mySerial
#define DEBUG_SERIAL Serial
#include "EasyVR.h"

#define OUTPUT_INTERRUPT_PIN 7 //Interrupt to Teensy
#define HW_JUMPER_FOR_SERIAL_OUTPUT 6 //Stops the Arduino from printing anything but the essential serial messages for teensy testing
boolean PRINT_FLUFF_FOR_TESTING = true;

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

EasyVR easyvr(EASYVR_SERIAL);

void setup() {
    // setup EasyVR serial port (high speed)
    EASYVR_SERIAL.begin(9600);
    DEBUG_SERIAL.begin(115200);
    
    //pinMode(HW_JUMPER_FOR_SERIAL_OUTPUT, INPUT);
    PRINT_FLUFF_FOR_TESTING = false;
    //PRINT_FLUFF_FOR_TESTING = digitalRead(HW_JUMPER_FOR_SERIAL_OUTPUT); //Read jumper
    
    // initialize EasyVR  
  while (!easyvr.detect())
  {
    if(PRINT_FLUFF_FOR_TESTING){
      DEBUG_SERIAL.println(F("EasyVR not detected!"));
    }
    delay(1000);
  }

  easyvr.setPinOutput(EasyVR::IO1, LOW);
  easyvr.setKnob(EasyVR::STRICT); // Set the EasyVR to have the strictest interpretation of the words
  if(PRINT_FLUFF_FOR_TESTING){
    DEBUG_SERIAL.print(F("EasyVR detected, version "));
    DEBUG_SERIAL.println(easyvr.getID());
  }
  easyvr.setTimeout(5);
  
  lang = EasyVR::ENGLISH;
  easyvr.setLanguage(lang);

  pinMode(OUTPUT_INTERRUPT_PIN, OUTPUT);
}

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

void loop() {
 
 //PRINT_FLUFF_FOR_TESTING = digitalRead(HW_JUMPER_FOR_SERIAL_OUTPUT); //Read jumper
 
 if(PRINT_FLUFF_FOR_TESTING){ 
  DEBUG_SERIAL.println(F("Starting to wait for word: "));
 }
 
 easyvr.setPinOutput(EasyVR::IO1, HIGH); // LED on (listening)
 easyvr.recognizeWord(group); //STOP and RUN is in first group --> the module will be busy until it finds the command
 while(!easyvr.hasFinished()){
  //DEBUG_SERIAL.println(F("Waiting for recog"));
  checkMonitorInput();
  delay(1);
 }

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
    // Communication with the Arduino
    // The Arduino hears the word and prints the letter to serial 
    // Then tells the Teensy to check for serial
    if(easyvr.getWord() == 6){ // STOP
      DEBUG_SERIAL.print(F("S")); //Put stop onto output
      digitalWrite(OUTPUT_INTERRUPT_PIN, HIGH);
      delay(1);
      digitalWrite(OUTPUT_INTERRUPT_PIN, LOW);
    }
    if(easyvr.getWord() == 3){ //RUN
      DEBUG_SERIAL.print(F("R")); //Put run onto output serial
      digitalWrite(OUTPUT_INTERRUPT_PIN, HIGH);
      delay(1);
      digitalWrite(OUTPUT_INTERRUPT_PIN, LOW);
    }

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
