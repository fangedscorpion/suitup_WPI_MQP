#include "abspose.h"
#include "model/model.h"
#include <stdexcept>

QuatPose::QuatPose(QVector3D xAxis, QVector3D zAxis, BandType b) :
    AbsPose(b),
    x(xAxis),
    z(zAxis) {
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
    node->calibrate(node->getDefaultRotation());
}

IError* QuatPose::error(AbsState* goal) const {
    //    % returns swing and twist components of this quaternion about
    //    % axis pure quaternion axis
    //    % qerr = twist * swing
    //    axis = axis.v/norm(axis.v);
    //    p = dot(qerr.v, axis)*axis;
    //    twist = Quaternion([qerr.w; p]).unit();
    //    swing = inv(twist) * qerr;
    QVector3D axis = x.normalized();//((QuatState*)getState())->rotatedVector(x).normalized();
    QuatState* qerr = qqinv(goal,getState());

/* debug */
    float a = 0;
    QVector3D v = QVector3D(0,0,0);
    qerr->getAxisAndAngle(&v,&a);
    qDebug() << "axis: " << v;
    qDebug() << "angle: " << a;
/* /debug */

    QVector3D p = QVector3D::dotProduct(qerr->vector(),axis)*axis;
    QQuaternion twist = QQuaternion(qerr->scalar(),p).normalized();
    QQuaternion swing = twist.inverted() * (*qerr);

    if (bandType == LEFT_SHOULDER) return new QuatError(*qerr,swing,twist,z,x,false);
    else return new QuatError(*qerr,swing,twist,z,x,true);
}

void QuatPose::update(BandType type,AbsState *s){
    QuatState* qs = static_cast<QuatState*>(s);
    QQuaternion adjustment;

    switch (type){
    case LEFT_LOWER_ARM:
        adjustment = QQuaternion::fromAxisAndAngle(0,0,1,-90);
        break;
    case LEFT_UPPER_ARM:
        adjustment = QQuaternion::fromAxisAndAngle(0,0,1,-90);
        break;
    case LEFT_SHOULDER:
        adjustment = QQuaternion::fromAxisAndAngle(-1,1,1,120);
        break;
    case RIGHT_LOWER_ARM:
        adjustment = QQuaternion::fromAxisAndAngle(1,1,0,180);
        break;
    case RIGHT_UPPER_ARM:
        adjustment = QQuaternion::fromAxisAndAngle(1,1,0,180);
        break;
    case RIGHT_SHOULDER:
        adjustment = QQuaternion::fromAxisAndAngle(-1,1,1,120);
        break;
    case CHEST:
        adjustment = QQuaternion::fromAxisAndAngle(1,1,1,-120);
        break;
    default:
        throw std::invalid_argument("Unrecognized band type.");
    }

    *qs = *qs * adjustment;
    node->pushNewOrientation(*qs);
}

AbsState* QuatPose::getCalibrationState() const {
    return new QuatState(node->getCalibration());
}

AbsState* QuatPose::getState() const {
    return new QuatState(node->getWorldRotation());
}

size_t QuatPose::objectSize() {
    return sizeof(QuatPose);
}

