#ifndef ABSPOSE_H
#define ABSPOSE_H

#include "absstate.h"
#include <vector>
#include <QVector3D>
#include <QQuaternion>

class AbsPose
{
public:
    AbsPose();
    void update(AbsState* s) {current = adjust(s);}
    void calibrate() {calibration = current;}
    AbsState* getCalibrationState() {return calibration;}
    AbsState* getState() {return current;}
    QVector3D getEndpoint() {return points.back();}

    virtual AbsState* adjust(AbsState* state) const = 0;
    virtual AbsState* error(AbsState* state) const = 0;
    virtual void updatePoints(AbsState* parentState, QVector3D parentEndpoint) = 0;
protected:
    std::vector<QVector3D> points;
    AbsState* current;
    AbsState* calibration;
};

class QuatPose : public AbsPose{
public:
    QuatPose(QVector3D translation, QVector3D rotatedTranslation) : t(translation), rt(rotatedTranslation) {points.reserve(3);}

    AbsState* adjust(AbsState* state) const;
    AbsState* error(AbsState* state) const;
    void updatePoints(AbsState* parentState, QVector3D parentEndpoint);
private:
    QVector3D t;
    QVector3D rt;
};

#endif // ABSPOSE_H
