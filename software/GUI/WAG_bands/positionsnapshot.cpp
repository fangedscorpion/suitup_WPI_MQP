#include "positionsnapshot.h"
#include <QMetaProperty>
#include "band/absstate.h"

PositionSnapshot::PositionSnapshot()
{
    snapshotData = QHash<BandType, AbsState *>();
}

void PositionSnapshot::addMapping(BandType band, AbsState *pose) {
    snapshotData[band] = pose;
}

QSet<BandType> PositionSnapshot::getRecordedBands() {
    return QSet<BandType>::fromList(snapshotData.keys());
}


void PositionSnapshot::serialize(QDataStream *ds) {
    qDebug()<<"Serializing position snap";
    QList<BandType> keys = snapshotData.keys();
    qDebug()<<keys;
    qDebug()<<"keys.length"<<keys.length();
    for (int i = 0; i < keys.length(); i++) {
        qint32 castedKey = keys[i];
        (*ds)<<castedKey;
        //AbsState *state = snapshotData[keys[i]];
        //state->serialize(ds);
        qint32 serializeThis = 84;
        (*ds)<<serializeThis;
    }
    qint32 endByte = END_SNAP_SERIALIZE;
    qDebug()<<"adding "<<END_SNAP_SERIALIZE<<"To output";
    (*ds)<<endByte;
}

PositionSnapshot * PositionSnapshot::deserialize(QDataStream *ds) {
    bool keepGoing = true;
    qint32 firstThing = 0;
    (*ds)>>firstThing;
    PositionSnapshot *snapshot;
    snapshot = new PositionSnapshot();
    int iterations = 0;
    while (keepGoing) {
        qDebug()<<"iterations "<<iterations;
        iterations++;
        qDebug("Getting new absState");
        qDebug()<<firstThing;
        qDebug()<<END_SNAP_SERIALIZE;
        if (firstThing == END_SNAP_SERIALIZE) {
            keepGoing = false;
            break;
        } else if (iterations > 20) {
            break;
        }
        /* AbsState *newState;
        newState = AbsState::deserialize(ds);
        BandType addBand = (BandType) firstThing;
        snapshot->addMapping(addBand, newState);
        qDebug()<<"Done with current absstate";*/
        qint32 newInt = 0;
        (*ds)>>newInt;
        qDebug()<<"NEW INT "<<newInt;
        (*ds)>>firstThing;
    }
    qDebug()<<"Done with positionSnap";
    return snapshot;
}
