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
    QList<BandType> keys = snapshotData.keys();
    for (int i = 0; i < keys.length(); i++) {
        qint32 castedKey = keys[i];
        (*ds)<<castedKey;
        AbsState *state = snapshotData[keys[i]];
        if (state->getStateRep() == QUAT_REP) {
            QuatState *quatRep = (QuatState*) state;
            quatRep->serialize(ds);
        } else {
            state->serialize(ds);
        }
    }
    qint32 endByte = END_SNAP_SERIALIZE;
    (*ds)<<endByte;
}

PositionSnapshot * PositionSnapshot::deserialize(QDataStream *ds) {
    bool keepGoing = true;
    qint32 firstThing = 0;
    (*ds)>>firstThing;
    PositionSnapshot *snapshot;
    snapshot = new PositionSnapshot();
    while (keepGoing) {
        if (firstThing == END_SNAP_SERIALIZE) {
            keepGoing = false;
            break;
        }
        AbsState *newState;
        newState = AbsState::deserialize(ds);
        BandType addBand = (BandType) firstThing;
        snapshot->addMapping(addBand, newState);
        (*ds)>>firstThing;
    }
    return snapshot;
}
