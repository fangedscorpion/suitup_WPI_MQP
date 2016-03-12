#include "glwidget.h"
#include "window.h"
#include <QSlider>
#include <QHBoxLayout>

Window::Window() {
    glWidget = new GLWidget();

    QHBoxLayout *container = new QHBoxLayout;
    container->addWidget(glWidget);

    setLayout(container);
}
