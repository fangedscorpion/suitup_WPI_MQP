#include "Suit.h"
#include <utility>

Suit::Suit() {
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

	bands[RIGHT_SHOULDER]->setDependentBand(bands[CHEST]);
	bands[LEFT_SHOULDER]->setDependentBand(bands[CHEST]);
	bands[LEFT_UPPER_ARM]->setDependentBand(bands[LEFT_SHOULDER]);
	bands[RIGHT_UPPER_ARM]->setDependentBand(bands[RIGHT_SHOULDER]);
	bands[LEFT_LOWER_ARM]->setDependentBand(bands[LEFT_UPPER_ARM]);
	bands[RIGHT_LOWER_ARM]->setDependentBand(bands[RIGHT_UPPER_ARM]);
	bands[LEFT_HAND]->setDependentBand(bands[LEFT_LOWER_ARM]);
	bands[RIGHT_HAND]->setDependentBand(bands[RIGHT_LOWER_ARM]);

}
Band* Suit::getBand(BandType bt) {
	return bands[bt];
}
// bool playback(vector<PositionSnapshot> motion)
// PositionSnapshot takeSnapshot( )
// void calibrate( )
// map<enum, BandCalibration> getCalibrationData( )
