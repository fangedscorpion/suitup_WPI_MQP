#include "tabcontent.h"

#define RECORD_STOPWATCH_INTERVAL 100

QPointer<StyledGroupBox> TabContent::createRecordOptionsAndController() {
    recordingControls = new RecordingController(suitObj);
    recordingControls->setActiveMotion(motion);
    recordOptionsGroup = new StyledGroupBox("Recording Options");

    recordCountDownSpinner = new QDoubleSpinBox;
    recordCountDownSpinner->setMinimum(0.5);
    recordCountDownSpinner->setValue(5);
    recordCountDownSpinner->setMaximum(30); // consider changing
    recordCountDownSpinner->setDecimals(1);
    recordCountDownSpinner->setSingleStep(.5);
    recordCountDownSpinner->setMaximumWidth(60);
    QPointer<QLabel> countDownPreMessage = new QLabel("Count down for");
    QPointer<QLabel> countDownMessage = new QLabel("sec(s)");
    QPointer<QHBoxLayout> h = new QHBoxLayout;
    h->addWidget(countDownPreMessage);
    h->addWidget(recordCountDownSpinner);
    h->addWidget(countDownMessage);

    QPointer<StyledCheckBox> recordVoiceControl = new StyledCheckBox("Voice Control");

    QPointer<QVBoxLayout> options = recordOptionsGroup->getLayout();
    options->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    options->addWidget(recordVoiceControl);
    options->addLayout(h);
    options->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    recordOptionsGroup->setLayout(options);

    connect(recordCountDownSpinner, SIGNAL(valueChanged(double)), this, SLOT(recordSetCountDownTimer(double)));
    connect(recordVoiceControl, SIGNAL(toggled(bool)), recordingControls, SLOT(toggleVoiceControl(bool)));
    return recordOptionsGroup;
}

// The right panel under record motion
QPointer<StyledGroupBox> TabContent::createRecordingWindow() {
    recordGroup = new StyledGroupBox("Recording: " + motion->getName());
    // viewer window

    QFont bigFont = QFont("Arial", 25);
    QFont timeFont = QFont("Arial", 50);
    recordCountDownTitleLabel = new QLabel("Count Down");
    recordCountDownTitleLabel->setFont(bigFont);
    recordCountDownTitleLabel->setAlignment(Qt::AlignCenter);
    recordStopwatchTitleLabel = new QLabel("Record Time");
    recordStopwatchTitleLabel->setFont(bigFont);
    recordStopwatchTitleLabel->setAlignment(Qt::AlignCenter);
    recordStopwatchTitleLabel->hide();
    recordCountdownTime = new QLabel(QString::number(recordCountDownSpinner->value(), 'f', 1));
    recordCountdownTime->setFont(timeFont);
    recordCountdownTime->setAlignment(Qt::AlignBottom);
    recordCountdownSecondsTitleLabel = new QLabel("sec(s)");
    recordCountdownSecondsTitleLabel->setFont(bigFont);
    recordCountdownSecondsTitleLabel->setAlignment(Qt::AlignBottom);
    recordStopwatchMinutesTitleLabel = new QLabel("min(s)");
    recordStopwatchMinutesTitleLabel->setFont(bigFont);
    recordStopwatchMinutesTitleLabel->setAlignment(Qt::AlignBottom);
    recordStopwatchMinutesTitleLabel->hide();
    QPointer<QHBoxLayout> h = new QHBoxLayout;
    h->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    h->addWidget(recordCountdownTime);
    h->addWidget(recordCountdownSecondsTitleLabel);
    h->addWidget(recordStopwatchMinutesTitleLabel);
    h->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    resetButton = new SmartPushButton("Reset");
    resetButton->setEnabled(false);
    resetButton->setIcon(resetIcon);
    resetButton->setIconSize(QSize(35,30));

    recordButton = new SmartPushButton("Start Recording");
    recordButton->setIcon(recordIcon);
    recordButton->setIconSize(QSize(35,15));

    // viewer side of the GUI
    QPointer<QVBoxLayout> info = recordGroup->getLayout();
    info->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QPointer<QHBoxLayout> b = new QHBoxLayout;
    b->addWidget(resetButton, 1);
    b->addWidget(recordButton, 2);
    info->addLayout(b);
    info->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    info->addLayout(h);
    info->addWidget(recordCountDownTitleLabel);
    info->addWidget(recordStopwatchTitleLabel);

    connect(recordButton, SIGNAL(released()), this, SLOT(handleRecordingWindowButtons()));
    connect(resetButton, SIGNAL(released()), this, SLOT(handleRecordingWindowButtons()));
    connect(recordingControls, SIGNAL(vcChangeState(bool)), this, SLOT(handleVCChangeState(bool)));
    return recordGroup;
}

