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
#include <QLineEdit>
#include <QRadioButton>
#include <QFileDialog>
#include <QTextEdit>
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
    void createSettings();
    void createPlaybackOptionsAndControls();
    void createEditRecordingOptionsAndControls();
    void createViewer();
    void createSaveAs();

    QMenu *fileMenu;
    QMenu *modeMenu;
    QMenu *helpMenu;
    GLWidget *glWidget;

    QAction *newAct;
    QAction *openFromCompAct;
    QAction *openFromLibAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *exitAct;
    QAction *settingsAct;
    QAction *playbackAct;
    QAction *recordAct;
    QAction *helpAct;

    QStackedWidget *playbackEditOptions;

    // edit recording options
    QPushButton *undoBtn;
    QPushButton *cropBtn;
    QPushButton *splitBtn;

    // playback recording options
    QCheckBox *playOnSuit;
    QComboBox *stepThrough;
    QSlider *speedSlider;
    QSpinBox *seconds;
    QSlider *toleranceSlider;

    // viewer window
    QVBoxLayout *viewerPane;
    OverlayWidget *settingsWidget;
    Overlay *overlay;
    QWidget *mainWidget;
    QLabel *currentLoadedFilename;
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

    // save as
    OverlayWidget *saveAsWidget;
    QLineEdit *saveAsFilename;
    QLabel *tags;
    QLineEdit *inputTags;
    QTextEdit *description;
    QString filename;

    QPushButton *playPause;
    QLabel *sfi;
    QLabel *minSpeed;
    QLabel *midSpeed;
    QLabel *maxSpeed;
    PlaybackController *playbackControls;


private slots:
//    void newFile();
    void openFromComputer();
    void save();
    void playbackMode();
    void recordMode();
    void openSettings();
    void closeSettings();
    void saveSettings();
    void updateSpeedSliderText(QString playbackModeString);
    void openSaveAs();
    void saveSaveAs();
    void closeSaveAs();
    void saveToComputer();
    void addTag();

//    void help();
signals:
    void resizedWindow();
};

#endif // MAINWINDOW_H
