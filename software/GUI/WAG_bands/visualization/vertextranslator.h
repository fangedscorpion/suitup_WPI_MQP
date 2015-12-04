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
    int vertexCount() const { return m_count / 3; }
    int getPointCount() const { return pointCount; }
    const GLuint *getIndices() const { return pointOrdering.constData(); }
    void addShape(QVector<QVector3D> vertices, int numVertices, QVector<GLuint> pointOrder, int numPoints, QVector3D rotation, QVector3D translation);


private:
    void quad(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2, GLfloat x3, GLfloat y3, GLfloat x4, GLfloat y4);
    void extrude(GLfloat x1, GLfloat y1, GLfloat x2, GLfloat y2);
    void add(const QVector3D &v, const QVector3D &n);
    void add(const QVector3D &v);
    void addPointOrdering(QVector<GLuint> pointOrder, int numPoints);
    void makeRectangularPrism(GLfloat width, GLfloat height, GLfloat depth);
    void drawTriangle(QVector3D corner1, QVector3D corner2, QVector3D corner3);
    void drawRectangularPrism(GLfloat width, GLfloat height, GLfloat depth);
    void drawRectangle(QVector3D llcorner, QVector3D lrcorner, QVector3D trcorner, QVector3D tlcorner);

    QVector<GLuint> drawTriangle(GLuint corner1, GLuint corner2, GLuint corner3);
    QVector<GLuint> drawRectangle(GLuint llcorner, GLuint lrcorner, GLuint trcorner, GLuint tlcorner);



    QVector<GLfloat> m_data;
    int m_count;
    QVector<GLuint> pointOrdering;
    int pointCount;
};

#endif // VERTEXTRANSLATOR_H


