#pragma once
#include <QWidget>
#include "mainwindow.h"

class GLWidget;

class TabContent : public QWidget
{
    Q_OBJECT

public:
    TabContent(MainWindow* parent, QString filename, ACTION_TYPE t);
    ~TabContent();

    QString getFilename() { return filenameString;}
    void updateWithNewFilename(QString f);

private:
    MainWindow* parent;
    QWidget* createModeRadios();
    QWidget* createPlaybackOptionsAndControls();
    QWidget* createEditOptionsAndControls();
    QWidget* createRecordOptionsAndController();
    QWidget* createViewer(ACTION_TYPE t);
    QWidget* createFileInfoGroup();
    void createIcons();
    QStackedWidget *optionsStack;

    // fonts & styles
    QFont titleFont;
    QString titleStyleSheet;

    QString filenameString;

    // mode radio buttons
    QGroupBox *modeRadiosGroup;
    QRadioButton *playbackRadio;
    QRadioButton *editRadio;
    QRadioButton *recordRadio;

    // playback recording options

    QCheckBox *playOnSuit;
    QComboBox *stepThrough;
    QSlider *speedSlider;
    QSpinBox *seconds;
    QSlider *toleranceSlider;
    QGroupBox *playbackOptions;

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
    QIcon stopIcon;

    // edit
    QIcon cropIcon;
    QIcon splitIcon;
    QIcon undoIcon;
    QIcon editIcon;

    QPushButton *playPause;
    QLabel *sfi;
    QLabel *minSpeed;
    QLabel *midSpeed;
    QLabel *maxSpeed;
    PlaybackController *playbackControls;


    void lockOnPlayback(bool playing);
    void switchStepThroughMode(bool steppingThrough);
    void initializePlaybackSettings();


public slots:
    void showPlaybackMode();
    void showEditMode();
    void showRecordMode();
    void updateSpeedSliderText(QString playbackModeString);
    void applicationResized();
    void handleRecordingButton();
    void displayNewTime(int newMillis);
    void playbackToggled(bool playing);

signals:
    void stepThroughChanged(bool steppingThrough);
};
