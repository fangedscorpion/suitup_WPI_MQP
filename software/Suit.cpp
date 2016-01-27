#include "Suit.h"
#include <utility>
#include <string.h>
#include <stdexcept>

Suit::Suit() {
	try {
		bands.insert(std::make_pair<BandType, Band*>(CHEST,
				 	 new ChestBand()));
		bands.insert(std::make_pair<BandType, Band*>(LEFT_SHOULDER,
					 new ShoulderBand(LEFT_SHOULDER)));
		bands.insert(std::make_pair<BandType, Band*>(RIGHT_SHOULDER,
					 new ShoulderBand(RIGHT_SHOULDER)));
		bands.insert(std::make_pair<BandType, Band*>(LEFT_UPPER_ARM,
					 new ArmBand(LEFT_UPPER_ARM)));
		bands.insert(std::make_pair<BandType, Band*>(RIGHT_UPPER_ARM,
					 new ArmBand(RIGHT_UPPER_ARM)));
		bands.insert(std::make_pair<BandType, Band*>(LEFT_LOWER_ARM,
					 new ArmBand(LEFT_LOWER_ARM)));
		bands.insert(std::make_pair<BandType, Band*>(RIGHT_LOWER_ARM,
					 new ArmBand(RIGHT_LOWER_ARM)));
		bands.insert(std::make_pair<BandType, Band*>(RIGHT_HAND,
					 new Glove(RIGHT_HAND)));
		bands.insert(std::make_pair<BandType, Band*>(LEFT_HAND,
	            	 new Glove(LEFT_HAND)));
	} catch (const std::invalid_argument& e) {
		char str[100];
		strcpy(str, "Error in Suit's band: ");
		strcpy(str, e.what());
		throw std::logic_error(str);
	}

	refBand = bands[CHEST];

	bands[CHEST]->addDependentBand(bands[RIGHT_SHOULDER]);
	bands[CHEST]->addDependentBand(bands[LEFT_SHOULDER]);
	bands[LEFT_SHOULDER]->addDependentBand(bands[LEFT_UPPER_ARM]);
	bands[RIGHT_SHOULDER]->addDependentBand(bands[RIGHT_UPPER_ARM]);
	bands[LEFT_UPPER_ARM]->addDependentBand(bands[LEFT_LOWER_ARM]);
	bands[RIGHT_UPPER_ARM]->addDependentBand(bands[RIGHT_LOWER_ARM]);
	bands[LEFT_LOWER_ARM]->addDependentBand(bands[LEFT_HAND]);
	bands[RIGHT_LOWER_ARM]->addDependentBand(bands[RIGHT_HAND]);

}

Band* Suit::getBand(BandType bt) {
	return bands[bt];
}
// bool playback(vector<PositionSnapshot> motion)
// PositionSnapshot takeSnapshot( )
// void calibrate( )
// map<enum, BandCalibration> getCalibrationData( )
