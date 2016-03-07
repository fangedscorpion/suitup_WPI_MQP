#include "positionsnapshot.h"
#include <QMetaProperty>
#include "band/absstate.h"

PositionSnapshot::PositionSnapshot()
{

}

void PositionSnapshot::addMapping(BandType band, AbsState *pose) {
    //qDebug()<<"PositionSnapshot: Adding mapping for band "<<band<<" pose is "<<pose;
    snapshotData[band] = pose;
}



QSet<BandType> PositionSnapshot::getRecordedBands() {
    return QSet<BandType>::fromList(snapshotData.keys());
}
