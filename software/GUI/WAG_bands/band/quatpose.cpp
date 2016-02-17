#include "abspose.h"

QuatPose::QuatPose(QVector3D translation, QVector3D rotatedTranslation, QVector3D zAxis) :
    t(translation),
    rt(rotatedTranslation),
    z(zAxis) {
    calibration = new QuatState(1,0,0,0);
    points.reserve(3);
}

QuatState* QuatPose::qqinv(AbsState* q1, AbsState* q2) const {
    // multiplies q1 by the inverse of q2
    QQuaternion* qout = new QQuaternion();
    QQuaternion q = *(static_cast<QuatState*>(q1))*static_cast<QuatState*>(q2)->inverted();
    qout->setScalar(q.scalar());
    qout->setVector(q.vector());
    QuatState* qState = static_cast<QuatState*>(qout);
    return qState;
}

void QuatPose::calibrate(AbsState *calibrationPose) {
    // calibrationPose comes from externally configured calibration
    // state (ie where the entire suit calibration configuration is defined)
    calibration = qqinv(current,calibrationPose);
    current = calibrationPose;
}

AbsState* QuatPose::adjust(AbsState* state) const {
    // shifts the given state by the calibration state
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

