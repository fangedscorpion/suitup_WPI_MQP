#pragma once
#include <QWidget>
#include "mainwindow.h"
#include "smartradiobutton.h"

class GLWidget;

class TabContent : public QWidget
{
    Q_OBJECT

public:
    TabContent(MainWindow* parent, QString filename, USER u, ACTION_TYPE initiallyShow);
    ~TabContent();

    QString getFilename() { return filenameString;}
    void updateWithNewFilename(QString f);

protected:
    void timerEvent(QTimerEvent *event);

private:
    MainWindow* parent;
    USER user;
    QStackedWidget *optionsStack;
    QString filenameString;
    void createIcons();

    // fonts & styles
    QFont titleFont;
    QString groupboxStyleSheet;

    // mode radio buttons
    QGroupBox *modeRadiosGroup;
    smartRadioButton *playbackRadio;
    smartRadioButton *editRadio;
    smartRadioButton *recordRadio;
    QWidget* createModeRadios(USER u);
    // playback recording options
    QCheckBox *playOnSuit;
    QComboBox *stepThrough;
    QSlider *speedSlider;
    QSpinBox *secondsToHold;
    QSlider *toleranceSlider;
    QGroupBox *playbackOptions;
    // playback controls
    QPushButton *playPause;
    QLabel *sfi;
    QLabel *minSpeed;
    QLabel *midSpeed;
    QLabel *maxSpeed;
    PlaybackController *playbackControls;
    QWidget* createPlaybackOptionsAndControls();
    void lockOnPlayback(bool playing);
    void switchStepThroughMode(bool steppingThrough);
    void initializePlaybackSettings();
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
    QWidget* createViewer(ACTION_TYPE t);
    // recording viewer
    QLabel *time;
    QLabel *countDownLabel;
    QLabel *recordTime;
    QLabel *seconds;
    QLabel *minutes;
    QPushButton *recordButton;
    QPushButton *resetButton;
    void resetCountDownTimer();
    void handleCountDown();
    void handleRecordTimeCounter();
    int countDownTimerID;
    int recordTimeTimerID;
    int msecs;
    // recording
    QGroupBox *recordGroup;
    QLabel *recordMessage;
    QIcon stopIcon;
    QCheckBox* voiceControl;
    QDoubleSpinBox *countDownSpinner;
    QWidget* createRecordOptionsAndController();
    QWidget* createRecordingWindow();
    // edit
    QIcon cropIcon;
    QIcon splitIcon;
    QIcon undoIcon;
    QIcon editIcon;
    QWidget* createEditOptionsAndControls();

public slots:
    void show(ACTION_TYPE a);
    void updateSpeedSliderText(QString playbackModeString);
    void setCountDownTimer(double d);
    void applicationResized();
    void handleRecordingWindowButtons();
    void displayNewTime(int newMillis);
    void playbackToggled(bool playing);
    void sliderValueChanged(int newVal);

signals:
    void stepThroughChanged(bool steppingThrough);
    void countDownChanged(double d);
};
