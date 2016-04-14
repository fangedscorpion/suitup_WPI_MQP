#include "abserror.h"
#include <math.h>
#include <arpa/inet.h>

#define PI 3.14159265359
#define TWO_PI 6.28318530718
#define PI_HALF 1.57079632679

#define MAX_SWING_TOL 20 // deg
#define MAX_TWIST_TOL 20 // deg
#define MAX_SWING_ERR 20 // deg
#define MAX_TWIST_ERR 20 // deg

#define MAX_TOL_INPUT 10

QuatError::QuatError(QQuaternion err, QQuaternion swing, QQuaternion twist, QVector3D zAxis, QVector3D xAxis) :
    err(err),
    swing(swing),
    twist(twist),
    z(zAxis),
    x(xAxis) {

}

QByteArray QuatError::toMessage() const {
    /*
     * Returns a 12-byte QByteArray made of 3 floats:
     * float 1: swing orientation (radians from z to the swing motion vector, from -pi to pi)
     * float 2: swing error (from 0 to 1)
     * float 3: twist error (from -1 to 1)
     */

    float swingAngle, twistAngle;
    QVector3D swingAxis, twistAxis;
    swing.getAxisAndAngle(&swingAxis,&swingAngle);
    twist.getAxisAndAngle(&twistAxis,&twistAngle);

    // find angle between z and the swing axis about x
    float det = QVector3D::dotProduct(x.normalized(),QVector3D::crossProduct(z,swingAxis));
    float dot = QVector3D::dotProduct(z,swingAxis);
    float swingOrientation = atan2(det,dot); // on interval [-pi,+pi]
    // the force is 90 degrees from the swing orientation
    swingOrientation += PI_HALF;
    // keep swing orientation on interval [-pi,+pi]
    if (swingOrientation > PI) swingOrientation -= TWO_PI;

    // twist error between -1 and 1
    if (abs(twistAngle) > MAX_TWIST_ERR)
        twistAngle = twistAngle / abs(twistAngle); // = +/- 1
    else
        twistAngle = twistAngle / MAX_TWIST_ERR; // on interval [-1,+1]

    // swing error between -1 and 1
    if (abs(swingAngle) > MAX_SWING_ERR)
        swingAngle = swingAngle / abs(swingAngle); // = +/- 1
    else
        swingAngle = swingAngle / MAX_SWING_ERR; // on interval [-1,+1]

    // swing error between 0 and 1
    if (swingAngle < 0){
        swingAngle = -swingAngle;
        swingOrientation += PI;
        if (swingOrientation > PI) swingOrientation -= TWO_PI;
    }

    // message contains swing orientation, swing error, twist error
    QByteArray msg = QByteArray();
    qDebug()<<"Swing orientation"<<swingOrientation;
    qDebug()<<"swingAngle"<<swingAngle;
    qDebug()<<"twistAngle"<<twistAngle;
    //char bytes[4] = reinterpret_cast<const char *>(&swingOrientation);
    //int swingOrientationInt = (int) swingOrientation;

//    htonl()
//    uchar orientationBytes[4];
//    qToBigEndian(swingOrientationInt, orientationBytes);
//    msg.append(orientationBytes);
//    qint32 swingAngleInt = (qint32) swingAngle;
//    qToBigEndian(swingAngleInt, orientationBytes);
//    msg.append(orientationBytes);
//    qint32 twistAngleInt = (qint32) twistAngle;
//    qToBigEndian(twistAngleInt, orientationBytes);
//    msg.append(orientationBytes);


    float reversedSwingOrientation = floatHtonl(swingOrientation);
    float reversedSwingAngle = this->floatHtonl(swingAngle);
    float reversedTwistAngle = this->floatHtonl(twistAngle);


    msg.append(reinterpret_cast<const char*>(&reversedSwingOrientation), sizeof(reversedSwingOrientation));
    msg.append(reinterpret_cast<const char*>(&reversedSwingAngle), sizeof(reversedSwingAngle));
    msg.append(reinterpret_cast<const char*>(&reversedTwistAngle), sizeof(reversedTwistAngle));

    //msg.append(reinterpret_cast<const char*>(&swingOrientation), sizeof(swingOrientation));
    //msg.append(reinterpret_cast<const char*>(&swingAngle), sizeof(swingAngle));
    //msg.append(reinterpret_cast<const char*>(&twistAngle), sizeof(twistAngle));

    return msg;
}

bool QuatError::withinTolerance(int tolerance) const {
    // tolerance betweeen 1 and 10
    // this function assumes a tolerance of 0 to require 0 error

    float swingAngle, twistAngle;
    QVector3D swingAxis, twistAxis;
    swing.getAxisAndAngle(&swingAxis,&swingAngle);
    twist.getAxisAndAngle(&twistAxis,&twistAngle);

    bool twistWithinTol = abs(twistAngle) < (MAX_TWIST_TOL*tolerance/MAX_TOL_INPUT);
    bool swingWithinTol = abs(swingAngle) < (MAX_SWING_TOL*tolerance/MAX_TOL_INPUT);

    return twistWithinTol && swingWithinTol;
}


float QuatError::floatHtonl(float value) {
      int temp =  htonl(*(unsigned int*)&value);
      return *(float*) &temp;
    //return value;
}
