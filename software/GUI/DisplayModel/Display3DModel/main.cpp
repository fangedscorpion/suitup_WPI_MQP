#include <QApplication>
#include "openglwindow.h"
#include "modelwindow_gl_3_3.h"
#include "modelwindow_gl_2.h"

//QString OpenGL_Model("re3dmodel/velociraptor_mesh_materials.dae");
//QString OpenGL_Model("demon_head/demon_head.3ds");
QString OpenGL_Model("biped/biped_rig.obj");

// OpenGL ES 2.0, a single vertex buffer can have at max std::numeric_limits<unsigned short>::max() vertices,
// so use a smaller model if rendering with OpenGL ES
QString OpenGLES_Model("demon_head/demon_head.3ds");

QOpenGLContext *createOpenGLContext(int major, int minor) {
    QSurfaceFormat requestedFormat;
    requestedFormat.setDepthBufferSize( 24 );
    requestedFormat.setMajorVersion( major );
    requestedFormat.setMinorVersion( minor );

    requestedFormat.setSamples( 4 );
    requestedFormat.setProfile( QSurfaceFormat::CoreProfile );

    QOpenGLContext *context = new QOpenGLContext;
    context->setFormat( requestedFormat );
    context->create();

    // If unable to get OpenGL 3.3 context, fall back to OpenGL 2.1/OpenGL ES
    if(context->format().version() != qMakePair(major,minor)) {
        qDebug() << "Unable to get OpenGL context version" << QString(QString::number(major) + "." + QString::number(minor));
        context->deleteLater();
        requestedFormat.setMajorVersion(2);
        requestedFormat.setMinorVersion(1);
        context = new QOpenGLContext;
        context->setFormat( requestedFormat );
        context->create();

        // OpenGL versions < 2 is not supported
        if (context->format().version().first < 2) {
            context->deleteLater();
            context = 0;
        }
    }

    return context;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QOpenGLContext *context = createOpenGLContext(3, 3);

    if (!context)
        return 1;

    QPair<int, int> glVersion = context->format().version();
    bool isOpenGLES = context->format().renderableType() == QSurfaceFormat::OpenGLES;

    OpenGLWindow *modelWindow = 0;

    qDebug() << "OpenGL Version:" << glVersion << (isOpenGLES ? "ES" : "Desktop");

    if (glVersion == qMakePair(3,3) && !isOpenGLES) {
        modelWindow = new ModelWindow_GL_3_3(OpenGL_Model, ModelLoader::RelativePath);//new Scene(OpenGL_Model, ModelLoader::RelativePath);
    }
    // OpenGL ES 2 and desktop OpenGL 2.1 implementations are very similar,
    // there are a few small differences commented in the code,
    // and we use a smaller model for OpenGL ES
    else if (isOpenGLES) {
        modelWindow = new ModelWindow_GL_2(OpenGLES_Model, ModelLoader::RelativePath);
    }
    else {
        modelWindow = new ModelWindow_GL_2(OpenGL_Model, ModelLoader::RelativePath);
    }

    modelWindow->initializeWindow(context, 40);
    modelWindow->show();

    return a.exec();
}
