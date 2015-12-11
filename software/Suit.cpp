#include "Suit.h"
#include <utility>

Suit::Suit() {
	bands.insert(std::make_pair<BandType, Band*>(LEFT_LOWER_ARM,
	             new ArmBand(LEFT_LOWER_ARM)));
	bands.insert(std::make_pair<BandType, Band*>(RIGHT_LOWER_ARM,
	             new ArmBand(RIGHT_LOWER_ARM)));
	bands.insert(std::make_pair<BandType, Band*>(LEFT_UPPER_ARM,
	             new ArmBand(LEFT_UPPER_ARM)));
	bands.insert(std::make_pair<BandType, Band*>(RIGHT_UPPER_ARM,
	             new ArmBand(RIGHT_UPPER_ARM)));
	bands.insert(std::make_pair<BandType, Band*>(LEFT_SHOULDER,
	             new ArmBand(LEFT_SHOULDER)));
	bands.insert(std::make_pair<BandType, Band*>(RIGHT_SHOULDER,
	             new ArmBand(RIGHT_SHOULDER)));
	bands.insert(std::make_pair<BandType, Band*>(LEFT_HAND,
	             new ArmBand(LEFT_HAND)));
	bands.insert(std::make_pair<BandType, Band*>(RIGHT_HAND,
	             new ArmBand(RIGHT_HAND)));
	bands.insert(std::make_pair<BandType, Band*>(CHEST,
	             new ArmBand(CHEST)));

}
Band* Suit::getBand(BandType bt) {
	return bands[bt];
}
// bool playback(vector<PositionSnapshot> motion)
// PositionSnapshot takeSnapshot( )
// void calibrate( )
// map<enum, BandCalibration> getCalibrationData( )
