/*

    Source file for the battery monitor for the Teensy

*/
#include "BatteryMonitor.h"

    BatteryMonitor::BatteryMonitor(){
        pinMode(LOW_BATT_PIN_ACTIVE_LOW, INPUT);
        low_batt_sum = 0;
        lowBatIndex = 0;  
    }

    BatteryMonitor::~BatteryMonitor(){
        pinMode(LOW_BATT_PIN_ACTIVE_LOW, INPUT);
        low_batt_sum = 0;
        lowBatIndex = 0;  
    }

    void BatteryMonitor::initLowBatteryInfo(){
      pinMode(LOW_BATT_PIN_ACTIVE_LOW, INPUT);
      low_batt_sum = 0;
      lowBatIndex = 0;
    }

    void BatteryMonitor::checkBattery(){
      low_batt_sum += !digitalRead(LOW_BATT_PIN_ACTIVE_LOW);
      lowBatIndex++;
      if(lowBatIndex == NUM_LOW_BATT_CYCLES){
        lowBatIndex = 0;

        if(low_batt_sum == NUM_LOW_BATT_CYCLES){
          DEBUG_SERIAL.println("Low batt achieved");
      }
      low_batt_sum = 0;
    }  
}