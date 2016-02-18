#include "abspose.h"

QuatPose::QuatPose(QVector3D translation, QVector3D rotatedTranslation, QVector3D zAxis) :
    t(translation),
    rt(rotatedTranslation),
    z(zAxis) {
    calibration = new QuatState(1,0,0,0);
    current = new QuatState(1,0,0,0);

    const int npts = 3;
    for (int i=0; i<npts; i++)
        points.push_back(new QVector3D());
}

QuatState* QuatPose::qqinv(AbsState* q1, AbsState* q2) const {
    // multiplies q1 by the inverse of q2
    QQuaternion* qout = new QQuaternion();
    QQuaternion q = *((QuatState*)q1)*((QuatState*)q2)->inverted();
    qout->setScalar(q.scalar());
    qout->setVector(q.vector());
    QuatState* qState = (QuatState*)qout;
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
//    % returns swing and twist components of this quaternion about
//    % axis pure quaternion axis
//    % qerr = twist * swing
//    axis = axis.v/norm(axis.v);
//    p = dot(qerr.v, axis)*axis;
//    twist = Quaternion([qerr.w; p]).unit();
//    swing = inv(twist) * qerr;
    QVector3D axis = rt.normalized();
    QuatState* qerr = qqinv(goal,current);

    QVector3D p = QVector3D::dotProduct(qerr->vector(),axis)*axis;
    QQuaternion twist = QQuaternion(qerr->scalar(),p).normalized();
    QQuaternion swing = twist.inverted() * (*qerr);

    return new QuatError(*qerr,swing,twist,((QuatState*)current)->rotatedVector(z),((QuatState*)current)->rotatedVector(rt));
}

void QuatPose::updatePoints(AbsState* parentState, QVector3D* parentEndPoint) {
    QVector3D transNoRot = ((QuatState*)parentState)->rotatedVector(t) + *parentEndPoint;
    QVector3D transTotal = transNoRot + ((QuatState*)current)->rotatedVector(rt);

    points[0] = parentEndPoint;
    *(points[1]) = 1*transNoRot;
    *(points[2]) = 1*transTotal;
}

