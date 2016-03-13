#include "test/testquatpose.h"
#include "band/abspose.h"
#include <math.h>

#define PI 3.14159265

void TestQuatPose::testQuatPose(){
    QVector3D x = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(x,z);
    QCOMPARE(z,qp->getZ());
    QCOMPARE(x,qp->getX());
    QuatState* cs = (QuatState*)(qp->getCalibrationState());
    QCOMPARE(QuatState(1,0,0,0),*cs);
}

void TestQuatPose::testUpdate(){
    QVector3D x = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(x,z);
    QQuaternion q = QuatState::fromAxisAndAngle(QVector3D(1,0,0),30);
    QuatState* adj = (QuatState*)(&q);

    qp->update(adj);
    QuatState* qps = (QuatState*)(qp->getState());
    QVERIFY(qFuzzyCompare(*qps,*adj));
}

void TestQuatPose::testCalibrate(){
    // Probably fails now
    QVector3D x = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(x,z);

    float a1 = -30.0f;
    QVector3D axis = QVector3D(1,0,0);
    QQuaternion q1 = QQuaternion::fromAxisAndAngle(axis,a1);
    QuatState* currstate = (QuatState*)(&q1);
    qp->update(currstate);

    float a2 = 30.0f;
    QQuaternion q2 = QQuaternion::fromAxisAndAngle(axis,a2);
    QuatState* calstate = (QuatState*)(&q2);
//    qp->calibrate(calstate);

    QuatState* caladj = (QuatState*)(qp->getCalibrationState());
    QQuaternion caladj2 = q1*q2.inverted();

    QVERIFY(qFuzzyCompare(*caladj,caladj2));

    float* angle = new float();
    QVector3D* axis2 = new QVector3D();
    caladj->getAxisAndAngle(axis2,angle);

    QVERIFY((qFuzzyCompare(a1-a2,-*angle) && qFuzzyCompare(*axis2,-axis))  ||
            (qFuzzyCompare(a1-a2,*angle) && qFuzzyCompare(*axis2,axis)));

}

void TestQuatPose::testError(){
    QVector3D x = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(x,z);

    float a1 = 30.0f;
    QVector3D axis = QVector3D(1,0,0);
    QQuaternion qgoal = QQuaternion::fromAxisAndAngle(axis,a1);
    QuatState* goalstate = (QuatState*)(&qgoal);
    QuatError* err = (QuatError*)(qp->error(goalstate));
    // swing component should be identity, twist component should be original error
    QQuaternion sw = err->getSwing();
    QQuaternion tw = err->getTwist();
    QQuaternion er = err->getErr();
    QVERIFY(qFuzzyCompare(QQuaternion(sw.scalar(),sw.vector()+QVector3D(1,1,1)),QQuaternion(1,1,1,1)));
    QVERIFY(qFuzzyCompare(er,tw*sw));
    QVERIFY(qFuzzyCompare(er,tw));

    a1 = 30.0f;
    axis = QVector3D(0,1,0);
    qgoal = QQuaternion::fromAxisAndAngle(axis,a1);
    goalstate = (QuatState*)(&qgoal);
    err = (QuatError*)(qp->error(goalstate));
    // twist component should be identity, swing component should be original error
    sw = err->getSwing();
    tw = err->getTwist();
    er = err->getErr();
    QVERIFY(qFuzzyCompare(QQuaternion(tw.scalar(),tw.vector()+QVector3D(1,1,1)),QQuaternion(1,1,1,1)));
    QVERIFY(qFuzzyCompare(er,tw*sw));
    QVERIFY(qFuzzyCompare(er,sw));

    a1 = 30.0f;
    QVector3D axisS = QVector3D(0,1,0);
    QVector3D axisT = QVector3D(1,0,0);
    QQuaternion qgoalS = QQuaternion::fromAxisAndAngle(axisS,a1);
    QQuaternion qgoalT = QQuaternion::fromAxisAndAngle(axisT,a1);
    qgoal = qgoalT*qgoalS;
    goalstate = (QuatState*)(&qgoal);
    err = (QuatError*)(qp->error(goalstate));
    sw = err->getSwing();
    tw = err->getTwist();
    er = err->getErr();
    QVERIFY(qFuzzyCompare(er,tw*sw));
    QVERIFY(qFuzzyCompare(er,qgoal));
    QVERIFY(qFuzzyCompare(QQuaternion(qgoalS.scalar(),qgoalS.vector()+QVector3D(5,5,5)),QQuaternion(sw.scalar(),sw.vector()+QVector3D(5,5,5))));
    QVERIFY(qFuzzyCompare(QQuaternion(qgoalT.scalar(),qgoalT.vector()+QVector3D(5,5,5)),QQuaternion(tw.scalar(),tw.vector()+QVector3D(5,5,5))));
}

void TestQuatPose::testQQinv(){
    QVector3D x = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(x,z);

    QQuaternion q1 = QuatState::fromAxisAndAngle(QVector3D(1,0,0),30);
    QuatState* qs1 = (QuatState*)(&q1);
    QQuaternion q2 = q1.inverted().inverted();
    QuatState* qs2 = (QuatState*)(&q2);

    QuatState* qf = qp->qqinv(qs1,qs2);

    // it doesn't like to compare zeros: add (1,1,1) to both vectors
    qf->setVector(qf->vector()+QVector3D(1,1,1));
    QVERIFY(qFuzzyCompare(*qf,QQuaternion(1,1,1,1)));
}
