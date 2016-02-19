#include "abserror.h"
#include <math.h>

#define MAX_SWING_TOL 0.35 // 20 deg
#define MAX_TWIST_TOL 0.35 // 20 deg
#define MAX_SWING_ERR 0.35 // 20 deg
#define MAX_TWIST_ERR 0.35 // 20 deg

#define MAX_TOL_INPUT 10

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
        *twistAngle /= MAX_TWIST_ERR; // on interval [-1,+1]

    // swing error between -1 and 1
    if (abs(*swingAngle) > MAX_SWING_ERR)
        *swingAngle /= abs(*swingAngle); // = +/- 1
    else
        *swingAngle /= MAX_SWING_ERR; // on interval [-1,+1]

    // message contains swing orientation, swing error, twist error
    QByteArray msg = QByteArray();
    msg.append(reinterpret_cast<const char*>(&swingOrientation), sizeof(swingOrientation));
    msg.append(reinterpret_cast<const char*>(&swingAngle), sizeof(swingAngle));
    msg.append(reinterpret_cast<const char*>(&twistAngle), sizeof(twistAngle));

    return msg;
}

bool QuatError::withinTolerance(int tolerance) const {
    // tolerance betweeen 1 and 10
    // this function assumes a tolerance of 0 to require 0 error

    float *swingAngle = new float(), *twistAngle = new float();
    QVector3D *swingAxis = new QVector3D(), *twistAxis = new QVector3D();
    swing.getAxisAndAngle(swingAxis,swingAngle);
    twist.getAxisAndAngle(twistAxis,twistAngle);

    bool twistWithinTol = abs(*twistAngle) < (MAX_TWIST_TOL*tolerance/MAX_TOL_INPUT);
    bool swingWithinTol = abs(*swingAngle) < (MAX_SWING_TOL*tolerance/MAX_TOL_INPUT);

    delete swingAxis;
    delete twistAxis;
    delete swingAngle;
    delete twistAngle;

    return twistWithinTol && swingWithinTol;
}


