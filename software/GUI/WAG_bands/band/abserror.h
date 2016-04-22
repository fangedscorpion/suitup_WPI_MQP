#ifndef ABSERROR_H
#define ABSERROR_H

#include <QByteArray>
#include <QQuaternion>

class IError {
public:
    virtual ~IError(){}
    virtual QByteArray toMessage() const = 0;
    virtual bool withinTolerance(int tolerance) const = 0;
};

class QuatError : public IError {
public:
    QuatError(QQuaternion err, QQuaternion swing, QQuaternion twist, QVector3D zAxis, QVector3D xAxis, bool flipSwing);
    ~QuatError(){}
    QByteArray toMessage() const;
    bool withinTolerance(int tolerance) const;
    QQuaternion getErr() const {return err;}
    QQuaternion getSwing() const {return swing;}
    QQuaternion getTwist() const {return twist;}
    static float floatHtonl(float value);
private:
    QQuaternion err;
    QQuaternion swing;
    QQuaternion twist;
    QVector3D z;
    QVector3D x;
    bool flipSwing;
};

#endif // ABSERROR_H
