#include "tabcontent.h"

TabContent::TabContent(QString filename, ACTION_TYPE t) {
    filenameString = filename;
    playbackControls = new PlaybackController;
    titleFont = QFont( "Arial", 15, QFont::Bold);
    titleStyleSheet = "QGroupBox{ border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; subcontrol-origin: margin; left: 10px; padding: 25px 3px 0 3px;}";

    optionsStack = new QStackedWidget;
    optionsStack->addWidget(createEditOptionsAndControls());
    optionsStack->addWidget(createPlaybackOptionsAndControls());
    optionsStack->addWidget(createRecordOptionsAndController());

    viewerStack = new QStackedWidget;
    viewerStack->addWidget(createViewer(EDIT));
    viewerStack->addWidget(createViewer(PLAYBACK));
    viewerStack->addWidget(createFileInfoGroup());

    // contains both options and viewer
    QHBoxLayout *splitPanes = new QHBoxLayout;
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(createModeRadios());
    vl->addWidget(optionsStack);
    switch (t) {
    case EDIT:
        showEditMode();
        break;
    case PLAYBACK:
        showPlaybackMode();
        // TODO: if you are the author, allow this
        editRadio->setEnabled(false);
        break;
    default: // includes record
        showRecordMode();
        break;
    }

    splitPanes->addLayout(vl, 1);
    splitPanes->addWidget(viewerStack, 2);

    this->setLayout(splitPanes);
}

TabContent::~TabContent() {}

void TabContent::showPlaybackMode() {
    connect(playPause, SIGNAL(released()), playbackControls, SLOT (togglePlay()));
    optionsStack->setCurrentIndex(PLAYBACK);
    viewerStack->setCurrentIndex(PLAYBACK);
    playbackRadio->setChecked(true);
    modeRadiosGroup->setVisible(true);
}

void TabContent::showEditMode() {
    disconnect(playPause, SIGNAL(released()), playbackControls, 0);
    optionsStack->setCurrentIndex(EDIT);
    viewerStack->setCurrentIndex(EDIT);
    modeRadiosGroup->setVisible(true);
    editRadio->setChecked(true);
}

void TabContent::showRecordMode() {
    optionsStack->setCurrentIndex(RECORD);
    viewerStack->setCurrentIndex(RECORD);
    modeRadiosGroup->setVisible(false);
}

// The groupbox of Mode radio buttons
QWidget* TabContent::createModeRadios() {
    modeRadiosGroup = new QGroupBox("Modes");
    modeRadiosGroup->setStyleSheet(titleStyleSheet);
    modeRadiosGroup->setFont(titleFont);
    playbackRadio = new QRadioButton("Playback Motion");
    editRadio = new QRadioButton("Edit Motion");
    QVBoxLayout *vl = new QVBoxLayout;
    vl->addWidget(playbackRadio);
    vl->addWidget(editRadio);
    modeRadiosGroup->setLayout(vl);

    connect(playbackRadio, SIGNAL(released()), this, SLOT(showPlaybackMode()));
    connect(editRadio, SIGNAL(released()), this, SLOT(showEditMode()));
    return modeRadiosGroup;
}

