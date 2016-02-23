#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

class QSlider;
class QPushButton;

class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);

private slots:
//    void dockUndock();

private:
    QSlider *createSlider();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *zSlider;
    QPushButton *dockBtn;
    MainWindow *mainWindow;
};

#endif
