#ifndef REFLECTION_H
#define REFLECTION_H

#include <QVector3D>
#include <QQuaternion>

QVector3D reflect(QVector3D v, QVector3D normal);
QQuaternion reflect(QQuaternion q, QVector3D normal);

#endif // REFLECTION_H
