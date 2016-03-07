#pragma once
#include <QWidget>
#include "mainwindow.h"
#include "customWidgets/smartradiobutton.h"
#include "editingcontroller.h"
#include "customWidgets/motionviewer.h"
#include "band/suit.h"
#include "recordingcontroller.h"

class GLWidget;

class TabContent : public QWidget
{
    Q_OBJECT

public:
    TabContent(MainWindow* parent, WAGFile* in_motion, USER u, ACTION_TYPE initiallyShow, Suit *sysSuit);
    ~TabContent();

    QString getFilename() { return motion->getName();}

    ACTION_TYPE getCurrentMode();

private:
    MainWindow* parent;
    USER user;
    WAGFile* motion;
    QStackedWidget *optionsStack;
    ACTION_TYPE currentMode;
    void createIcons();
    void resizeEvent(QResizeEvent* r);
    Overlay* overlay;
    void updateMotion();
    Suit *suitObj;

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
    QGroupBox *playbackOptions;
    QTimer *playbackCountdownTimer;
    QLabel *playbackCountdownTime;
    void playbackResetCountDownTimer();
    // playback viewer and controls
    QWidget* createViewer(ACTION_TYPE t);
    QWidget* createPlaybackOptionsAndControls();
    QLabel *sfi;
    QLabel *minSpeed;
    QLabel *midSpeed;
    QLabel *maxSpeed;
    QStackedWidget *viewerStack;
    MotionViewer* playbackMotionViewer;
    PlaybackController *playbackControls;
    EditingController *editingControls;
    RecordingController *recordingControls;
    QDoubleSpinBox *playbackCountDownSpinner;
    void lockOnPlayback(bool playing);
    void switchStepThroughMode(bool steppingThrough);
    void initializePlaybackSettings();
    // recording
    QGroupBox *recordGroup;
    QIcon stopIcon;
    QIcon recordIcon;
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
    MotionViewer* editMotionViewer;
    // file info
    OverlayWidget *motionInfoWidget;
    void createMotionInfoWindow();
    QPushButton* addTagBtn;
    smartPushButton* saveMotionInfoBtn;
    QLineEdit* infoMotionNameTextEdit;
    QTextEdit* infoMotionDescription;
    QLineEdit* infoMotionTagsTextEdit;
    QHBoxLayout* infoMotionTagsLayout;
    QRadioButton *infoMotionCompRadio;
    QLabel *infoMotionSaveLocation;
    QPushButton *infoMotionBrowseBtn;

public slots:
    void show(ACTION_TYPE a);
    void updateSpeedSliderText(QString playbackModeString);
    void handleRecordingWindowButtons();
    void catchModeChanged(ACTION_TYPE newMode);
    void sliderValueChanged(int newVal);
    void launchMotionInfo();
    void closeMotionInfo();
    void saveMotionInfo();
    // validate input
    void handleInfoMotionRequiredInput();
    void handleInfoMotionRequiredInput(QString);
    void addTag();
    // timers
    void recordCountdownTimerEvent();
    void recordStopwatchTimerEvent();
    void playbackCountdownTimerEvent();
    void recordSetCountDownTimer(double d);
    void playbackSetCountDownTimer(double d);
    // save
    void saveMotion();
    // motion info
    void handleInfoMotionRadios();
    void catchCurrentFrameChange(int newSliderPos);
    void lockOnSuitPlayback(bool playingOnSuit);
    void playToggled(bool playing);

signals:
    void stepThroughChanged(bool steppingThrough);
    void modeChanged(ACTION_TYPE);
};
