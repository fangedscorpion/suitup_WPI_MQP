#include "abspose.h"

AbsState* QuatPose::adjust(AbsState* state) const {
    QQuaternion q = *(static_cast<QuatState*>(state))*static_cast<QuatState*>(calibration)->inverted();
    QuatState* qState = static_cast<QuatState*>(&q);
    return qState;
}

AbsState* QuatPose::error(AbsState* state) const {
    QQuaternion q = *(static_cast<QuatState*>(state))*static_cast<QuatState*>(current)->inverted();
    QuatState* qState = static_cast<QuatState*>(&q);
    return qState;
}

void QuatPose::updatePoints(AbsState* parentState, QVector3D parentEndPoint) {
    QVector3D transNoRot = static_cast<QuatState*>(parentState)->rotatedVector(t) + parentEndPoint;
    QVector3D transTotal = transNoRot + static_cast<QuatState*>(current)->rotatedVector(rt);
    points[0] = parentEndPoint;
    points[1] = transNoRot;
    points[2] = transTotal;
}

