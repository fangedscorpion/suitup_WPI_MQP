#include "rectangularprism.h"
#include <qmath.h>

RectangularPrism::RectangularPrism()
    : m_count(0)
{
    m_data.resize(2500 * 6);


    //drawTriangle(QVector3D(0, 0, 0), QVector3D(0.1, 0, 0), QVector3D(0.05, 0.05, 0));
    drawRectangularPrism(0.7, 0.4, 1);
    //makeRectangularPrism(0.3, 0.6, 0.9);

    /*const GLfloat x1 = +0.06f;
    const GLfloat y1 = -0.14f;
    const GLfloat x2 = +0.14f;
    const GLfloat y2 = -0.06f;
    const GLfloat x3 = +0.08f;
    const GLfloat y3 = +0.00f;
    const GLfloat x4 = +0.30f;
    const GLfloat y4 = +0.22f;
    const GLfloat x5 = +0.00f;
    const GLfloat y5 = +0.00f;
    const GLfloat x6 = +0.00f;
    const GLfloat y6 = +0.02f;
    const GLfloat x7 = +0.02f;
    const GLfloat y7 = +0.02f;
    const GLfloat x8 = +0.02f;
    const GLfloat y8 = +0.00f;


    quad(x1, y1, x2, y2, y2, x2, y1, x1);
    quad(x3, y3, x4, y4, y4, x4, y3, x3);
    quad(x5, y5, x6, y6, x7, y7, x8, y8);

    extrude(x1, y1, x2, y2);
    extrude(x2, y2, y2, x2);
    extrude(y2, x2, y1, x1);
    extrude(y1, x1, x1, y1);
    extrude(x3, y3, x4, y4);
    extrude(x4, y4, y4, x4);
    extrude(y4, x4, y3, x3);

    extrude(x5, y5, x6, y6);
    extrude(x6, y6, x7, y7); */

    //const int NumSectors = 100;

/*    for (int i = 0; i < NumSectors; ++i) {
        GLfloat angle = (i * 2 * M_PI) / NumSectors;
        GLfloat angleSin = qSin(angle);
        GLfloat angleCos = qCos(angle);
        const GLfloat x5 = 0.30f * angleSin;
        const GLfloat y5 = 0.30f * angleCos;
        const GLfloat x6 = 0.20f * angleSin;
        const GLfloat y6 = 0.20f * angleCos;

        angle = ((i + 1) * 2 * M_PI) / NumSectors;
        angleSin = qSin(angle);
        angleCos = qCos(angle);
        const GLfloat x7 = 0.20f * angleSin;
        const GLfloat y7 = 0.20f * angleCos;
        const GLfloat x8 = 0.30f * angleSin;
        const GLfloat y8 = 0.30f * angleCos;

        quad(x5, y5, x6, y6, x7, y7, x8, y8);

        extrude(x6, y6, x7, y7);
        extrude(x8, y8, x5, y5);
    } */
}

void RectangularPrism::add(const QVector3D &v, const QVector3D &n)
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

void RectangularPrism::drawTriangle(QVector3D corner1, QVector3D corner2, QVector3D corner3) {
    add(corner1, corner2);
    add(corner3, corner1);
    add(corner2, corner3);
}

void RectangularPrism::drawRectangularPrism(GLfloat width, GLfloat height, GLfloat depth) {
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

// draws a rectangle
// give corner points ordered counter clockwise when looking at rectangle from side that'd normally be seen
void RectangularPrism::drawRectangle(QVector3D llcorner, QVector3D lrcorner, QVector3D trcorner, QVector3D tlcorner) {
    drawTriangle(llcorner, trcorner, tlcorner);
    drawTriangle(llcorner, lrcorner, trcorner);
}

