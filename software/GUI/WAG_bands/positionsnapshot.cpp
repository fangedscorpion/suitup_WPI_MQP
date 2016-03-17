#include "positionsnapshot.h"
#include <QMetaProperty>
#include "band/absstate.h"

PositionSnapshot::PositionSnapshot()
{

}

PositionSnapshot::~PositionSnapshot() {
    qDeleteAll(snapshotData);
}

void PositionSnapshot::addMapping(BandType band, AbsState *pose) {
    snapshotData[band] = pose;
}



QSet<BandType> PositionSnapshot::getRecordedBands() {
    return QSet<BandType>::fromList(snapshotData.keys());
}
