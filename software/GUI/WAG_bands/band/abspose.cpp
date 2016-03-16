#include "abspose.h"

AbsPose::AbsPose() : QObject()
{

}

//void AbsPose::update(AbsState *s){
//     qDebug("Update");
//}

//QDataStream &operator<<(QDataStream &ds, const AbsPose &obj) {
//    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
//        if(obj.metaObject()->property(i).isStored(&obj)) {
//            ds << obj.metaObject()->property(i).read(&obj);

//        }
//    }
//    return ds;
//}
//QDataStream &operator>>(QDataStream &ds, AbsPose &obj) {
//    QVariant var;
//    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
//        if(obj.metaObject()->property(i).isStored(&obj)) {
//            ds >> var;
//            obj.metaObject()->property(i).write(&obj, var);
//        }
//    }
//    return ds;
//}

//QDataStream &operator<<(QDataStream &ds, const AbsState &obj) {
//    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
//        if(obj.metaObject()->property(i).isStored(&obj)) {
//            ds << obj.metaObject()->property(i).read(&obj);

//        }
//    }
//    return ds;
//}
//QDataStream &operator>>(QDataStream &ds, AbsState &obj) {
//    QVariant var;
//    for(int i=0; i<obj.metaObject()->propertyCount(); ++i) {
//        if(obj.metaObject()->property(i).isStored(&obj)) {
//            ds >> var;
//            obj.metaObject()->property(i).write(&obj, var);
//        }
//    }
//    return ds;
//}
