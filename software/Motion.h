#pragma once
#include "PositionSnapshot.h"
#include <string>
#include <time.h>

class Motion {
public:
	Motion();
	std::map<Time, PositionSnapshot> getMotion() { return recording; }
	void writeToFile(string filename);
	void loadFromFile(string filename);
	Motion duplicate();
	Motion crop(Time start, Time end);
private:
	std::map<Time, PositionSnapshot> recording;
	// metadata
};
