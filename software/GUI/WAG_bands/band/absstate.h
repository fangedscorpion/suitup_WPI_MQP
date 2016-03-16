#ifndef ABSSTATE_H
#define ABSSTATE_H

#include <QQuaternion>

enum PositionRepresentation {QUATERNION};

class AbsState{
public:
    AbsState(PositionRepresentation stateRep) : stateRep(stateRep) {}
    PositionRepresentation getStateRep(){return stateRep;}
protected:
    PositionRepresentation stateRep;
};

class QuatState : public AbsState, public QQuaternion {
public:
    QuatState(const QVector4D &vector) : AbsState(QUATERNION), QQuaternion(vector) {}
    QuatState(float scalar, const QVector3D &vector) : AbsState(QUATERNION), QQuaternion(scalar,vector){}
    QuatState(float scalar, float xpos, float ypos, float zpos) : AbsState(QUATERNION), QQuaternion(scalar,xpos,ypos,zpos){}
    QuatState() : AbsState(QUATERNION), QQuaternion(){}
    QuatState(QQuaternion q) : AbsState(QUATERNION), QQuaternion(q.scalar(),q.vector()){}

};

inline QDataStream & operator<<(QDataStream & str, AbsState*) {
    return str;
}

inline QDataStream & operator>>(QDataStream & str, AbsState*) {
    return str;
}

//inline QDataStream & operator<<(QDataStream & str, QuatState*) {
//    return str;
//}

//inline QDataStream & operator>>(QDataStream & str, QuatState*) {
//    return str;
//}

#endif // ABSSTATE_H
