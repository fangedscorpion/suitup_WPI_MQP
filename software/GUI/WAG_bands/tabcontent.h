#pragma once
#include <QWidget>
#include "mainwindow.h"
#include "customWidgets/smartradiobutton.h"
#include "editingcontroller.h"

class GLWidget;

class TabContent : public QWidget
{
    Q_OBJECT

public:
    TabContent(MainWindow* parent, WAGFile* in_motion, USER u, ACTION_TYPE initiallyShow);
    ~TabContent();

    QString getFilename() { return motion->getName();}
    bool isVoiceControlOn() { return recordVoiceControl->isChecked(); }
    void setVoiceControl(bool b) { recordVoiceControl->setChecked(b); }

private:
    MainWindow* parent;
    USER user;
    WAGFile* motion;
    QStackedWidget *optionsStack;
    void createIcons();
    void resizeEvent(QResizeEvent* r);
    Overlay* overlay;
    void updateMotion(WAGFile* file);

    // fonts & styles
    QFont titleFont;
    QString groupboxStyleSheet;
    QString textInputStyleWhite;
    QString textInputStyleRed;
    int buttonHeight;
    // mode radio buttons
    QGroupBox *modeRadiosGroup;
    smartRadioButton *playbackRadio;
    smartRadioButton *editRadio;
    smartRadioButton *recordRadio;
    QWidget* createModeRadios(USER u);
    // playback options
    QCheckBox *playOnSuit;
    QComboBox *stepThrough;
    QSlider *speedSlider;
    QSpinBox *secondsToHold;
    QSlider *toleranceSlider;
    QGroupBox *playbackOptions;
    QTimer *playbackCountdownTimer;
    QLabel *playbackCountdownTime;
    QDoubleSpinBox *playbackCountDownSpinner;
    QLabel *playbackCountDownTitleLabel;
    QLabel *playbackCountdownSecondsTitleLabel;
    QCheckBox* playbackVoiceControl;
    void playbackResetCountDownTimer();
    // playback controls
    QPushButton *playPause;
    QLabel *sfi;
    QLabel *minSpeed;
    QLabel *midSpeed;
    QLabel *maxSpeed;
    PlaybackController *playbackControls;
    EditingController *editingControls;
    QWidget* createPlaybackOptionsAndControls();
    void lockOnPlayback(bool playing);
    void switchStepThroughMode(bool steppingThrough);
    void initializePlaybackSettings();
    // viewer
    QStackedWidget *viewerStack;
    QLabel *currentLoadedFilename;
    GLWidget *viewer;
    SuperSlider *videoSlider;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon recordIcon;
    QLabel *handle1Time;
    QLabel *handle2Time;
    QWidget* createViewer(ACTION_TYPE t);
    // recording
    QGroupBox *recordGroup;
    QLabel *recordMessage;
    QIcon stopIcon;
    QCheckBox* recordVoiceControl;
    QDoubleSpinBox *recordCountDownSpinner;
    QWidget* createRecordOptionsAndController();
    QWidget* createRecordingWindow();
    // recording viewer
    QLabel *recordCountdownTime;
    QLabel *recordCountDownTitleLabel;
    QLabel *recordStopwatchTitleLabel;
    QLabel *recordCountdownSecondsTitleLabel;
    QLabel *recordStopwatchMinutesTitleLabel;
    QPushButton *recordButton;
    QPushButton *resetButton;
    void recordResetCountDownTimer();
    void handleRecordTimeCounter();
    QTimer *recordCountdownTimer;
    QTimer *recordStopwatchTimer;
    int msecs;
    // edit
    QIcon cropIcon;
    QIcon splitIcon;
    QIcon undoIcon;
    QIcon editIcon;
    QIcon resetIcon;
    QWidget* createEditOptionsAndControls();
    // file info
    OverlayWidget *motionInfoWidget;
    void createMotionInfoWindow();
    QPushButton* addTagBtn;
    smartPushButton* saveMotionInfoBtn;
    QLabel* newFileTagsLabel;
    QLineEdit* infoMotionNameTextEdit;
    QTextEdit* infoMotionDescription;
    QLineEdit* infoMotionTagsTextEdit;
    QLabel* infoMotionTagsLabel;

public slots:
    void show(ACTION_TYPE a);
    void updateSpeedSliderText(QString playbackModeString);
    void applicationResized();
    void handleRecordingWindowButtons();
    void recordDisplayNewTime(int newMillis);
    void playbackToggled(bool playing);
    void editPlayToggled(bool playing);

    void sliderValueChanged(int newVal);
    void launchMotionInfo();
    void closeMotionInfo();
    void saveMotionInfo();
    // validate input
    void handleNewMotionRequiredInput();
    void handleNewMotionRequiredInput(QString);
    // timers
    void recordCountdownTimerEvent();
    void recordStopwatchTimerEvent();
    void playbackCountdownTimerEvent();
    void recordSetCountDownTimer(double d);
    void playbackSetCountDownTimer(double d);
    // save
    void saveMotion();

signals:
    void stepThroughChanged(bool steppingThrough);
    void countDownChanged(double d);
    void resizedWindow();
};
