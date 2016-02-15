#include "positionsnapshot.h"

PositionSnapshot::PositionSnapshot()
{

}

void PositionSnapshot::addMapping(BandType band, AbsPose *pose) {
    snapshotData[band] = pose;
}



QSet<BandType> PositionSnapshot::getRecordedBands() {
    return new QSet(snapshotData.keys());
}