void TabContent::handleVCChangeState(bool shouldRecord) {
    if (((shouldRecord) && (recordButton->text() == QString("Start Recording"))) || ((!shouldRecord) && (recordButton->text() == QString("Stop Recording")))) {
        changeRecordingState(shouldRecord);
    } else {
        // shouldn't do anything b/c they issued the wrong voice command for the current recording state
    }
}

// Handle buttons on the right panel of the recording window
void TabContent::changeRecordingState(bool shouldRecord) {
    if (shouldRecord) {
        // disable options when recording
        recordOptionsGroup->setEnabled(false);
        modeRadiosGroup->setEnabled(false);
        parent->lockOnPlayOrRecord(true);
        // update button
        recordButton->setText("Stop Recording");
        recordButton->setIcon(stopIcon);
        recordButton->setIconSize(QSize(25,15));
        // start countdown timer
        recordCountdownTimer->start(500); // half a second
    } else {
        // update buttons
        recordButton->setText("Recording Stopped");
        parent->lockOnPlayOrRecord(false);
        // stopped during countdown
        if (recordCountdownTimer->isActive()) {
            // stop countdown timer
            recordCountdownTimer->stop();
            recordResetCountDownTimer();
            // automatically reset
            handleRecordingWindowButtons();
            return;
        } else {
            recordingControls->stopRecording();
        }
        resetButton->setEnabled(true);
        recordButton->setEnabled(false);
        modeRadiosGroup->setEnabled(true);
        // stop recording timer
        recordStopwatchTimer->stop();
        // save new recording
        saveMotion();
    }
}


void TabContent::handleRecordingWindowButtons() {
    // pressed "start recording"
    if (recordButton->text() == QString("Start Recording")) {
        changeRecordingState(true);
        // pressed "stop recording"
    } else if (recordButton->text() == QString("Stop Recording")){
        changeRecordingState(false);
        // pressed "reset"
    } else if (recordButton->text() == QString("Recording Stopped")) {
        // update button
        recordButton->setText("Start Recording");
        recordButton->setIcon(recordIcon);
        recordButton->setIconSize(QSize(35,15));
        recordButton->setEnabled(true);
        resetButton->setEnabled(false);
        // fix labels
        recordCountDownTitleLabel->show();
        recordStopwatchTitleLabel->hide();
        recordCountdownSecondsTitleLabel->show();
        recordStopwatchMinutesTitleLabel->hide();
        recordResetCountDownTimer();
        // enable options and modes
        recordOptionsGroup->setEnabled(true);
        // disable other modes since there is no longer a motion
        modeRadiosGroup->setEnabled(false);
    }
}

// sets the count down label to the given number of seconds
void TabContent::recordSetCountDownTimer(double d) {
    recordCountdownTime->setText(QString::number(d, 'f', 1));
}

// resets the count down label to the user chosen number of seconds
void TabContent::recordResetCountDownTimer() {
    recordCountdownTime->setText(QString::number(recordCountDownSpinner->value(), 'f', 1));
}

// handle the countdown
void TabContent::recordCountdownTimerEvent() {
    // counting down
    if (recordCountdownTime->text().toDouble() > 0) {
        recordCountdownTime->setText(QString::number(recordCountdownTime->text().toDouble() - 0.5, 'f', 1));
        // count down over
    } else {
        // stop counting down
        recordCountdownTimer->stop();
        // start counting up
        recordStopwatchTimer->start(RECORD_STOPWATCH_INTERVAL); // 1 tenth of a second
        // fix labels
        recordDisplayTime = 0;
        recordCountDownTitleLabel->hide();
        recordStopwatchTitleLabel->show();
        recordCountdownSecondsTitleLabel->hide();
        recordStopwatchMinutesTitleLabel->show();
        recordingControls->startRecording();
    }
}

// handles the stopwatch counter
void TabContent::recordStopwatchTimerEvent() {
    qint32 maxTime = recordingControls->getMaxRecordedTime();

    QString displayString = "";

    if (maxTime <= recordDisplayTime) {
        displayString = convertTimeToString(recordDisplayTime);
        recordDisplayTime += RECORD_STOPWATCH_INTERVAL;
    } else {
        recordDisplayTime = maxTime;
        displayString = convertTimeToString(recordDisplayTime);
    }

    recordCountdownTime->setText(displayString);
}


QString TabContent::convertTimeToString(qint32 convertTime) {
    // round number to nearest 10th of millisecond
    int modVal = convertTime%10;
    if (modVal >= 5) {
        convertTime += (10-modVal);
    } else {
        convertTime  -= modVal;
    }

    QTime time = QTime(0, 0, 0, 0);
    time = time.addMSecs(convertTime);

    QString timeString = time.toString("mm:ss.zzz");
    timeString.remove(timeString.length() - 2, 2);
    return timeString;
}
