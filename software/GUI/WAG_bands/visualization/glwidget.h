#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "modelglloader.h"
#include "model/model.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT

    public:
        GLWidget(Model* m);
        ~GLWidget();

    protected:
        void initializeGL() Q_DECL_OVERRIDE;
        void paintGL() Q_DECL_OVERRIDE;
        void resizeGL(int width, int height) Q_DECL_OVERRIDE;
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    private:
        void createShaderProgram( QString vShader, QString fShader);
        void createBuffers();
        void createAttributes();
        void setupLightingAndMatrices();

        void setXRotation(int angle);
        void setZRotation(int angle);

//        void draw();
        void drawNodes();
        void setMaterialUniforms(Material &mater);

        QOpenGLShaderProgram m_shaderProgram;

        QOpenGLVertexArrayObject m_vao;

        QOpenGLBuffer m_vertexBuffer;
        QOpenGLBuffer m_normalBuffer;
        QOpenGLBuffer m_indexBuffer;

        QMatrix4x4 m_projection, m_view, m_model;

        QString m_filepath;
        ModelGLLoader::PathType m_pathType;

        LightInfo m_lightInfo;

        bool m_error;

        int m_xRot;
        int m_yRot;
        int m_zRot;
        QPoint m_lastPos;

        QVector3D m_cam_offset;

        ModelGL* modelGL;
        Model* model;
    };

    #endif
