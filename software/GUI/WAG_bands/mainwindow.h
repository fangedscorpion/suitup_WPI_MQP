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
#include <QPainter>
#include <QStackedWidget>
#include <QGraphicsView>
#include "superslider.h"
#include "overlay.h"
#include "overlaywidget.h"
#include "playbackcontroller.h"

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
    void resizeEvent(QResizeEvent* r);

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
    OverlayWidget *ow;
    Overlay *overlay;
    QStackedWidget *widget;
    QLabel *filename;
    GLWidget *viewer;
    SuperSlider *videoSlider;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon recordIcon;
    QLabel *handle1Time;
    QLabel *handle2Time;

    // settings
    QGraphicsView *view;
    QCheckBox *voiceControl;
    QCheckBox *leftShoulder;
    QCheckBox *leftUpperArm;
    QCheckBox *leftLowerArm;
    QCheckBox *leftHand;
    QCheckBox *rightShoulder;
    QCheckBox *rightUpperArm;
    QCheckBox *rightLowerArm;
    QCheckBox *rightHand;
    QPushButton *ok;
    QPushButton *cancel;
    QPushButton *calibrate;
    QPushButton *connectBands;


    QPushButton *playPause;
    QLabel *sfi;
    QLabel *minSpeed;
    QLabel *midSpeed;
    QLabel *maxSpeed;
    PlaybackController *playbackControls;


private slots:
//    void newFile();
//    void open();
//    void save();
//    void saveAs();
    void playbackMode();
    void recordMode();
    void openSettings();
    void cancelSettings();
    void saveSettings();
    void updateSpeedSliderText(QString playbackModeString);
//    void help();
signals:
    void resizedWindow();
};

#endif // MAINWINDOW_H
