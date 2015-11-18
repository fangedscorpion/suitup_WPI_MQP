#include "glcamera.h"
#include <QMatrix4x4>
#include <QDebug>

GLCamera::GLCamera()
{
    lookingAt = QVector3D(0, 0, 0);
    upVector = QVector3D(0, 1, 0);
    currentPos = QVector3D(0, 0, -3);
    this->lookAt(currentPos, lookingAt, upVector);
    qDebug("set camera location");
}

void GLCamera::zoomToDepth(GLfloat newDepth) {
    setToIdentity();
    static int timesCalled = 0;
    // calculate new camera location
    qDebug("%i", timesCalled);
    qDebug("Trying to change zoom depth to %f", newDepth);
    QVector3D currentDisplacement = currentPos - lookingAt;
    printVect(&currentDisplacement);
    QVector3D directionVect = currentDisplacement/currentDisplacement.length();
    if (directionVect.z() > 0) {
        directionVect *= -1;
    }
    qDebug("%f", directionVect.z());
    printVect(&directionVect);
    currentPos = newDepth*directionVect + lookingAt;
    printVect(&currentPos);
    this->lookAt(currentPos, lookingAt, upVector);
    timesCalled++;
}

void GLCamera::moveCameraToLocation(QVector3D cameraLocation) {
    setToIdentity();
    // get camera ray vector projected onto x,z plane
    QVector3D currentDisplacement = cameraLocation - lookingAt;
    currentDisplacement.setY(0);
    // find the vector pointing left from the camera (this should remain constant and parallel to the x, z place
    // no matter where we put the camera
    QVector3D leftVect = QVector3D::crossProduct(QVector3D(0, 1, 0), currentDisplacement);
    // the upvector should be perpendicular to the ray to what we're looking at and the left vector
    upVector = QVector3D::crossProduct((cameraLocation - lookingAt), leftVect);

    currentPos = cameraLocation;
    this->lookAt(cameraLocation, lookingAt, upVector);
}

void GLCamera::printVect(QVector3D *vectToPrint) {
    qDebug("%f, %f, %f", vectToPrint->x(), vectToPrint->y(), vectToPrint->z());
}

