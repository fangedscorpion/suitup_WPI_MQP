#include "test/testquatpose.h"
#include "band/abspose.h"

void TestQuatPose::testQuatPose(){
    QVector3D t = QVector3D(1,0,0);
    QVector3D rt = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(t,rt,z);
    QCOMPARE(z,qp->getZ());
    QCOMPARE(t,qp->getT());
    QCOMPARE(rt,qp->getRT());
    QuatState* cs = static_cast<QuatState*>(qp->getCalibrationState());
    QCOMPARE(QuatState(1,0,0,0),*cs);
}

void TestQuatPose::testUpdate(){
    QVector3D t = QVector3D(1,0,0);
    QVector3D rt = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(t,rt,z);
    QQuaternion q = QuatState::fromAxisAndAngle(QVector3D(1,0,0),30);
    QuatState* adj = static_cast<QuatState*>(&q);

    qp->update(adj);
    QuatState* qps = static_cast<QuatState*>(qp->getState());
    QVERIFY(qFuzzyCompare(*qps,*adj));
}

void TestQuatPose::testGetCalibrationState(){
    QSKIP("Not yet implemented");
}

void TestQuatPose::testGetState(){
    QSKIP("Not yet implemented");
}

void TestQuatPose::testGetEndpoint(){
    QSKIP("Not yet implemented");
}

void TestQuatPose::testCalibrate(){
    QSKIP("Not yet implemented");
}

void TestQuatPose::testAdjust(){
    QuatPose* qp = new QuatPose(QVector3D(1,0,0),QVector3D(1,0,0),QVector3D(0,0,1));
    QSKIP("Not yet implemented");
}

void TestQuatPose::testError(){
    QSKIP("Not yet implemented");
}

void TestQuatPose::testUpdatePoints(){
    QSKIP("Not yet implemented");
}

void TestQuatPose::testQQinv(){
    QVector3D t = QVector3D(1,0,0);
    QVector3D rt = QVector3D(2,0,0);
    QVector3D z = QVector3D(0,0,1);
    QuatPose* qp = new QuatPose(t,rt,z);

    QQuaternion q1 = QuatState::fromAxisAndAngle(QVector3D(1,0,0),30);
    QuatState* qs1 = static_cast<QuatState*>(&q1);
    QQuaternion q2 = q1.inverted().inverted();
    QuatState* qs2 = static_cast<QuatState*>(&q2);

    QuatState* qf = qp->qqinv(qs1,qs2);

    qf->setVector(qf->vector()+QVector3D(1,1,1));
    QVERIFY(qFuzzyCompare(*qf,QQuaternion(1,1,1,1)));
}
