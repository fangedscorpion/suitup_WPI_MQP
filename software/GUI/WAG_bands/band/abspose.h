#ifndef ABSPOSE_H
#define ABSPOSE_H

#include "absstate.h"
#include "abserror.h"
#include <vector>
#include <QVector3D>
#include <QQuaternion>

class AbsPose {
//    Q_PROPERTY(std::vector<QVector3D> points READ getPoints WRITE setPoints)

public:
    AbsPose();
    virtual void update(AbsState* s) = 0;
    AbsState* getCalibrationState() const {return calibration;}
    AbsState* getState() const {return current;}

    virtual void calibrate() = 0;
    virtual IError* error(AbsState* goal) const = 0;
    virtual size_t objectSize() = 0;
protected:
    std::vector<QVector3D*> points;
    AbsState* current;
    AbsState* calibration;
};

class QuatPose : public AbsPose{
public:
    void update(AbsState *s);
    QuatPose(QVector3D xAxis, QVector3D zAxis);
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
