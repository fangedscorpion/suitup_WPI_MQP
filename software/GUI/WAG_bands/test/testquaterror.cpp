#include "testquaterror.h"
#include "band/abserror.h"

#define PI_HALF 1.57079632679

void TestQuatError::testQuatError(){
//    QVector3D x = QVector3D(1,0,0);
//    QVector3D z = QVector3D(0,0,1);

//    float a1 = 30.0f; // degrees
//    QVector3D axisS = QVector3D(0,1,0);
//    QVector3D axisT = QVector3D(1,0,0);
//    QQuaternion sw = QQuaternion::fromAxisAndAngle(axisS,a1);
//    QQuaternion tw = QQuaternion::fromAxisAndAngle(axisT,a1);
//    QQuaternion er = tw*sw;

//    QuatError qe = QuatError(er,sw,tw,z,x);

//    QVERIFY(qFuzzyCompare(qe.getErr(),er));
//    QVERIFY(qFuzzyCompare(qe.getSwing(),sw));
//    QVERIFY(qFuzzyCompare(qe.getTwist(),tw));
}

void TestQuatError::testToMessage(){
//    QVector3D x = QVector3D(1,0,0);
//    QVector3D z = QVector3D(0,0,1);

//    float a1 = 10.0f; // degrees
//    float a2 = 10.0f; // degrees
//    QVector3D axisS = QVector3D(0,0,1);
//    QVector3D axisT = QVector3D(1,0,0);
//    QQuaternion sw = QQuaternion::fromAxisAndAngle(axisS,a1);
//    QQuaternion tw = QQuaternion::fromAxisAndAngle(axisT,a2);
//    QQuaternion er = tw*sw;

//    QuatError qe = QuatError(er,sw,tw,z,x);

//    QByteArray qba = qe.toMessage();
//    float f1 = *reinterpret_cast<const float*>(qba.mid(0,4).data());
//    float f2 = *reinterpret_cast<const float*>(qba.mid(4,4).data());
//    float f3 = *reinterpret_cast<const float*>(qba.mid(8,4).data());

//    float pi_half = PI_HALF;
//    QVERIFY(qFuzzyCompare(f1,pi_half)); // z is 90 degrees behind the swing motion direction, so pi/2
//    QVERIFY(qFuzzyCompare(f2,a1/20.0f));
//    QVERIFY(qFuzzyCompare(f3,a2/20.0f));
}

void TestQuatError::testWithinTolerance(){
//    QVector3D x = QVector3D(1,0,0);
//    QVector3D z = QVector3D(0,0,1);

//    float a1 = 19.0f; // degrees
//    float a2 = 19.0f; // degrees
//    QVector3D axisS = QVector3D(0,0,1);
//    QVector3D axisT = QVector3D(1,0,0);
//    QQuaternion sw = QQuaternion::fromAxisAndAngle(axisS,a1);
//    QQuaternion tw = QQuaternion::fromAxisAndAngle(axisT,a2);
//    QQuaternion er = tw*sw;

//    QuatError qe = QuatError(er,sw,tw,z,x);

//    QVERIFY(!qe.withinTolerance(9));
//    QVERIFY(qe.withinTolerance(10));

//    a1 = 11.0f;
//    a2 = 13.0f;
//    axisS = QVector3D(0,0,1);
//    axisT = QVector3D(1,0,0);
//    sw = QQuaternion::fromAxisAndAngle(axisS,a1);
//    tw = QQuaternion::fromAxisAndAngle(axisT,a2);
//    er = tw*sw;
//    qe = QuatError(er,sw,tw,z,x);

//    QVERIFY(!qe.withinTolerance(5));
//    QVERIFY(!qe.withinTolerance(6));
//    QVERIFY(qe.withinTolerance(7));

//    a1 = 5.0f;
//    a2 = 3.0f;
//    axisS = QVector3D(0,0,1);
//    axisT = QVector3D(1,0,0);
//    sw = QQuaternion::fromAxisAndAngle(axisS,a1);
//    tw = QQuaternion::fromAxisAndAngle(axisT,a2);
//    er = tw*sw;
//    qe = QuatError(er,sw,tw,z,x);

//    QVERIFY(!qe.withinTolerance(1));
//    QVERIFY(!qe.withinTolerance(2));
//    QVERIFY(qe.withinTolerance(3));
}
