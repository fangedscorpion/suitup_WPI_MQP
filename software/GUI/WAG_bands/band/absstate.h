#ifndef ABSSTATE_H
#define ABSSTATE_H

#include <QQuaternion>

class AbsState{
};

class QuatState : public AbsState, public QQuaternion {
};

#endif // ABSSTATE_H
