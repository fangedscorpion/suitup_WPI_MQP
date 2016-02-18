#ifndef ABSSTATE_H
#define ABSSTATE_H

#include <QQuaternion>

class AbsState{
};

class QuatState : public AbsState, public QQuaternion {
public:
    QuatState(const QVector4D &vector) : QQuaternion(vector){}
    QuatState(float scalar, const QVector3D &vector) : QQuaternion(scalar,vector){}
    QuatState(float scalar, float xpos, float ypos, float zpos) : QQuaternion(scalar,xpos,ypos,zpos){}
    QuatState() : QQuaternion(){}
};

#endif // ABSSTATE_H
