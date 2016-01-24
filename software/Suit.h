#pragma once
#include "bands/Band.h"
#include <map>

class Suit {
public:
	Suit();
	Band* getBand(BandType bt);
	// bool playback(vector<PositionSnapshot> motion)
	// PositionSnapshot takeSnapshot( )
	// void calibrate( )
	// map<enum, BandCalibration> getCalibrationData( )
	Band* getRefBand() const {return refBand;}; 

private:
	std::map<BandType, Band*> bands;
	Band* refBand;
};