// Playback motion options and controller
QWidget* TabContent::createPlaybackOptionsAndControls() {
    // Playback options
    QGroupBox *playbackOptions = new QGroupBox("Playback Options");
    playbackOptions->setStyleSheet(titleStyleSheet);
    playbackOptions->setFont(titleFont);
    QVBoxLayout *playbackLayout = new QVBoxLayout;
    QVBoxLayout *options = new QVBoxLayout;
    options->setAlignment(Qt::AlignCenter);
    QWidget *checkbox = new QWidget;
    QHBoxLayout *checkboxLayout = new QHBoxLayout;
    checkboxLayout->setAlignment(Qt::AlignCenter);
    playOnSuit = new QCheckBox("Play on suit");
    checkboxLayout->addWidget(playOnSuit, -1);
    checkbox->setLayout(checkboxLayout);
    stepThrough = new QComboBox;
    stepThrough->addItem("Step through mode");
    stepThrough->addItem("Timed mode");
    playbackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    options->addWidget(checkbox);
    options->addWidget(stepThrough);
    // speed/frame slider
    QVBoxLayout *speedSliderLayout = new QVBoxLayout;
    sfi = new QLabel("Frame Interval");
    sfi->setAlignment(Qt::AlignCenter);
    speedSliderLayout->addWidget(sfi);
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setValue(speedSlider->maximum()/2);
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
    options->addLayout(speedSliderLayout, -1);
    options->addLayout(holdLast, -1);
    options->addLayout(positionToleranceLayout, -1);
    playbackLayout->addLayout(options);
    playbackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    playbackOptions->setLayout(playbackLayout);
    playbackOptions->setVisible(false);
    //playbackControls
    connect(stepThrough, SIGNAL(currentIndexChanged(QString)), playbackControls, SLOT(setStepThroughMode(QString)));
    connect(stepThrough, SIGNAL(currentIndexChanged(QString)), this, SLOT(updateSpeedSliderText(QString)));
    connect(toleranceSlider, SIGNAL(sliderMoved(int)), playbackControls, SLOT(updateStepThroughTolerance(int)));
    connect(speedSlider, SIGNAL(sliderMoved(int)), playbackControls, SLOT(speedChanged(int)));
    connect(playOnSuit, SIGNAL(toggled(bool)), playbackControls, SLOT(toggleSuitActive(bool)));
    connect(seconds, SIGNAL(valueChanged(double)), playbackControls, SLOT(modifyHoldTime(double)));
    connect(playbackControls, SIGNAL(timeChanged(int)), this, SLOT(displayNewTime(int)));

    return playbackOptions;
}

