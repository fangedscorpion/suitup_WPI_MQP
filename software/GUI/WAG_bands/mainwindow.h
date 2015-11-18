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
#include "superslider.h"

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
    void resizeEvent(QResizeEvent* r);

    QMenu *fileMenu;
    QMenu *modeMenu;
    QMenu *helpMenu;

    QAction *newAct;
    QAction *openAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *settingsAct;
    QAction *playbackAct;
    QAction *recordAct;
    QAction *helpAct;

    // edit recording options
    QWidget *editOptions;
    QPushButton *undoBtn;
    QPushButton *cropBtn;
    QPushButton *splitBtn;

    // playback recording options
    QWidget *playbackOptions;
    QCheckBox *playOnSuit;
    QComboBox *stepThrough;
    QSlider *speedSlider;
    QSpinBox *seconds;
    QSlider *toleranceSlider;

    // viewer window
    QLabel *filename;
    QOpenGLWidget *viewer;
    QPushButton *playPause;
    SuperSlider *videoSlider;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon recordIcon;
    QLabel *handle1Time;
    QLabel *handle2Time;

private slots:
//    void newFile();
//    void open();
//    void save();
//    void saveAs();
    void playbackMode();
    void recordMode();
//    void settings();
//    void help();
signals:
    void resizedWindow();
};

#endif // MAINWINDOW_H
