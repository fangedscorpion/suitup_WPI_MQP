#include "tabcontent.h"

// Playback motion options and controller
QWidget* TabContent::createPlaybackOptionsAndControls() {
    // Playback options
    playbackControls->setActiveMotion(motion);

    playbackOptions = new QGroupBox("Playback Options");
    playbackOptions->setStyleSheet(groupboxStyleSheet);
    playbackOptions->setFont(titleFont);
    QVBoxLayout *playbackLayout = new QVBoxLayout;
    QVBoxLayout *options = new QVBoxLayout;
    playOnSuit = new QCheckBox("Play on suit");
    stepThrough = new QComboBox;
    stepThrough->addItem("Step through mode");
    stepThrough->addItem("Timed mode");
    playbackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    // speed/frame slider
    QVBoxLayout *speedSliderLayout = new QVBoxLayout;
    sfi = new QLabel("Frame Interval");
    sfi->setAlignment(Qt::AlignCenter);
    speedSliderLayout->addWidget(sfi);
    speedSlider = new QSlider(Qt::Horizontal);
    speedSliderLayout->addWidget(speedSlider);
    QHBoxLayout *speeds = new QHBoxLayout;
    minSpeed = new QLabel("8 fpm");
    speeds->addWidget(minSpeed);
    midSpeed = new QLabel("2 fpm");
    midSpeed->setAlignment(Qt::AlignCenter);
    speeds->addWidget(midSpeed);
    maxSpeed = new QLabel("1/2 fpm");
    maxSpeed->setAlignment(Qt::AlignRight);
    speeds->addWidget(maxSpeed);
    speedSliderLayout->addLayout(speeds);
    //hold last frame ticket
    QHBoxLayout *holdLast = new QHBoxLayout;
    holdLast->addWidget(new QLabel("Hold last frame for"));
    QDoubleSpinBox *seconds = new QDoubleSpinBox;
    seconds->setMinimum(0.1);
    seconds->setMaximum(15); // consider changing
    seconds->setDecimals(1);
    holdLast->addWidget(seconds);
    holdLast->addWidget(new QLabel("sec(s)"), -1);
    // speed/frame slider
    QVBoxLayout *positionToleranceLayout = new QVBoxLayout;
    QLabel *pmt = new QLabel("Position matching tolerance");
    pmt->setAlignment(Qt::AlignCenter);
    positionToleranceLayout->addWidget(pmt);
    toleranceSlider = new QSlider(Qt::Horizontal);
    positionToleranceLayout->addWidget(toleranceSlider);
    QHBoxLayout *tolerance = new QHBoxLayout;
    tolerance->addWidget(new QLabel("close"));
    QLabel *rough = new QLabel("rough");
    rough->setAlignment(Qt::AlignRight);
    tolerance->addWidget(rough);
    positionToleranceLayout->addLayout(tolerance);
    // countdown spinner
    playbackCountDownSpinner = new QDoubleSpinBox;
    playbackCountDownSpinner->setMinimum(0.5);
    playbackCountDownSpinner->setValue(5);
    playbackCountDownSpinner->setMaximum(30); // consider changing
    playbackCountDownSpinner->setDecimals(1);
    playbackCountDownSpinner->setSingleStep(.5);
    playbackCountDownSpinner->setMaximumWidth(60);
    QLabel *countDownPreMessage = new QLabel("Count down for");
    QLabel *countDownMessage = new QLabel("sec(s)");
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(countDownPreMessage);
    h->addWidget(playbackCountDownSpinner);
    h->addWidget(countDownMessage);
    // voice control
    playbackVoiceControl = new QCheckBox("Voice Control");
    // add everything
    options->addWidget(playOnSuit);
    options->addWidget(playbackVoiceControl);
    options->addSpacing(10);
    options->addLayout(h);
    options->addLayout(holdLast, -1);
    options->addSpacing(10);
    options->addWidget(stepThrough);
    options->addLayout(speedSliderLayout, -1);
    options->addSpacing(10);
    options->addLayout(positionToleranceLayout, -1);
    options->addSpacing(10);

    playbackLayout->addLayout(options);
    playbackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    playbackOptions->setLayout(playbackLayout);
    playbackOptions->setVisible(false);
    //playbackControls
    //connect(stepThrough, SIGNAL(currentIndexChanged(QString)), playbackControls, SLOT(setStepThroughMode(QString)));
    connect(this, SIGNAL(stepThroughChanged(bool)), playbackControls, SLOT(setStepThroughMode(bool)));
    connect(stepThrough, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSpeedSliderText(QString)));
    connect(toleranceSlider, SIGNAL(sliderMoved(int)), playbackControls, SLOT(updateStepThroughTolerance(int)));
    connect(speedSlider, SIGNAL(sliderMoved(int)), playbackControls, SLOT(speedChanged(int)));
    connect(playOnSuit, SIGNAL(toggled(bool)), playbackControls, SLOT(toggleSuitActive(bool)));
    connect(seconds, SIGNAL(valueChanged(double)), playbackControls, SLOT(modifyHoldTime(double)));
    connect(playbackControls, SIGNAL(timeChanged(int)), this, SLOT(recordDisplayNewTime(int)));
    connect(playbackControls, SIGNAL(playbackStateChanged(bool)), this, SLOT(playbackToggled(bool)));
    connect(playbackCountDownSpinner, SIGNAL(valueChanged(double)), this, SLOT(recordSetCountDownTimer(double)));

    initializePlaybackSettings();

    return playbackOptions;
}