// Edit Recording options and controller
QWidget* TabContent::createEditOptionsAndControls() {
    // Edit recording options
    QGroupBox *editOptions = new QGroupBox("Editing Options");
    editOptions->setStyleSheet(titleStyleSheet);
    editOptions->setFont(titleFont);
    editOptions->setAlignment(Qt::AlignRight);
    QPushButton *undoBtn = new QPushButton("Undo");
    QPushButton *cropBtn = new QPushButton("Crop");
    QPushButton *splitBtn = new QPushButton("Split");
    QPushButton *rerecord = new QPushButton("Restart Recording");
    QVBoxLayout *recordPlaybackLayout = new QVBoxLayout;
    QVBoxLayout *buttons = new QVBoxLayout;
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(undoBtn);
    buttons->addWidget(cropBtn);
    buttons->addWidget(splitBtn);
    buttons->addWidget(rerecord);
    recordPlaybackLayout->addLayout(buttons, 1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    editOptions->setLayout(recordPlaybackLayout);

    connect(rerecord, SIGNAL(released()), this, SLOT(showRecordMode()));

    return editOptions;
}

// OpenGL Motion Viewer window with video slider
QWidget* TabContent::createViewer(ACTION_TYPE t) {
    if (t == EDIT) {
        viewerGroup = new QGroupBox("Editing: " + filenameString);
    } else {
        viewerGroup = new QGroupBox("Playing: " + filenameString);
    }

    viewerGroup->setStyleSheet(titleStyleSheet);
    viewerGroup->setFont(titleFont);

    // viewer window
    viewer = new GLWidget;
    // video controls
    QHBoxLayout *controls = new QHBoxLayout;
    playPause = new QPushButton;
    videoSlider = new SuperSlider;
    playIcon = QIcon(QPixmap(":/icons/play.png"));
    pauseIcon = QIcon(QPixmap(":/icons/pause.png"));
    recordIcon = QIcon(QPixmap(":/icons/record.png"));
    handle1Time = new QLabel("00:00");
    handle2Time = new QLabel("00:10");
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
    viewerGroup->setLayout(viewerPane);
    return viewerGroup;
}

// updates the playback mode's speed slider labels
void TabContent::updateSpeedSliderText(QString playbackModeString) {
    if (playbackModeString == "Step through mode") {
        qDebug("stepThroughMode");
        sfi->setText("Frame Interval");
        // this is motions to hold per second of recording
        minSpeed->setText("8 fpm"); // I feel like we should have something more descriptive than this, but 4x didn't really make sense
        midSpeed->setText("2 fpm");
        maxSpeed->setText("1/2 fpm");
    }
    else if (playbackModeString == "Timed mode") {
        qDebug("Timed mode");
        sfi->setText("Playback Speed");
        minSpeed->setText("1/4 x");
        midSpeed->setText("1x");
        maxSpeed->setText("4x");
    }
    else {
        qDebug("Neither option matched, please examine strings to match");
    }
}


void TabContent::applicationResized() {
    videoSlider->resized();
}

void TabContent::handleRecordingButton() {
    if (recordButton->text() == QString("Start Recording")) {
        recordMessage->setText("Recording...");
        recordButton->setText("Stop Recording");
        edit->setEnabled(false);
    } else {
        recordMessage->setText("Stopped Recording");
        recordButton->setText("Start Recording");
        edit->setEnabled(true);
    }
    // TODO: make text red and big
}

// TODO: add count down spinner?
QWidget* TabContent::createRecordOptionsAndController() {
    QGroupBox *recordOptionsGroup = new QGroupBox("Recording Options");
    recordOptionsGroup->setStyleSheet(titleStyleSheet);
    recordOptionsGroup->setFont(titleFont);

    recordMessage = new QLabel();
    recordButton = new QPushButton;
    recordButton->setText("Start Recording");
    edit = new QPushButton;
    edit->setText("Edit Recording");
    QVBoxLayout *buttons = new QVBoxLayout;
    buttons->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(recordMessage);
    buttons->addWidget(recordButton);
    buttons->addWidget(edit);
    buttons->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    recordOptionsGroup->setLayout(buttons);

    connect(recordButton, SIGNAL(released()), this, SLOT(handleRecordingButton()));
    connect(edit, SIGNAL(released()), this, SLOT(showEditMode()));

    return recordOptionsGroup;
}

QWidget* TabContent::createFileInfoGroup() {
    recordGroup = new QGroupBox("Recording: " + filenameString);
    recordGroup->setStyleSheet(titleStyleSheet);
    recordGroup->setFont(titleFont);
    // viewer window
    // TODO: Make a File class to hold tags, description, filename

    // description
    QHBoxLayout *d = new QHBoxLayout;
    QLabel *l2 = new QLabel("Description: ");
    l2->setMinimumWidth(100);
    d->addWidget(l2, -1);
    QLabel *description = new QLabel("description");
    description->setWordWrap(true);
    description->setMinimumHeight(150);
    d->addWidget(description, 1);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    QLabel *l3 = new QLabel("Tags: ");
    l3->setMinimumWidth(100);
    t->addWidget(l3, -1);
    QLabel *tags = new QLabel("Sports;");
    tags->setStyleSheet("border: 1px;");
    t->addWidget(tags, 1);

    // viewer side of the GUI
    QVBoxLayout *info = new QVBoxLayout;
    info->addLayout(d);
    info->addLayout(t);
    recordGroup->setLayout(info);

    return recordGroup;
}

void TabContent::updateWithNewFilename(QString f) {
    filenameString = f;
    static_cast<QGroupBox*>(viewerStack->widget(PLAYBACK))->setTitle("Playing: " + f);
    static_cast<QGroupBox*>(viewerStack->widget(EDIT))->setTitle("Editing: " + f);
    static_cast<QGroupBox*>(viewerStack->widget(RECORD))->setTitle("Recording: " + f);
}

void TabContent::displayNewTime(int newMillis) {
    int numSeconds = newMillis/1000;
    int numMinutes = numSeconds/60;
    numSeconds = numSeconds - numMinutes*60;
    // set label
    QString minNum = QString("%1").arg(numMinutes, 2, 10, QChar('0'));
    QString secNum = QString("%1").arg(numSeconds, 2, 10, QChar('0'));
    handle1Time->setText(minNum + ":" + secNum);

}
