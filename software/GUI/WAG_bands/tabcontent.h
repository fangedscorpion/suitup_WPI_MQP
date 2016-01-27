#pragma once
#include <QWidget>
#include "mainwindow.h"
#include "smartradiobutton.h"
#include "editingcontroller.h"

class GLWidget;

class TabContent : public QWidget
{
    Q_OBJECT

public:
    TabContent(MainWindow* parent, WAGFile* in_motion, USER u, ACTION_TYPE initiallyShow);
    TabContent(MainWindow* parent, std::vector<USER> users);
    ~TabContent();

    QString getFilename() { return motion->getName();}
    bool isVoiceControlOn() { return voiceControl->isChecked(); }
    void setVoiceControl(bool b) { voiceControl->setChecked(b); }

protected:
    void timerEvent(QTimerEvent *event);

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
    QIcon resetIcon;
    QWidget* createEditOptionsAndControls();
    // file info
    OverlayWidget *fileInfoWidget;
    void createFileInfoWindow();
    QPushButton* addTagBtn;
    QLabel* newFileTagsLabel;
    QLineEdit* infoFilenameTextEdit;
    QTextEdit* infoFileDescription;
    QLineEdit* infoFileTagsTextEdit;
    QLabel* infoFileTagsLabel;

public slots:
    void show(ACTION_TYPE a);
    void updateSpeedSliderText(QString playbackModeString);
    void setCountDownTimer(double d);
    void applicationResized();
    void handleRecordingWindowButtons();
    void displayNewTime(int newMillis);
    void playbackToggled(bool playing);
    void editPlayToggled(bool playing);

    void sliderValueChanged(int newVal);
    void launchFileInfo();
    void closeFileInfo();
    void saveFileInfo();

signals:
    void stepThroughChanged(bool steppingThrough);
    void countDownChanged(double d);
    void resizedWindow();
};
