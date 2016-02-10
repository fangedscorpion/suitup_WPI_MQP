#include "reflection.h"
#include <QVector3D>
#include <QQuaternion>

QVector3D reflect(QVector3D v, QVector3D normal){
    // reflect vector v across the plane perpendicular to the given normal vector
    // normal is the axis perpendicular to the reflection plane
    // to reflect vector across xy plane (along z-axis): normal = [0,0,1]
    QQuaternion qref = QQuaternion(normal.normalized().toVector4D());
    return (qref*QQuaternion(v.toVector4D())*qref).vector();
}

QQuaternion reflect(QQuaternion q, QVector3D normal){
    // reflect quaternion rotation q across plane perpendicular to the given normal vector
    QVector3D* axis = new QVector3D();
    float* angle = new float();
    q.getAxisAndAngle(axis,angle);
    return QQuaternion::fromAxisAndAngle(reflect(*axis,normal),-*angle);
}
