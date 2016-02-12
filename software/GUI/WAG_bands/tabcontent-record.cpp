#include "tabcontent.h"

QWidget* TabContent::createRecordOptionsAndController() {
    recordingControls = new RecordingController(suitObj);
    recordingControls->setActiveMotion(motion);
    QGroupBox *recordOptionsGroup = new QGroupBox("Recording Options");
    recordOptionsGroup->setStyleSheet(groupboxStyleSheet);
    recordOptionsGroup->setFont(titleFont);

    recordCountDownSpinner = new QDoubleSpinBox;
    recordCountDownSpinner->setMinimum(0.5);
    recordCountDownSpinner->setValue(5);
    recordCountDownSpinner->setMaximum(30); // consider changing
    recordCountDownSpinner->setDecimals(1);
    recordCountDownSpinner->setSingleStep(.5);
    recordCountDownSpinner->setMaximumWidth(60);
    QLabel *countDownPreMessage = new QLabel("Count down for");
    QLabel *countDownMessage = new QLabel("sec(s)");
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(countDownPreMessage);
    h->addWidget(recordCountDownSpinner);
    h->addWidget(countDownMessage);

    recordVoiceControl = new QCheckBox("Voice Control");

    QVBoxLayout *buttons = new QVBoxLayout;
    buttons->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(recordVoiceControl);
    buttons->addLayout(h);
    buttons->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    recordOptionsGroup->setLayout(buttons);

    connect(recordCountDownSpinner, SIGNAL(valueChanged(double)), this, SLOT(recordSetCountDownTimer(double)));
    connect(recordVoiceControl, SIGNAL(toggled(bool)), recordingControls, SLOT(toggleVoiceControl(bool)));
    return recordOptionsGroup;
}

// The right panel under record motion
QWidget* TabContent::createRecordingWindow() {
    recordGroup = new QGroupBox("Recording: " + motion->getName());
    recordGroup->setStyleSheet(groupboxStyleSheet);
    recordGroup->setFont(titleFont);
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
    QHBoxLayout *h = new QHBoxLayout;
    h->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    h->addWidget(recordCountdownTime);
    h->addWidget(recordCountdownSecondsTitleLabel);
    h->addWidget(recordStopwatchMinutesTitleLabel);
    h->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    resetButton = new QPushButton("Reset");
    resetButton->setMinimumHeight(buttonHeight);
    resetButton->setEnabled(false);
    resetButton->setIcon(resetIcon);
    resetButton->setIconSize(QSize(35,30));

    recordButton = new QPushButton;
    recordButton->setMinimumHeight(buttonHeight);
    recordButton->setText("Start Recording");
    recordButton->setIcon(recordIcon);
    recordButton->setIconSize(QSize(35,15));

    // viewer side of the GUI
    QVBoxLayout *info = new QVBoxLayout;
    info->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QHBoxLayout* b = new QHBoxLayout;
    b->addWidget(resetButton, 1);
    b->addWidget(recordButton, 2);
    info->addLayout(b);
    info->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    info->addLayout(h);
    info->addWidget(recordCountDownTitleLabel);
    info->addWidget(recordStopwatchTitleLabel);
    recordGroup->setLayout(info);

    connect(recordButton, SIGNAL(released()), this, SLOT(handleRecordingWindowButtons()));
    connect(resetButton, SIGNAL(released()), this, SLOT(handleRecordingWindowButtons()));
    return recordGroup;
}

// Handle buttons on the right panel of the recording window
void TabContent::handleRecordingWindowButtons() {
    // pressed "start recording"
    if (recordButton->text() == QString("Start Recording")) {
        // disable options when recording
        recordVoiceControl->setEnabled(false);
        recordCountDownSpinner->setEnabled(false);
        modeRadiosGroup->setEnabled(false);
        // update button
        recordButton->setText("Stop Recording");
        recordButton->setIcon(stopIcon);
        recordButton->setIconSize(QSize(25,15));
        // start countdown timer
        msecs = 0;
        recordCountdownTimer->start(500); // half a second
    // pressed "stop recording"
    } else if (recordButton->text() == QString("Stop Recording")){
        // update buttons
        recordButton->setText("Recording Stopped");
        // stopped during countdown
        if (recordCountdownTimer->isActive()) {
            // stop countdown timer
            recordCountdownTimer->stop();
            recordResetCountDownTimer();
            // automatically reset
            handleRecordingWindowButtons();
            modeRadiosGroup->setEnabled(true);
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
        recordVoiceControl->setEnabled(true);
        recordCountDownSpinner->setEnabled(true);
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
        recordStopwatchTimer->start(100); // 1 tenth of a second
        // fix labels
        recordCountDownTitleLabel->hide();
        recordStopwatchTitleLabel->show();
        recordCountdownSecondsTitleLabel->hide();
        recordStopwatchMinutesTitleLabel->show();
        recordingControls->startRecording();
    }
}

// handles the stopwatch counter
void TabContent::recordStopwatchTimerEvent() {
    // display the time 00:00.0
    int hundredths = msecs % 10;
    int numSeconds = (msecs/10) % 60;
    int numMinutes = msecs/600;
    QString minNum = QString("%1").arg(numMinutes, 2, 10, QChar('0'));
    QString secNum = QString("%1").arg(numSeconds, 2, 10, QChar('0'));
    // set label
    recordCountdownTime->setText(minNum + ":" + secNum + "." + QString::number(hundredths));

    msecs++;
}
