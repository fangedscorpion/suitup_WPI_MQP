#ifndef ABSPOSE_H
#define ABSPOSE_H

#include "absstate.h"
#include "abserror.h"
#include <vector>
#include <QVector3D>
#include <QQuaternion>

class AbsPose
{
public:
    AbsPose();
    void update(AbsState* s) {current = adjust(s);}
    AbsState* getCalibrationState() {return calibration;}
    AbsState* getState() {return current;}
    QVector3D getEndpoint() {return points.back();}

    virtual void calibrate(AbsState* calibrationPose) = 0;
    virtual AbsState* adjust(AbsState* state) const = 0;
    virtual IError* error(AbsState* goal) const = 0;
    virtual void updatePoints(AbsState* parentState, QVector3D parentEndpoint) = 0;
protected:
    std::vector<QVector3D> points;
    AbsState* current;
    AbsState* calibration;
};

class QuatPose : public AbsPose{
public:
    QuatPose(QVector3D translation, QVector3D rotatedTranslation, QVector3D zAxis);
    void calibrate(AbsState *calibrationPose);
    AbsState* adjust(AbsState* state) const;
    IError* error(AbsState* goal) const;
    void updatePoints(AbsState* parentState, QVector3D parentEndpoint);
    QuatState* qqinv(AbsState* q1, AbsState* q2) const;

    QVector3D getT() {return t;}
    QVector3D getRT() {return rt;}
    QVector3D getZ() {return z;}
private:
    QVector3D t;
    QVector3D rt;
    QVector3D z;
};

#endif // ABSPOSE_H
