#include "tabcontent.h"
#include <unistd.h>
#include <QTimerEvent>

TabContent::TabContent(MainWindow *in_parent, WAGFile* in_motion, USER u, ACTION_TYPE initiallyShow) : parent(in_parent){
    motion = in_motion;
    user = u;
    playbackControls = new PlaybackController;
    titleFont = QFont( "Arial", 15, QFont::Bold);
    groupboxStyleSheet = "QGroupBox{ border: 1px solid gray; border-radius: 9px; margin-left: 0.25em; margin-right: 0.25em; margin-top: 0.5em; padding: 35px 3px 0 3px;} QGroupBox::title{subcontrol-position: top center; subcontrol-origin: margin;}";
    textInputStyleRed = "QLineEdit {border: 1px solid red; background: white;} QTextEdit {border: 1px solid red; background: white;}";
    textInputStyleWhite = "QLineEdit {background: white;} QTextEdit {background: white;}";
    buttonHeight = 35;

    createIcons();
    createFileInfoWindow();

    optionsStack = new QStackedWidget;
    viewerStack = new QStackedWidget;
    if (u.hasAction(RECORD)) {
        optionsStack->addWidget(createRecordOptionsAndController());
        viewerStack->addWidget(createRecordingWindow());
    }
    if (u.hasAction(EDIT)) {
        optionsStack->addWidget(createEditOptionsAndControls());
        viewerStack->addWidget(createViewer(EDIT));
    }
    if (u.hasAction(PLAYBACK)) {
        optionsStack->addWidget(createPlaybackOptionsAndControls());
        viewerStack->addWidget(createViewer(PLAYBACK));
    }

    // contains both options and viewer
    QHBoxLayout *splitPanes = new QHBoxLayout;
    QVBoxLayout *vl = new QVBoxLayout;

    vl->addWidget(optionsStack);
    vl->addWidget(createModeRadios(u));
    this->show(initiallyShow);

    splitPanes->addLayout(vl, 1);
    splitPanes->addWidget(viewerStack, 2);

    this->setLayout(splitPanes);
    overlay = new Overlay(this);
    overlay->makeSemiTransparent();
    overlay->hide();
    connect(this, SIGNAL(resizedWindow()), overlay, SLOT(resizeWindow()));
}

TabContent::~TabContent() {}

void TabContent::createIcons() {
    playIcon = QIcon(QPixmap(":/icons/play.png"));
    pauseIcon = QIcon(QPixmap(":/icons/pause.png"));
    recordIcon = QIcon(QPixmap(":/icons/record.png"));
    cropIcon = QIcon(QPixmap(":/icons/crop.png"));
    splitIcon = QIcon(QPixmap(":/icons/split.png"));
    undoIcon = QIcon(QPixmap(":/icons/undo.png"));
    editIcon = QIcon(QPixmap(":/icons/edit.png"));
    stopIcon = QIcon(QPixmap(":/icons/stop.png"));
    resetIcon = QIcon(QPixmap(":/icons/reset.png"));
}

// event for when the main window is resized
void TabContent::resizeEvent(QResizeEvent* r) {
    QWidget::resizeEvent(r);
    // resize the overlay to cover the whole window
    overlay->resize(this->width(), this->height());
    emit resizedWindow();
}

void TabContent::show(ACTION_TYPE a) {
    // if the user has the action a
    if (user.hasAction(a)) {
        optionsStack->setCurrentIndex(a);
        viewerStack->setCurrentIndex(a);
    // EDIT is used when opening a file, so if the user doesn't have edit ability, check playback
    } else if (a == EDIT && user.hasAction(PLAYBACK)) {
        optionsStack->setCurrentIndex(PLAYBACK);
        viewerStack->setCurrentIndex(PLAYBACK);
    // otherwise show any one of the available actions.
    } else {
        std::set<ACTION_TYPE>::iterator it=user.getActions().begin();
        if (it != user.getActions().end())
            show(*it);
        // else ERROR
        // TODO: throw exception here
        return;
    }

    if (a == RECORD && user.hasAction(RECORD)) {
        recordRadio->setChecked(true);
    } else if (a == EDIT && user.hasAction(EDIT)) {
        disconnect(playPause, SIGNAL(released()), playbackControls, 0);
        editRadio->setChecked(true);
    } else if ((a == PLAYBACK || a == EDIT) && user.hasAction(PLAYBACK)) {
        connect(playPause, SIGNAL(released()), playbackControls, SLOT (togglePlay()));
        playbackRadio->setChecked(true);
    } else {
        std::set<ACTION_TYPE>::iterator it=user.getActions().begin();
        if (it != user.getActions().end())
            show(*it);
        // else ERROR
        // TODO: throw exception here
        return;
    }
}

