#include "abspose.h"

QuatState* QuatPose::qqinv(AbsState* q1, AbsState* q2) const {
    QQuaternion q = *(static_cast<QuatState*>(q1))*static_cast<QuatState*>(q2)->inverted();
    QuatState* qState = static_cast<QuatState*>(&q);
    return qState;
}

void QuatPose::calibrate(AbsState *calibrationPose) {
    calibration = qqinv(current,calibrationPose);
    current = calibrationPose;
}

AbsState* QuatPose::adjust(AbsState* state) const {
    return qqinv(state,calibration);
}

IError* QuatPose::error(AbsState* goal) const {
    QVector3D* errAxis = new QVector3D();
    float* angle = new float();
    QuatState* qCurrent = static_cast<QuatState*>(current);

    // calculate error
    QuatState* qErr = qqinv(goal,current);
    // get error axis
    qErr->getAxisAndAngle(errAxis, angle);
    // rotate rt by current to get axis along joint
    QVector3D axis = qCurrent->rotatedVector(rt.normalized());
    // scale the joint axis by the length of its projection onto the error axis
    axis *= QVector3D::dotProduct(*errAxis,axis);

    // err * twist * swing
    // twist = rotation about joint axis
    QQuaternion twist = QQuaternion(qErr->scalar(),axis).normalized();
    // swing = rotation perpendicular to joint axis
    QQuaternion swing = twist.inverted()*(*qErr);

    return new QuatError(*qErr,swing,twist,qCurrent->rotatedVector(z),qCurrent->rotatedVector(rt));
}

void QuatPose::updatePoints(AbsState* parentState, QVector3D parentEndPoint) {
    QVector3D transNoRot = static_cast<QuatState*>(parentState)->rotatedVector(t) + parentEndPoint;
    QVector3D transTotal = transNoRot + static_cast<QuatState*>(current)->rotatedVector(rt);
    points[0] = parentEndPoint;
    points[1] = transNoRot;
    points[2] = transTotal;
}

size_t QuatPose::objectSize() {
    return sizeof(QuatPose);
}
