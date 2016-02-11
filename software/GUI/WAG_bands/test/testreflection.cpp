#include "testreflection.h"
#include "math/reflection.h"

bool qFuzzyCompare2(QQuaternion q1,QQuaternion q2){
    return qFuzzyCompare(q1,-q2) || qFuzzyCompare(q1,q2);
}

void TestReflection::testVector(){
    QVector3D v1 = QVector3D(5,5,5);
    QVector3D n1 = QVector3D(1,0,0);
    QVector3D n2 = QVector3D(-2,0,0);

    QVector3D rv1 = reflect(v1,n1);
    QVector3D rv2 = reflect(v1,n2);
    QVERIFY(qFuzzyCompare(rv1,QVector3D(-5,5,5)));
    QVERIFY(qFuzzyCompare(rv1,rv2));  // reflect vector along scalar multiple of normal

    QVector3D rrv1 = reflect(v1,n1);
    QVERIFY(qFuzzyCompare(rv1,rrv1)); // reflect vector then reflect it back

    QVector3D v3 = QVector3D(1,2,3);
    QVector3D n3 = QVector3D(1,1,0);
    QVector3D rv3 = reflect(v3,n3);
    QVERIFY(qFuzzyCompare(rv3,QVector3D(-2,-1,3)));

    QVector3D v4 = QVector3D(0,0,0);
    QVector3D n4 = QVector3D(1,5,-2);
    QVERIFY(qFuzzyCompare(reflect(v4,n4),v4)); // reflect zero vector
}

void TestReflection::testQuaternion(){
    QQuaternion q1 = QQuaternion(1,0,0,0);
    QQuaternion q2 = QQuaternion(-1,0,0,0);
    QVERIFY(qFuzzyCompare2(q1,q2));

    QQuaternion q3 = QQuaternion::fromAxisAndAngle(QVector3D(1,2,3),90);
    QQuaternion q4 = QQuaternion::fromAxisAndAngle(QVector3D(-2,-1,3),-90);
    QVector3D n = QVector3D(1,1,0);
    QVERIFY(qFuzzyCompare2(q3,reflect(q4,n)));

    QVERIFY(qFuzzyCompare2(q3,reflect(reflect(q3,n),n))); // reflect it then reflect it back
}
