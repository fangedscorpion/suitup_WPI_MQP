#ifndef GLCAMERA_H
#define GLCAMERA_H

#include <QVector3D>
#include <QMatrix4x4>
#include <qopengl.h>

class GLCamera : public QMatrix4x4
{
public:
    GLCamera();

    void zoomToDepth(GLfloat newDepth);
    void moveCameraToLocation(QVector3D cameraLocation);




private:
    QVector3D upVector;
    QVector3D lookingAt;
    QVector3D currentPos;

    void printVect(QVector3D *vectToPrint);
};

#endif // GLCAMERA_H
