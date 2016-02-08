#ifndef POSITIONSNAPSHOT_H
#define POSITIONSNAPSHOT_H

#include <map>
#include <QHash>
#include "band/abspose.h"
#include "band/absband.h"

class PositionSnapshot
{
public:
    PositionSnapshot();
    //std::map<BandType, Pose> getSnapshot() { return snapshot; }
    //Pose getPositionOf(BandType b) { return snapshot[b]; }
private:
    //std::map<BandType, Pose> snapshot;
    QHash<BandType, AbsPose *> snapshotData;
};

#endif // POSITIONSNAPSHOT_H
