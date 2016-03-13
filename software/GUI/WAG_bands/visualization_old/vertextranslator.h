#ifndef VERTEXTRANSLATOR_H
#define VERTEXTRANSLATOR_H


#include <qopengl.h>
#include <QVector>
#include <QVector3D>


class VertexTranslator {
public:
    VertexTranslator();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count /6; }
    int getPointCount() const { return pointCount; }
    const GLuint *getIndices() const { return vertexIndices.constData(); }
    void addConvexPlane(QVector<QVector3D> planeCorners, QVector3D translatePlane, QQuaternion rotatePlane);



private:
    void add(const QVector3D &v, const QVector3D &n);
    void drawRectangularPrism(GLfloat width, GLfloat height, GLfloat depth);
    void addPointOrdering(QVector<GLuint> pointOrder, int numPoints);
    void drawRectangle(QVector3D llcorner, QVector3D lrcorner, QVector3D trcorner, QVector3D tlcorner);

    int verticesAdded;


    QVector<GLfloat> m_data;
    int m_count;
    QVector<GLuint> vertexIndices;
    int pointCount;
};

#endif // VERTEXTRANSLATOR_H


