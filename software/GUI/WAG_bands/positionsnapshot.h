#ifndef POSITIONSNAPSHOT_H
#define POSITIONSNAPSHOT_H

#include <map>
#include <QHash>
#include "band/absstate.h"
#include "band/absband.h"

class PositionSnapshot
{
    Q_PROPERTY(QHash snapshotData READ getSnapshot WRITE PositionSnapshot)
public:
    PositionSnapshot();
    QHash<BandType, AbsState *> getSnapshot() { return snapshotData; }
    //Pose getPositionOf(BandType b) { return snapshot[b]; }
    void addMapping(BandType band, AbsState *pose);
    QSet<BandType> getRecordedBands();
private:
    //std::map<BandType, Pose> snapshot;
    QHash<BandType, AbsState *> snapshotData;
};

#endif // POSITIONSNAPSHOT_H
