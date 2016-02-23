#include "positionsnapshot.h"
#include <QMetaProperty>
#include "band/absstate.h"

PositionSnapshot::PositionSnapshot()
{

}

void PositionSnapshot::addMapping(BandType band, AbsState *pose) {
    qDebug()<<"Adding mapping for band "<<band<<" pose is "<<pose;
    snapshotData[band] = pose;
}



QSet<BandType> PositionSnapshot::getRecordedBands() {
    return QSet<BandType>::fromList(snapshotData.keys());
}

QDataStream & operator<<(QDataStream & str, PositionSnapshot & v) {
    str << v.getSnapshot();
    return str;
}

QDataStream & operator>>(QDataStream & str, PositionSnapshot & v) {
    QHash<BandType, AbsState *> t;
    str >> t;
    v.setSnapshot(t);
    return str;
}

