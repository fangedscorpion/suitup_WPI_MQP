#include "abspose.h"

QuatPose::QuatPose(QVector3D xAxis, QVector3D zAxis) :
    x(xAxis),
    z(zAxis) {
    calibration = new QuatState(1,0,0,0);
    current = new QuatState(1,0,0,0);
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

void QuatPose::calibrate() {
    // TODO tell model node to calibrate


}

IError* QuatPose::error(AbsState* goal) const {
    //    % returns swing and twist components of this quaternion about
    //    % axis pure quaternion axis
    //    % qerr = twist * swing
    //    axis = axis.v/norm(axis.v);
    //    p = dot(qerr.v, axis)*axis;
    //    twist = Quaternion([qerr.w; p]).unit();
    //    swing = inv(twist) * qerr;
    QVector3D axis = x.normalized();
    QuatState* qerr = qqinv(goal,current);

    QVector3D p = QVector3D::dotProduct(qerr->vector(),axis)*axis;
    QQuaternion twist = QQuaternion(qerr->scalar(),p).normalized();
    QQuaternion swing = twist.inverted() * (*qerr);

    return new QuatError(*qerr,swing,twist,((QuatState*)current)->rotatedVector(z),((QuatState*)current)->rotatedVector(x));
}

size_t QuatPose::objectSize() {
    return sizeof(QuatPose);
}

void QuatPose::update(AbsState *s){
    // TODO push state to model node
}
