#ifndef POSITIONSNAPSHOT_H
#define POSITIONSNAPSHOT_H

#include <QHash>
#include <QObject>
#include "band/absstate.h"
#include "band/absband.h"
#include <QDebug>


// this can't be used as a band type
#define END_SNAP_SERIALIZE 32

class PositionSnapshot
{
public:
    PositionSnapshot();
    ~PositionSnapshot() {qDeleteAll(snapshotData);}
    void setSnapshot(QHash<BandType, AbsState *> s) {snapshotData = s;}
    QHash<BandType, AbsState *> getSnapshot() const { return snapshotData; }
    //Pose getPositionOf(BandType b) { return snapshot[b]; }
    void addMapping(BandType band, AbsState *pose);
    QSet<BandType> getRecordedBands();
    void serialize(QDataStream *ds);
    static PositionSnapshot *deserialize(QDataStream *ds);
private:
    QHash<BandType, AbsState *> snapshotData;
};

#endif // POSITIONSNAPSHOT_H
