#include <QApplication>
#include "openglwindow.h"
#include "modelwindow_gl.h"

QString OpenGL_Model("biped/biped_rig.3ds");

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set up opengl format and context
    QSurfaceFormat requestedFormat;
    requestedFormat.setDepthBufferSize(24);
    requestedFormat.setMajorVersion(3);
    requestedFormat.setMinorVersion(1);
    requestedFormat.setSamples(4);
    requestedFormat.setProfile(QSurfaceFormat::CoreProfile );
    QOpenGLContext *context = new QOpenGLContext;
    context->setFormat(requestedFormat);
    context->create();

    // create window
    OpenGLWindow* modelWindow = new ModelWindow_GL(OpenGL_Model, ModelLoader::RelativePath);
    modelWindow->initializeWindow(context, 40);
    modelWindow->show();

    return a.exec();
}
