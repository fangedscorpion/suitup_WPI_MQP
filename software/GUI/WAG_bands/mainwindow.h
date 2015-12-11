#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QAction>
#include <QOpenGLWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSpinBox>

class GLWidget;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void createActions(QMenuBar *menu);

    QMenu *fileMenu;
    QMenu *modeMenu;
    QMenu *helpMenu;
    GLWidget *glWidget;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *settingsAct;
    QAction *playbackAct;
    QAction *recordAct;
    QAction *helpAct;

    QWidget *editOptions;
    QWidget *playbackOptions;

private slots:
//    void newFile();
//    void open();
//    void save();
//    void saveAs();
    void playbackMode();
    void recordMode();
//    void settings();
//    void help();
};

#endif // MAINWINDOW_H