// The groupbox of Mode radio buttons
QWidget* TabContent::createModeRadios(USER u) {
    modeRadiosGroup = new QGroupBox("Modes");
    modeRadiosGroup->setAlignment(Qt::AlignHCenter);
    modeRadiosGroup->setStyleSheet(groupboxStyleSheet);
    modeRadiosGroup->setFont(titleFont);
    QVBoxLayout *vl = new QVBoxLayout;
    recordRadio = new smartRadioButton("Record Motion", RECORD);
    editRadio = new smartRadioButton("Edit Motion", EDIT);
    playbackRadio = new smartRadioButton("Playback Motion", PLAYBACK);

    // if there is only 1 action this user can perform, don't show any modes
    int count = 0;
    if (u.hasAction(RECORD))
        count++;
    if (u.hasAction(EDIT))
        count++;
    if (u.hasAction(PLAYBACK))
        count++;

    if (count == 1) {
        modeRadiosGroup->hide();
        return modeRadiosGroup;
    }

    vl->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    if (u.hasAction(RECORD)) {
        vl->addWidget(recordRadio);
        connect(recordRadio, SIGNAL(released(ACTION_TYPE)), this, SLOT(show(ACTION_TYPE)));
    }
    if (u.hasAction(EDIT)) {
        vl->addWidget(editRadio);
        connect(editRadio, SIGNAL(released(ACTION_TYPE)), this, SLOT(show(ACTION_TYPE)));
    }
    if (u.hasAction(PLAYBACK)) {
        vl->addWidget(playbackRadio);
        connect(playbackRadio, SIGNAL(released(ACTION_TYPE)), this, SLOT(show(ACTION_TYPE)));
    }
    vl->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    modeRadiosGroup->setLayout(vl);
    return modeRadiosGroup;
}

// Playback motion options and controller
QWidget* TabContent::createPlaybackOptionsAndControls() {
    // Playback options
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
    // add everything
    options->addWidget(playOnSuit);
    options->addSpacing(10);
    options->addWidget(stepThrough);
    options->addSpacing(10);
    options->addLayout(speedSliderLayout, -1);
    options->addSpacing(10);
    options->addLayout(holdLast, -1);
    options->addSpacing(10);
    options->addLayout(positionToleranceLayout, -1);
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
    connect(playbackControls, SIGNAL(timeChanged(int)), this, SLOT(displayNewTime(int)));
    connect(playbackControls, SIGNAL(playbackStateChanged(bool)), this, SLOT(playbackToggled(bool)));

    initializePlaybackSettings();

    return playbackOptions;
}

