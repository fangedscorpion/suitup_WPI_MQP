#include "vertextranslator.h"
#include <qmath.h>
#include <QDebug>
#include <QQuaternion>
#include <QMatrix4x4>

VertexTranslator::VertexTranslator()
    : m_count(0)
{
    m_data.resize(2500 * 6);

    pointCount = 0;
    verticesAdded = 0;

    drawRectangularPrism(0.7, 0.4, 1);

    QVector<QVector3D> points = QVector<QVector3D>();
    points.append(QVector3D(-0.5, 0.1, 0));
    points.append(QVector3D(-0.5, 0.3, 0));
    points.append(QVector3D(-0.8, 0.3, 0));
    points.append(QVector3D(-0.8, 0.1, 0));

    //addConvexPlane(points, QVector3D(0, 0, 0), QQuaternion());
}

void VertexTranslator::add(const QVector3D &v, const QVector3D &n)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_count += 6;
}


void VertexTranslator::drawRectangularPrism(GLfloat width, GLfloat height, GLfloat depth) {
    QVector3D vertices[] = {
        QVector3D(0, 0, 0),
        QVector3D(0, 0, depth),
        QVector3D(width, 0, depth),
        QVector3D(width, 0, 0),
        QVector3D(0, height, 0),
        QVector3D(0, height, depth),
        QVector3D(width, height, depth),
        QVector3D(width, height, 0)
    };

    qDebug()<<"Drawing rect";

    // draw top face
    drawRectangle(vertices[4], vertices[7], vertices[6], vertices[5]);
    // draw bottom face
    drawRectangle(vertices[1], vertices[2], vertices[3], vertices[0]);
    // draw left side
    drawRectangle(vertices[1], vertices[0], vertices[4], vertices[5]);
    // draw right side
    drawRectangle(vertices[3], vertices[2], vertices[6], vertices[7]);
    // draw front face
    drawRectangle(vertices[0], vertices[3], vertices[7], vertices[4]);
    // draw back face
    drawRectangle(vertices[2], vertices[1], vertices[5], vertices[6]);
}


void VertexTranslator::addPointOrdering(QVector<GLuint> pointOrder, int numPoints) {
    for (int i = 0; i < numPoints; i++) {
        pointOrder[i] += verticesAdded;
    }

    vertexIndices += pointOrder;
    pointCount += numPoints;
}

void VertexTranslator::addConvexPlane(QVector<QVector3D> planeCorners, QVector3D translatePlane, QQuaternion rotatePlane) {
    if (planeCorners.size() < 3) {
        // cannot have plane defined with 2 points
        qDebug("plane cannot have less than 3 poitns");
        return;
    }
    QMatrix4x4 movePlane = QMatrix4x4();
    // may need to move translation after rotation...
    movePlane.translate(translatePlane);
    movePlane.rotate(rotatePlane);
    int i;

    for (i = 0; i < planeCorners.size(); i++) {
        planeCorners[i] = movePlane*planeCorners[i];
    }

    QVector3D planeNormal = -1*QVector3D::normal(planeCorners[0], planeCorners[1], planeCorners[2]);

    for (i = 0; i < planeCorners.size(); i++) {
        // maybe switch...?
        add(planeCorners[i], planeNormal);
    }

    QVector<GLuint> indices = QVector<GLuint>();

    for (i = 0; i < planeCorners.size() - 2; i++) {
        indices.append(0);
        indices.append(i+2);
        indices.append(i+1);
    }
    addPointOrdering(indices, indices.size());

    verticesAdded += planeCorners.size();
}

void VertexTranslator::drawRectangle(QVector3D llcorner, QVector3D lrcorner, QVector3D trcorner, QVector3D tlcorner) {
    QVector<QVector3D> points = QVector<QVector3D>();
    points.append(llcorner);
    points.append(lrcorner);
    points.append(trcorner);
    points.append(tlcorner);
    addConvexPlane(points, QVector3D(0, 0, 0), QQuaternion());

}

