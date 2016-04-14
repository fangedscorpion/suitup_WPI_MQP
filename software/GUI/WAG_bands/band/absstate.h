#ifndef ABSSTATE_H
#define ABSSTATE_H

#include <QQuaternion>

enum PositionRepresentation {QUAT_REP};

class AbsState{
public:
    AbsState(PositionRepresentation stateRep) : stateRep(stateRep) {}
    ~AbsState(){}
    PositionRepresentation getStateRep(){return stateRep;}
    void serialize(QDataStream *ds);
    static AbsState *deserialize(QDataStream *);
protected:
    PositionRepresentation stateRep;
};

class QuatState : public AbsState, public QQuaternion {
public:
    QuatState(const QVector4D &vector) : AbsState(QUAT_REP), QQuaternion(vector) {}
    QuatState(float scalar, const QVector3D &vector) : AbsState(QUAT_REP), QQuaternion(scalar,vector){}
    QuatState(float scalar, float xpos, float ypos, float zpos) : AbsState(QUAT_REP), QQuaternion(scalar,xpos,ypos,zpos){}
    QuatState() : AbsState(QUAT_REP), QQuaternion(){}
    QuatState(QQuaternion q) : AbsState(QUAT_REP), QQuaternion(q.scalar(),q.vector()){}
    ~QuatState(){}
    void serialize(QDataStream *ds);
};

inline QDataStream & operator<<(QDataStream & str, AbsState*) {
    return str;
}

inline QDataStream & operator>>(QDataStream & str, AbsState*) {
    return str;
}

#endif // ABSSTATE_H
