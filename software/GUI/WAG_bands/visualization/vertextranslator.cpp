#include "vertextranslator.h"
#include <qmath.h>

#define APPROX_POINT_NUM 3000

VertexTranslator::VertexTranslator()
    : m_count(0)
{
    m_data.resize(2500 * 6);

    pointCount = 0;
    pointOrdering.resize(APPROX_POINT_NUM);


    //drawTriangle(QVector3D(0, 0, 0), QVector3D(0.1, 0, 0), QVector3D(0.05, 0.05, 0));
    drawRectangularPrism(0.7, 0.4, 1);
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

void VertexTranslator::add(const QVector3D &v) {

    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    m_count += 3;

}

void VertexTranslator::drawTriangle(QVector3D corner1, QVector3D corner2, QVector3D corner3) {
    add(corner1, corner2);
    add(corner3, corner1);
    add(corner2, corner3);
}

QVector<GLuint> VertexTranslator::drawTriangle(GLuint corner1, GLuint corner2, GLuint corner3) {
    QVector<GLuint> indices;
    indices.append(corner1);
    indices.append(corner2);
    indices.append(corner3);
    return indices;
}

void VertexTranslator::drawRectangularPrism(GLfloat width, GLfloat height, GLfloat depth) {
    QVector<QVector3D> vertices(8);
    vertices.append(QVector3D(0, 0, 0));
    vertices.append(QVector3D(0, 0, depth));
    vertices.append(QVector3D(width, 0, depth));
    vertices.append(QVector3D(width, 0, 0));
    vertices.append(QVector3D(0, height, 0));
    vertices.append(QVector3D(0, height, depth));
    vertices.append(QVector3D(width, height, depth));
    vertices.append(QVector3D(width, height, 0));


    QVector<GLuint> indices;
    // draw top face
    indices += drawRectangle(4, 7, 6, 5);
    // draw bottom face
//    indices += drawRectangle(1, 2, 3, 0);
//    // draw left side
//    indices += drawRectangle(1, 0, 4, 5);
//    // draw right side
//    indices += drawRectangle(3, 2, 6, 7);
//    // draw front face
//    indices += drawRectangle(0, 3, 7, 4);
//    // draw back face
//    indices += drawRectangle(2, 1, 5, 6);

    addShape(vertices, 8, indices, indices.size(), QVector3D(0, 0, 0), QVector3D(0, 0, 0));
}

// draws a rectangle
// give corner points ordered counter clockwise when looking at rectangle from side that'd normally be seen
QVector<GLuint> VertexTranslator::drawRectangle(GLuint llcorner, GLuint lrcorner, GLuint trcorner, GLuint tlcorner) {

    QVector<GLuint> indices;
    indices += drawTriangle(llcorner, trcorner, tlcorner);
    indices += drawTriangle(llcorner, lrcorner, trcorner);
    return indices;
}


void VertexTranslator::addShape(QVector<QVector3D> vertices, int numVertices, QVector<GLuint> pointOrder, int numPoints, QVector3D rotation, QVector3D translation) {
    // rotate and translate

    for (int i = 0; i < numVertices; i++) {
        add(vertices[i]);
    }

    addPointOrdering(pointOrder, numPoints);

}

void VertexTranslator::addPointOrdering(QVector<GLuint> pointOrder, int numPoints) {
    for (int i = 0; i < numPoints; i++) {
        pointOrder[i] += pointCount;
    }

    pointOrdering += pointOrder;
    pointCount += numPoints;
}
