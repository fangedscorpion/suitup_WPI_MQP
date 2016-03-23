#pragma once
#include <QWidget>
#include "mainwindow.h"
#include "customWidgets/smartradiobutton.h"
#include "editingcontroller.h"
#include "customWidgets/motionviewer.h"
#include "band/suit.h"
#include "model/modelloader.h"
#include "recordingcontroller.h"
#include "customWidgets/styledgroupbox.h"
#include "customWidgets/styledcheckbox.h"

class GLWidget;

class TabContent : public QWidget
{
    Q_OBJECT

public:
    TabContent(QPointer<MainWindow> parent, QPointer<WAGFile> in_motion, USER u, ACTION_TYPE initiallyShow, Suit *sysSuit, ModelLoader *modelLoader);
    ~TabContent();

    QString getFilename() { return motion->getName();}

    ACTION_TYPE getCurrentMode();

private:
    QPointer<MainWindow> parent;
    USER user;
    QPointer<WAGFile> motion;
    QPointer<QStackedWidget> optionsStack;
    ACTION_TYPE currentMode;
    void createIcons();
    void resizeEvent(QResizeEvent* r);
    QPointer<Overlay> overlay;
    void updateMotion();
    QPointer<Suit> suitObj;
    ModelLoader* modelLoader;

    // models
    QPointer<Model> playbackModel;
    QPointer<Model> editModel;
    // fonts & styles
    QString textInputStyleWhite;
    QString textInputStyleRed;
    // mode radio buttons
    QPointer<StyledGroupBox> modeRadiosGroup;
    QPointer<SmartRadioButton> playbackRadio;
    QPointer<SmartRadioButton> editRadio;
    QPointer<SmartRadioButton> recordRadio;
    QPointer<StyledGroupBox> createModeRadios(USER u);
    // playback options
    QPointer<StyledCheckBox> playOnSuit;
    QPointer<QComboBox> stepThrough;
    QPointer<QSlider> speedSlider;
    QPointer<StyledGroupBox> playbackOptions;
    QPointer<QTimer> playbackCountdownTimer;
    QPointer<QLabel> playbackCountdownTime;
    void playbackResetCountDownTimer();
    // playback viewer and controls
    QPointer<StyledGroupBox> createViewer(ACTION_TYPE t);
    QPointer<StyledGroupBox> createPlaybackOptionsAndControls();
    QPointer<QLabel> sfi;
    QPointer<QLabel> minSpeed;
    QPointer<QLabel> midSpeed;
    QPointer<QLabel> maxSpeed;
    QPointer<QStackedWidget> viewerStack;
    QPointer<MotionViewer> playbackMotionViewer;
    QPointer<PlaybackController> playbackControls;
    QPointer<EditingController> editingControls;
    QPointer<RecordingController> recordingControls;
    QPointer<QDoubleSpinBox> playbackCountDownSpinner;
    void lockOnPlayback(bool playing);
    void switchStepThroughMode(bool steppingThrough);
    void initializePlaybackSettings();
    // recording
    QPointer<StyledGroupBox> recordGroup;
    QIcon stopIcon;
    QIcon recordIcon;
    QPointer<QDoubleSpinBox> recordCountDownSpinner;
    QPointer<StyledGroupBox> recordOptionsGroup;
    QPointer<StyledGroupBox> createRecordOptionsAndController();
    QPointer<StyledGroupBox> createRecordingWindow();
    // recording viewer
    QPointer<QLabel> recordCountdownTime;
    QPointer<QLabel> recordCountDownTitleLabel;
    QPointer<QLabel> recordStopwatchTitleLabel;
    QPointer<QLabel> recordCountdownSecondsTitleLabel;
    QPointer<QLabel> recordStopwatchMinutesTitleLabel;
    QPointer<SmartPushButton> recordButton;
    QPointer<SmartPushButton> resetButton;
    void recordResetCountDownTimer();
    void handleRecordTimeCounter();
    QPointer<QTimer> recordCountdownTimer;
    QPointer<QTimer> recordStopwatchTimer;
    int msecs;
    // edit
    QIcon cropIcon;
    QIcon splitIcon;
    QIcon undoIcon;
    QIcon editIcon;
    QIcon resetIcon;
    QPointer<StyledGroupBox> createEditOptionsAndControls();
    QPointer<MotionViewer> editMotionViewer;
    // file info
    QPointer<OverlayWidget> motionInfoWidget;
    void createMotionInfoWindow();
    QPointer<SmartPushButton> addTagBtn;
    QPointer<SmartPushButton> saveMotionInfoBtn;
    QPointer<QLineEdit> infoMotionNameTextEdit;
    QPointer<QTextEdit> infoMotionDescription;
    QPointer<QLineEdit> infoMotionTagsTextEdit;
    QPointer<QHBoxLayout> infoMotionTagsLayout;
    QPointer<SmartRadioButton> infoMotionCompRadio;
    QPointer<QLabel> infoMotionSaveLocation;
    QPointer<SmartPushButton> infoMotionBrowseBtn;
    void changeRecordingState(bool shouldRecord);

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
    void handleVCChangeState(bool shouldRecord);

signals:
    void stepThroughChanged(bool steppingThrough);
    void modeChanged(ACTION_TYPE);
};
