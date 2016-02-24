#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QMatrix4x4>
#include "modelloader.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
    {
        Q_OBJECT

    public:
        GLWidget(QString filepath, ModelLoader::PathType pathType /*QString texturePath, QWidget *parent = 0*/);
        ~GLWidget();

    public slots:
        void setXRotation(int angle);
        void setZRotation(int angle);
        void cleanup();

    signals:
        void xRotationChanged(int angle);
        void zRotationChanged(int angle);

    protected:
        void initializeGL() Q_DECL_OVERRIDE;
        void paintGL() Q_DECL_OVERRIDE;
        void resizeGL(int width, int height) Q_DECL_OVERRIDE;
        void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
        void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;

    private:
//        void setupVertexAttribs();

        void createShaderProgram( QString vShader, QString fShader);
        void createBuffers();
        void createAttributes();
        void setupLightingAndMatrices();

        void draw();
        void drawNode(const Node *node, QMatrix4x4 objectMatrix);
        void setMaterialUniforms(MaterialInfo &mater);

        QOpenGLShaderProgram m_shaderProgram;

        QOpenGLVertexArrayObject m_vao;

        QOpenGLBuffer m_vertexBuffer;
        QOpenGLBuffer m_normalBuffer;
        QOpenGLBuffer m_textureUVBuffer;
        QOpenGLBuffer m_indexBuffer;

        QSharedPointer<Node> m_rootNode;

        QMatrix4x4 m_projection, m_view, m_model;

        QString m_filepath;
        ModelLoader::PathType m_pathType;
        QString m_texturePath;

        LightInfo m_lightInfo;
        MaterialInfo m_materialInfo;

        bool m_error;

        int m_xRot;
        int m_yRot;
        int m_zRot;
        QPoint m_lastPos;


//        Logo m_logo;
//        QOpenGLVertexArrayObject m_vao;
//        QOpenGLBuffer m_logoVbo;
//        QOpenGLShaderProgram *m_program;
//        int m_projMatrixLoc;
//        int m_mvMatrixLoc;
//        int m_normalMatrixLoc;
//        int m_lightPosLoc;
//        QMatrix4x4 m_proj;
//        QMatrix4x4 m_camera;
//        QMatrix4x4 m_world;
    };

    #endif
