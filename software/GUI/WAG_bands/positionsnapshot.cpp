#include "positionsnapshot.h"

PositionSnapshot::PositionSnapshot()
{

}

void PositionSnapshot::addMapping(BandType band, AbsState *pose) {
    snapshotData[band] = pose;
}



QSet<BandType> PositionSnapshot::getRecordedBands() {
    return QSet<BandType>::fromList(snapshotData.keys());
}

//QDataStream &operator<<(QDataStream &ds, const PositionSnapshot &obj) {
//    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
//        if(obj.metaObject()->property(i).isStored(&obj)) {
//            ds << obj.metaObject()->property(i).read(&obj);

//        }
//    }
//    return ds;
//}
//QDataStream &operator>>(QDataStream &ds, PositionSnapshot &obj) {
//    QVariant var;
//    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
//        if(obj.metaObject()->property(i).isStored(&obj)) {
//            ds >> var;
//            obj.metaObject()->property(i).write(&obj, var);
//        }
//    }
//    return ds;
//}

