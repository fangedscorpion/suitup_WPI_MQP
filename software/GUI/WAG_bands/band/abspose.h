#ifndef ABSPOSE_H
#define ABSPOSE_H

#include "absstate.h"
#include "abserror.h"
#include <vector>
#include <QVector3D>
#include <QQuaternion>

class Node;

class AbsPose : public QObject {
//    Q_PROPERTY(std::vector<QVector3D> points READ getPoints WRITE setPoints)
    Q_OBJECT

public:
    AbsPose();
    void addNode(Node* node){this->node = node;}

    virtual AbsState* getCalibrationState() const = 0; //{return calibration;}
    virtual AbsState* getState() const = 0; //{return current;}
    virtual void update(AbsState* s) = 0;
    virtual void calibrate() = 0;
    virtual IError* error(AbsState* goal) const = 0;
    virtual size_t objectSize() = 0;
protected:
    Node* node;
};

class QuatPose : public AbsPose{
public:
    QuatPose(QVector3D xAxis, QVector3D zAxis);

    AbsState* getCalibrationState() const;
    AbsState* getState() const;
    void update(AbsState *s);
    void calibrate();
    IError* error(AbsState* goal) const;
    size_t objectSize();

    QVector3D getX() {return x;}
    QVector3D getZ() {return z;}

    QuatState* qqinv(AbsState* q1, AbsState* q2) const;

private:
    QVector3D x;
    QVector3D z;
};

#endif // ABSPOSE_H
