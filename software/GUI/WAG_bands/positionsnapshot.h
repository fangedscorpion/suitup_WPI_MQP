#ifndef POSITIONSNAPSHOT_H
#define POSITIONSNAPSHOT_H

#include <QHash>
#include <QObject>
#include "band/absstate.h"
#include "band/absband.h"
#include <QDebug>

class PositionSnapshot
{
public:
    PositionSnapshot();
    void setSnapshot(QHash<BandType, AbsState *> s) {snapshotData = s;}
    QHash<BandType, AbsState *> getSnapshot() { return snapshotData; }
    //Pose getPositionOf(BandType b) { return snapshot[b]; }
    void addMapping(BandType band, AbsState *pose);
    QSet<BandType> getRecordedBands();
private:
    QHash<BandType, AbsState *> snapshotData;
};

#endif // POSITIONSNAPSHOT_H
