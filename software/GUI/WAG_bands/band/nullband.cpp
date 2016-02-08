#include "absband.h"


NullBand::NullBand() : AbsBand(_NULL) {
    setActive(false);
}

AbsState* NullBand::getState(){
    return static_cast<QuatState*>(new QQuaternion(1,0,0,0));
}

QVector3D NullBand::getEndpoint(){
    return QVector3D(0,0,0);
}