// Edit Recording options and controller
// TODO: add edit file name and description in here
QWidget* TabContent::createEditOptionsAndControls() {
    // Edit recording options
    QGroupBox *editOptions = new QGroupBox("Editing Options");
    editOptions->setStyleSheet(groupboxStyleSheet);
    editOptions->setFont(titleFont);
    editOptions->setAlignment(Qt::AlignRight);
    QPushButton *undoBtn = new QPushButton;
    undoBtn->setIcon(undoIcon);
    undoBtn->setIconSize(QSize(51,25));
    undoBtn->setMinimumHeight(buttonHeight);
    undoBtn->setText("Undo");
    undoBtn->setEnabled(false);
    QPushButton *cropBtn = new QPushButton;
    cropBtn->setIcon(cropIcon);
    cropBtn->setIconSize(QSize(49,25));
    cropBtn->setMinimumHeight(buttonHeight);
    cropBtn->setText("Crop");
    QPushButton *splitBtn = new QPushButton;
    splitBtn->setIcon(splitIcon);
    splitBtn->setIconSize(QSize(62,25));
    splitBtn->setMinimumHeight(buttonHeight);
    splitBtn->setText("Split");
    QPushButton *fileInfoBtn = new QPushButton;
    fileInfoBtn->setIcon(editIcon);
    fileInfoBtn->setIconSize(QSize(62,25));
    fileInfoBtn->setMinimumHeight(buttonHeight);
    fileInfoBtn->setText("Edit Motion Information");
    QVBoxLayout *recordPlaybackLayout = new QVBoxLayout;
    QVBoxLayout *buttons = new QVBoxLayout;
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(undoBtn);
    buttons->addWidget(cropBtn);
    buttons->addWidget(splitBtn);
    buttons->addWidget(fileInfoBtn);
    recordPlaybackLayout->addLayout(buttons, 1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    editOptions->setLayout(recordPlaybackLayout);

    connect(fileInfoBtn, SIGNAL(released()), this, SLOT(launchFileInfo()));

    return editOptions;
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

    connect(videoSlider, SIGNAL(alt_valueChanged(int)), this, SLOT(sliderValueChanged(int)));

    connect(videoSlider, SIGNAL(sliderMoved(int)), this, SLOT(sliderValueChanged(int)));

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

void TabContent::applicationResized() {
    videoSlider->resized();
}

// TODO: add count down spinner?
QWidget* TabContent::createRecordOptionsAndController() {
    QGroupBox *recordOptionsGroup = new QGroupBox("Recording Options");
    recordOptionsGroup->setStyleSheet(groupboxStyleSheet);
    recordOptionsGroup->setFont(titleFont);

    countDownSpinner = new QDoubleSpinBox;
    countDownSpinner->setMinimum(0.5);
    countDownSpinner->setValue(5);
    countDownSpinner->setMaximum(30); // consider changing
    countDownSpinner->setDecimals(1);
    countDownSpinner->setSingleStep(.5);
    countDownSpinner->setMaximumWidth(60);
    QLabel *countDownPreMessage = new QLabel("Count down for");
    QLabel *countDownMessage = new QLabel("sec(s)");
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(countDownPreMessage);
    h->addWidget(countDownSpinner);
    h->addWidget(countDownMessage);

    voiceControl = new QCheckBox("Voice Control");
    voiceControl->setChecked(parent->isVoiceControlOn());

    QVBoxLayout *buttons = new QVBoxLayout;
    buttons->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addLayout(h);
    buttons->addWidget(voiceControl);
    buttons->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    recordOptionsGroup->setLayout(buttons);

    connect(countDownSpinner, SIGNAL(valueChanged(double)), this, SLOT(setCountDownTimer(double)));
    connect(voiceControl, SIGNAL(clicked(bool)), parent, SLOT(setVoiceControl(bool)));

    return recordOptionsGroup;
}

QWidget* TabContent::createRecordingWindow() {
    recordGroup = new QGroupBox("Recording: " + motion->getName());
    recordGroup->setStyleSheet(groupboxStyleSheet);
    recordGroup->setFont(titleFont);
    // viewer window
    // TODO: Make a File class to hold tags, description, filename

    QFont bigFont = QFont("Arial", 25);
    QFont timeFont = QFont("Arial", 50);
    countDownLabel = new QLabel("Count Down");
    countDownLabel->setFont(bigFont);
    countDownLabel->setAlignment(Qt::AlignCenter);
    recordTime = new QLabel("Record Time");
    recordTime->setFont(bigFont);
    recordTime->setAlignment(Qt::AlignCenter);
    recordTime->hide();
    time = new QLabel(QString::number(countDownSpinner->value(), 'f', 1));
    time->setFont(timeFont);
    time->setAlignment(Qt::AlignBottom);
    seconds = new QLabel("sec(s)");
    seconds->setFont(bigFont);
    seconds->setAlignment(Qt::AlignBottom);
    minutes = new QLabel("min(s)");
    minutes->setFont(bigFont);
    minutes->setAlignment(Qt::AlignBottom);
    minutes->hide();
    QHBoxLayout *h = new QHBoxLayout;
    h->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    h->addWidget(time);
    h->addWidget(seconds);
    h->addWidget(minutes);
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
    info->addWidget(countDownLabel);
    info->addWidget(recordTime);
    recordGroup->setLayout(info);

    connect(recordButton, SIGNAL(released()), this, SLOT(handleRecordingWindowButtons()));
    connect(resetButton, SIGNAL(released()), this, SLOT(handleRecordingWindowButtons()));
    return recordGroup;
}

void TabContent::createFileInfoWindow() {

    fileInfoWidget = new OverlayWidget(this, "Edit Motion Information");
    QVBoxLayout *layout = fileInfoWidget->getLayout();

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(100);
    l1->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    f->addWidget(l1, -1);
    infoFilenameTextEdit = new QLineEdit;
    infoFilenameTextEdit->setStyleSheet(textInputStyleWhite);
    f->addWidget(infoFilenameTextEdit);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    QLabel *l2 = new QLabel("Description: ");
    l2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l2->setMinimumWidth(100);
    d->addWidget(l2, -1);
    infoFileDescription = new QTextEdit;
    infoFileDescription->setStyleSheet(textInputStyleWhite);
    infoFileDescription->setMinimumHeight(150);
    d->addWidget(infoFileDescription);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    QLabel *l3 = new QLabel("Keywords: ");
    l3->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l3->setMinimumWidth(100);
    t->addWidget(l3, -1);
    infoFileTagsTextEdit = new QLineEdit;
    infoFileTagsTextEdit->setStyleSheet(textInputStyleWhite);
    t->addWidget(infoFileTagsTextEdit);
    addTagBtn = new QPushButton("Add Keyword");
    addTagBtn->setEnabled(false);
    addTagBtn->setMinimumHeight(buttonHeight);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *l4 = new QLabel;
    l4->setMinimumWidth(100);
    t2->addWidget(l4, -1);
    t2->addWidget(addTagBtn, -1);
    infoFileTagsLabel = new QLabel();
    t2->addWidget(infoFileTagsLabel, 1);

    QHBoxLayout *btns = new QHBoxLayout;
    smartPushButton* saveFileBtn = new smartPushButton("Save");
    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setMinimumHeight(buttonHeight);
    btns->addWidget(cancel);
    btns->addWidget(saveFileBtn);
    layout->addLayout(f);
    layout->addSpacing(10);
    layout->addLayout(d);
    layout->addSpacing(10);
    layout->addLayout(t);
    layout->addLayout(t2);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addLayout(btns);

    // only connect handleUserOptions when the user selection window is visible to user
    connect(saveFileBtn, SIGNAL(released()), this, SLOT(saveFileInfo()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeFileInfo()));
    connect(addTagBtn, SIGNAL(released()), this, SLOT(addTag()));
    connect(infoFileTagsTextEdit, SIGNAL(returnPressed()), this, SLOT(addTag()));
//    connect(infoFilenameTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleNewFileRequiredInput(QString)));
//    connect(infoFileDescription, SIGNAL(textChanged()), this, SLOT(handleNewFileRequiredInput()));
//    connect(infoFileTagsTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleNewFileRequiredInput(QString)));
    connect(this, SIGNAL(resizedWindow()), fileInfoWidget, SLOT(resizeWindow()));
    emit this->resizedWindow();
}

void TabContent::launchFileInfo() {
    overlay->show();
    fileInfoWidget->show();
    fileInfoWidget->raise();

    infoFilenameTextEdit->setText(motion->getName());
    infoFileDescription->setText(motion->getDescription());
//    infoFileTagsLabel->setText(motion->getTagsString());

}

void TabContent::saveFileInfo() {
    WAGFile* w = new WAGFile(infoFilenameTextEdit->text(), infoFileDescription->toPlainText(),
                             motion->getAuthor(), infoFileTagsLabel->text().split("; ").toVector());
    updateMotion(w);
    closeFileInfo();
}

void TabContent::closeFileInfo() {
    overlay->hide();
    fileInfoWidget->hide();
}

// sets the count down label to the given number of seconds
void TabContent::setCountDownTimer(double d) {
    time->setText(QString::number(d, 'f', 1));
}

// resets the count down label to the user chosen number of seconds
void TabContent::resetCountDownTimer() {
    time->setText(QString::number(countDownSpinner->value(), 'f', 1));
}

void TabContent::handleRecordingWindowButtons() {
    // pressed "start recording"
    if (recordButton->text() == QString("Start Recording")) {
        // disable options when recording
        voiceControl->setEnabled(false);
        countDownSpinner->setEnabled(false);
        modeRadiosGroup->setEnabled(false);
        // update button
        recordButton->setText("Stop Recording");
        recordButton->setIcon(stopIcon);
        recordButton->setIconSize(QSize(25,15));
        // start countdown timer
        msecs = 0;
        countDownTimerID = startTimer(500);
    // pressed "stop recording"
    } else if (recordButton->text() == QString("Stop Recording")){
        // update buttons
        recordButton->setText("Recording Stopped");
        // stopped during countdown
        if (countDownTimerID != 0) {
            // stop countdown timer
            killTimer(countDownTimerID);
            countDownTimerID = 0;
            resetCountDownTimer();
            // automatically reset
            handleRecordingWindowButtons();
            modeRadiosGroup->setEnabled(true);
            return;
        }
        resetButton->setEnabled(true);
        recordButton->setEnabled(false);
        modeRadiosGroup->setEnabled(true);
        // stop recording timer
        killTimer(recordTimeTimerID);
        recordTimeTimerID = 0;
    // pressed "reset"
    } else if (recordButton->text() == QString("Recording Stopped")) {
        // update button
        recordButton->setText("Start Recording");
        recordButton->setIcon(recordIcon);
        recordButton->setIconSize(QSize(35,15));
        recordButton->setEnabled(true);
        resetButton->setEnabled(false);
        // fix labels
        countDownLabel->show();
        recordTime->hide();
        seconds->show();
        minutes->hide();
        resetCountDownTimer();
        // enable options and modes
        voiceControl->setEnabled(true);
        countDownSpinner->setEnabled(true);
    }
}

void TabContent::handleCountDown() {
    // counting down
    if (time->text().toDouble() > 0) {
       time->setText(QString::number(time->text().toDouble() - 0.5, 'f', 1));
    // count down over
    } else {
        // stop counting down
        killTimer(countDownTimerID);
        countDownTimerID = 0;
        // start counting up
        recordTimeTimerID = startTimer(100);
        // fix labels
        countDownLabel->hide();
        recordTime->show();
        seconds->hide();
        minutes->show();
    }
}

void TabContent::handleRecordTimeCounter() {
    // display the time 00:00.0
    int hundredths = msecs % 10;
    int numSeconds = (msecs/10) % 60;
    int numMinutes = msecs/600;
    QString minNum = QString("%1").arg(numMinutes, 2, 10, QChar('0'));
    QString secNum = QString("%1").arg(numSeconds, 2, 10, QChar('0'));
    // set label
    time->setText(minNum + ":" + secNum + "." + QString::number(hundredths));

    msecs++;
}

void TabContent::updateMotion(WAGFile* file) {
    motion = file;
    if (user.hasAction(PLAYBACK))
        static_cast<QGroupBox*>(viewerStack->widget(PLAYBACK))->setTitle(QString("Playing: ") + motion->getName());
    if (user.hasAction(EDIT))
        static_cast<QGroupBox*>(viewerStack->widget(EDIT))->setTitle(QString("Editing: ") + motion->getName());
    if (user.hasAction(RECORD))
        recordGroup->setTitle(QString("Recording: ") + motion->getName());
}

void TabContent::timerEvent(QTimerEvent *event) {
    if(event->timerId() == countDownTimerID) {
        handleCountDown();
    } else if (event->timerId() == recordTimeTimerID) {
        handleRecordTimeCounter();
    }
}

void TabContent::displayNewTime(int newMillis) {
    int numSeconds = newMillis/1000;
    int hundredths = (newMillis - numSeconds*1000)/10;
    int numMinutes = numSeconds/60;
    numSeconds = numSeconds - numMinutes*60;
    // set label
    QString minNum = QString("%1").arg(numMinutes, 2, 10, QChar('0'));
    QString secNum = QString("%1").arg(numSeconds, 2, 10, QChar('0'));
    QString hundNum = QString("%1").arg(hundredths, 2, 10, QChar('0'));
    handle1Time->setText(minNum + ":" + secNum + "." + hundNum);
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
