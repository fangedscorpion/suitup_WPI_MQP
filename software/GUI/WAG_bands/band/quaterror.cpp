#include "abserror.h"
#include <math.h>

#define SWING_TOL 0.08725  // 5 deg
#define TWIST_TOL 0.08725  // 5 deg
#define MAX_SWING_ERR 0.35 // 20 deg
#define MAX_TWIST_ERR 0.35 // 20 deg

QuatError::QuatError(QQuaternion err, QQuaternion swing, QQuaternion twist, QVector3D zAxis, QVector3D xAxis) : err(err), swing(swing), twist(twist), z(zAxis), x(xAxis) {

}

QByteArray QuatError::toMessage() const {
    float *swingAngle = new float(), *twistAngle = new float();
    QVector3D *swingAxis = new QVector3D(), *twistAxis = new QVector3D();
    swing.getAxisAndAngle(swingAxis,swingAngle);
    twist.getAxisAndAngle(twistAxis,twistAngle);

    float det = QVector3D::dotProduct(x.normalized(),QVector3D::crossProduct(z,*swingAxis));
    float dot = QVector3D::dotProduct(z,*swingAxis);
    float swingOrientation = atan2(det,dot); // [-pi,+pi]

    // twist error between -1 and 1
    if (abs(*twistAngle) > MAX_TWIST_ERR)
        *twistAngle /= abs(*twistAngle); // = +/- 1
    else
        *twistAngle /= MAX_TWIST_ERR; // [-1,+1]

    // swing error between -1 and 1
    if (abs(*swingAngle) > MAX_SWING_ERR)
        *swingAngle /= abs(*swingAngle); // = +/- 1
    else
        *swingAngle /= MAX_SWING_ERR; // [-1,+1]

    // message contains swing orientation, swing error, twist error
    QByteArray msg = QByteArray();
    msg.append(reinterpret_cast<const char*>(&swingOrientation), sizeof(swingOrientation));
    msg.append(reinterpret_cast<const char*>(&swingAngle), sizeof(swingAngle));
    msg.append(reinterpret_cast<const char*>(&twistAngle), sizeof(twistAngle));

    return msg;
}

bool QuatError::withinTolerance() const {
    float *swingAngle = new float(), *twistAngle = new float();
    QVector3D *swingAxis = new QVector3D(), *twistAxis = new QVector3D();
    swing.getAxisAndAngle(swingAxis,swingAngle);
    twist.getAxisAndAngle(twistAxis,twistAngle);

    // tentative implementation
    return (abs(*twistAngle) < TWIST_TOL) && (abs(*swingAngle < SWING_TOL));
}


