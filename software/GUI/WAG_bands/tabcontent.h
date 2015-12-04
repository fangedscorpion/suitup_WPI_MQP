#pragma once
#include <QWidget>
#include "mainwindow.h"

class GLWidget;

class TabContent : public QWidget
{
    Q_OBJECT

public:
    TabContent(QString filename, ACTION_TYPE t);
    ~TabContent();

    QString getFilename() { return filenameString;}
    void updateWithNewFilename(QString f);

private:
    QWidget* createModeRadios();
    QWidget* createPlaybackOptionsAndControls();
    QWidget* createEditOptionsAndControls();
    QWidget* createRecordOptionsAndController();
    QWidget* createViewer(ACTION_TYPE t);
    QWidget* createFileInfoGroup();
    QStackedWidget *optionsStack;

    // fonts & styles
    QFont titleFont;
    QString titleStyleSheet;

    QString filenameString;

    // mode radio buttons
    QGroupBox *modeRadiosGroup;
    QRadioButton *playbackRadio;
    QRadioButton *editRadio;

    // playback recording options

    QCheckBox *playOnSuit;
    QComboBox *stepThrough;
    QSlider *speedSlider;
    QSpinBox *seconds;
    QSlider *toleranceSlider;

    // viewer
    QStackedWidget *viewerStack;
    QGroupBox *viewerGroup;
    QLabel *currentLoadedFilename;
    GLWidget *viewer;
    SuperSlider *videoSlider;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon recordIcon;
    QLabel *handle1Time;
    QLabel *handle2Time;

    // recording
    QGroupBox *recordGroup;
    QLabel *recordMessage;
    QPushButton *recordButton;
    QPushButton *edit;

    QPushButton *playPause;
    QLabel *sfi;
    QLabel *minSpeed;
    QLabel *midSpeed;
    QLabel *maxSpeed;
    PlaybackController *playbackControls;

public slots:
    void showPlaybackMode();
    void showEditMode();
    void showRecordMode();
    void updateSpeedSliderText(QString playbackModeString);
    void applicationResized();
    void handleRecordingButton();
    void displayNewTime(int newMillis);
    void playbackToggled(bool playing);
};
