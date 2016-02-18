#ifndef POSITIONSNAPSHOT_H
#define POSITIONSNAPSHOT_H

#include <map>
#include <QHash>
#include "band/abspose.h"
#include "band/absband.h"

class PositionSnapshot
{
    Q_PROPERTY(QHash snapshotData READ getSnapshot WRITE PositionSnapshot)
public:
    PositionSnapshot();
    QHash<BandType, AbsPose *> getSnapshot() { return snapshotData; }
    //Pose getPositionOf(BandType b) { return snapshot[b]; }
private:
    //std::map<BandType, Pose> snapshot;
    QHash<BandType, AbsPose *> snapshotData;
};

#endif // POSITIONSNAPSHOT_H
