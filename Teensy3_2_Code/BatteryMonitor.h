#ifndef BatteryMonitor_h
#define BatteryMonitor_h
/*
	
	Everything for the low battery monitor for the Teensy
	which does a simple averaging for a certain number of loop cycles to read a value

*/
#include <Arduino.h>
#include "WAGBandCommon.h"
	
class BatteryMonitor
{
public:

	#define LOW_BATT_PIN_ACTIVE_LOW 12
	int low_batt_sum = 0;
	int lowBatIndex = 0;
	#define NUM_LOW_BATT_CYCLES 15

	BatteryMonitor();
	~BatteryMonitor();

	void initLowBatteryInfo();

	void checkBattery();
};


#endif