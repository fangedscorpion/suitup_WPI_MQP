#include "suit.h"
#include <QHash>
#include <stdexcept>

Suit::Suit(WifiManager *comms):QObject() {

    try {
        bands[CHEST] = new ChestBand();
        bands[LEFT_SHOULDER] = new ShoulderBand(LEFT_SHOULDER);
        bands[RIGHT_SHOULDER] = new ShoulderBand(RIGHT_SHOULDER);
        bands[LEFT_UPPER_ARM] = new ArmBand(LEFT_UPPER_ARM);
        bands[RIGHT_UPPER_ARM] = new ArmBand(RIGHT_UPPER_ARM);
        bands[LEFT_LOWER_ARM] = new ArmBand(LEFT_LOWER_ARM);
        bands[RIGHT_LOWER_ARM] = new ArmBand(RIGHT_LOWER_ARM);
       /* bands.insert(std::make_pair<BandType, AbsBand*>(RIGHT_HAND,
                                                     new Glove(RIGHT_HAND)));
        bands.insert(std::make_pair<BandType, AbsBand*>(LEFT_HAND,
                                                     new Glove(LEFT_HAND))); */

    } catch (const std::invalid_argument& e) {
        char str[100];
        strcpy(str, "Error in Suit's band: ");
        strcpy(str, e.what());
        throw std::logic_error(str);
    }

    bands[RIGHT_SHOULDER]->setDependentBand(bands[CHEST]);
    bands[LEFT_SHOULDER]->setDependentBand(bands[CHEST]);
    bands[LEFT_UPPER_ARM]->setDependentBand(bands[LEFT_SHOULDER]);
    bands[RIGHT_UPPER_ARM]->setDependentBand(bands[RIGHT_SHOULDER]);
    bands[LEFT_LOWER_ARM]->setDependentBand(bands[LEFT_UPPER_ARM]);
    bands[RIGHT_LOWER_ARM]->setDependentBand(bands[RIGHT_UPPER_ARM]);
    /* bands[LEFT_HAND]->setDependentBand(bands[LEFT_LOWER_ARM]);
    bands[RIGHT_HAND]->setDependentBand(bands[RIGHT_LOWER_ARM]); */

    connect(comms, SIGNAL(dataAvailable(BandType,QByteArray,QTime)), this, SLOT(getRecvdData(BandType,QByteArray,QTime)));

}


void Suit::getRecvdData(BandType band, QByteArray data, QTime dataTimestamp) {
    // process QByteArray here or let individual band do it?
    // pass data to band
    AbsBand *targetBand = getBand(band);
    // send data to target band
}


AbsBand* Suit::getBand(BandType bt) {
    return bands[bt];
}
// bool playback(vector<PositionSnapshot> motion)
// PositionSnapshot takeSnapshot( )
// void calibrate( )
// map<enum, BandCalibration> getCalibrationData( )

