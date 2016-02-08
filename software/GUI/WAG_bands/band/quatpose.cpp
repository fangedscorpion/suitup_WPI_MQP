#include "abspose.h"

QuatState* QuatPose::qqinv(AbsState* q1, AbsState* q2) const {
    QQuaternion q = *(static_cast<QuatState*>(q1))*static_cast<QuatState*>(q2)->inverted();
    QuatState* qState = static_cast<QuatState*>(&q);
    return qState;
}

void QuatPose::calibrate(AbsState *calibrationPose) {
    calibration = qqinv(current,calibrationPose);
}

AbsState* QuatPose::adjust(AbsState* state) const {
    return qqinv(state,calibration);
}

AbsState* QuatPose::error(AbsState* state) const {
    return qqinv(state,current);
}

void QuatPose::updatePoints(AbsState* parentState, QVector3D parentEndPoint) {
    QVector3D transNoRot = static_cast<QuatState*>(parentState)->rotatedVector(t) + parentEndPoint;
    QVector3D transTotal = transNoRot + static_cast<QuatState*>(current)->rotatedVector(rt);
    points[0] = parentEndPoint;
    points[1] = transNoRot;
    points[2] = transTotal;
}

