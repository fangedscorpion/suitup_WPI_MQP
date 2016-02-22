#include "testquatstate.h"
#include "band/absstate.h"

void TestQuatState::testQuatState(){
    float w=1, x=2, y=3, z=4;
    QQuaternion qcmp1 = QQuaternion(w,x,y,z);
    QQuaternion qcmp2 = QQuaternion();

    QuatState qs1 = QuatState(QVector4D(x,y,z,w));
    QuatState qs2 = QuatState(w,QVector3D(x,y,z));
    QuatState qs3 = QuatState(w,x,y,z);
    QuatState qs4 = QuatState();

    QCOMPARE(qcmp1,(QQuaternion)qs1);
    QCOMPARE(qcmp1,(QQuaternion)qs2);
    QCOMPARE(qcmp1,(QQuaternion)qs3);
    QCOMPARE(qcmp2,(QQuaternion)qs4);
}