// OpenGL Motion Viewer window with video slider
QWidget* TabContent::createViewer(ACTION_TYPE t) {
    QGroupBox* v;
    if (t == EDIT) {
        v = new QGroupBox("Editing: " + motion->getName());
    } else {
        v= new QGroupBox("Playing: " + motion->getName());
    }

    v->setStyleSheet(groupboxStyleSheet);
    v->setFont(titleFont);

    // viewer window
    viewer = new GLWidget;
    // video controls
    QHBoxLayout *controls = new QHBoxLayout;
    playPause = new QPushButton;
    videoSlider = new SuperSlider;
    handle1Time = new QLabel("00:00.00");
    handle2Time = new QLabel("00:10.00");
    playPause->setIcon(playIcon);
    playPause->setIconSize(QSize(15,15));
    controls->addWidget(playPause);
    controls->addWidget(handle1Time);
    controls->addWidget(videoSlider);
    controls->addWidget(handle2Time);

    // viewer side of the GUI
    QVBoxLayout *viewerPane = new QVBoxLayout;
    viewerPane->addWidget(viewer,1);
    viewerPane->addLayout(controls);
    v->setLayout(viewerPane);
    return v;
}

// updates the playback mode's speed slider labels
void TabContent::updateSpeedSliderText(QString playbackModeString) {
    if (playbackModeString == "Step through mode") {
        switchStepThroughMode(true);
        emit stepThroughChanged(true);
    }
    else if (playbackModeString == "Timed mode") {
        switchStepThroughMode(false);
        emit stepThroughChanged(false);
    }
    else {
        qDebug("Neither option matched, please examine strings to match");
    }
}

void TabContent::switchStepThroughMode(bool steppingThrough) {
    if (steppingThrough) {
        qDebug("stepThroughMode");
        sfi->setText("Frame Interval");
        // this is motions to hold per second of recording
        minSpeed->setText("8 fpm"); // I feel like we should have something more descriptive than this, but 4x didn't really make sense
        midSpeed->setText("2 fpm");
        maxSpeed->setText("1/2 fpm");
    } else {
        qDebug("Timed mode");
        sfi->setText("Playback Speed");
        minSpeed->setText("1/4x");
        midSpeed->setText("1x");
        maxSpeed->setText("4x");
    }
}

void TabContent::playbackToggled(bool playing) {
    lockOnPlayback(playing);
    if (playing) {
        playPause->setIcon(pauseIcon);
    }
    else {
        playPause->setIcon(playIcon);
    }
}

void TabContent::initializePlaybackSettings() {
    stepThrough->setCurrentIndex(1);
    switchStepThroughMode(false);
    speedSlider->setValue(speedSlider->maximum()/2);
}

void TabContent::lockOnPlayback(bool playing) {
    modeRadiosGroup->setEnabled(!playing);
    playbackOptions->setEnabled(!playing);
}

void TabContent::sliderValueChanged(int newVal) {
    qDebug()<<"Value is : "<<newVal;
}

void TabContent::playbackCountdownTimerEvent() {
    // counting down
    if (playbackCountdownTime->text().toDouble() > 0) {
       playbackCountdownTime->setText(QString::number(playbackCountdownTime->text().toDouble() - 0.5, 'f', 1));
    // count down over
    } else {
        // stop counting down
        playbackCountdownTimer->stop();
        playbackResetCountDownTimer();
        // TODO: Call playbackController play button
    }
}

// sets the count down label to the given number of seconds
void TabContent::playbackSetCountDownTimer(double d) {
    playbackCountdownTime->setText(QString::number(d, 'f', 1));
}

// resets the count down label to the user chosen number of seconds
void TabContent::playbackResetCountDownTimer() {
    playbackCountdownTime->setText(QString::number(recordCountDownSpinner->value(), 'f', 1));
}

// When play button is pressed.
// playbackCountdownTimer->start(500); // half a second
// display countdown time
// have playbackCountdownTimerEvent() call playbackController play
// when pause button is pressed.
// playbackCountdownTimer->stop(); <- if .isActive();

