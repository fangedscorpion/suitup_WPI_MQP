#include "mainwindow.h"
#include "superslider.h"
#include "glwidget.h"
#include "playbackcontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    widget = new QStackedWidget;
    QWidget *w = new QWidget;
    setCentralWidget(widget);
    QVBoxLayout *layout = new QVBoxLayout;

    playbackControls = new PlaybackController;

    // menubar
    QMenuBar *menu = new QMenuBar;
    menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    createActions(menu);

    // Edit recording options
    editOptions = new QWidget;
    QLabel *editTitle = new QLabel("Edit Recording");
    editTitle->setAlignment(Qt::AlignCenter);
    undoBtn = new QPushButton;
    undoBtn->setText("Undo");
    cropBtn = new QPushButton;
    cropBtn->setText("Crop");
    splitBtn = new QPushButton;
    splitBtn->setText("Split");
    QVBoxLayout *recordPlaybackLayout = new QVBoxLayout;
    QVBoxLayout *buttons = new QVBoxLayout;
    recordPlaybackLayout->addWidget(editTitle, -1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(undoBtn);
    buttons->addWidget(cropBtn);
    buttons->addWidget(splitBtn);
    recordPlaybackLayout->addLayout(buttons, 1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    editOptions->setLayout(recordPlaybackLayout);

    // Playback options
    playbackOptions = new QWidget;
    QVBoxLayout *playbackLayout = new QVBoxLayout;
    QVBoxLayout *options = new QVBoxLayout;
    options->setAlignment(Qt::AlignCenter);
    QLabel *playbackTitle = new QLabel("Playback Motion");
    playbackTitle->setAlignment(Qt::AlignCenter);
    QWidget *checkbox = new QWidget;
    QHBoxLayout *checkboxLayout = new QHBoxLayout;
    checkboxLayout->setAlignment(Qt::AlignCenter);
    playOnSuit = new QCheckBox("Play on suit");
    checkboxLayout->addWidget(playOnSuit, -1);
    checkbox->setLayout(checkboxLayout);
    stepThrough = new QComboBox;
    stepThrough->addItem("Step through mode");
    stepThrough->addItem("Timed mode");
    playbackLayout->addWidget(playbackTitle, -1);
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

    // Filename: example.wagz
    QHBoxLayout *viewerTitle = new QHBoxLayout;
    viewerTitle->addWidget(new QLabel("Filename: "));
    filename = new QLabel;
    filename->setText("example.wagz");
    viewerTitle->addWidget(filename, 1);

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
    viewerPane->addLayout(viewerTitle);
    viewerPane->addWidget(viewer,1);
    viewerPane->addLayout(controls);

    // contains both options and viewer
    QHBoxLayout *splitPanes = new QHBoxLayout;
    splitPanes->addWidget(editOptions, 1);
    splitPanes->addWidget(playbackOptions, 1);
    splitPanes->addLayout(viewerPane, 2);

    // line under menubar
    QFrame *line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(320, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->setMargin(5);

    layout->addWidget(menu);
    layout->addWidget(line);
    layout->addLayout(splitPanes, 1);
    w->setLayout(layout);
    widget->addWidget(w);

    overlay = new Overlay(this);
    overlay->makeTransparent();

    ow = new OverlayWidget(this);
    QVBoxLayout *settingsLayout = new QVBoxLayout;
    QLabel *lbl = new QLabel("Settings");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setMinimumHeight(50);
    settingsLayout->addWidget(lbl);
    settingsLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *o = new QVBoxLayout;

    voiceControl = new QCheckBox("Voice Control");
    o->addWidget(voiceControl);
    h->addLayout(o, -1);
    // line
    QFrame *vLine = new QFrame();
    vLine->setObjectName(QString::fromUtf8("line"));
    vLine->setGeometry(QRect(3, 3, 3, 3));
    vLine->setFrameShape(QFrame::VLine);
    h->addWidget(vLine);
    // Graphic of bands
    view = new QGraphicsView;
    view->setMinimumHeight(250);
    QVBoxLayout *left = new QVBoxLayout;
    QVBoxLayout *right = new QVBoxLayout;
    right->setAlignment(Qt::AlignLeft);
    left->setAlignment(Qt::AlignRight);
    h->addLayout(left, 1);
    h->addWidget(view, 2);
    h->addLayout(right, 1);
    settingsLayout->addLayout(h);
    settingsLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    // checkboxes
    leftShoulder = new QCheckBox("Left Shoulder");
    leftShoulder->setChecked(true);
    leftUpperArm = new QCheckBox("Left Upper Arm");
    leftUpperArm->setChecked(true);
    leftLowerArm = new QCheckBox("Left Lower Arm");
    leftLowerArm->setChecked(true);
    leftHand = new QCheckBox("Left Hand");
    leftHand->setChecked(true);
    rightShoulder = new QCheckBox("Right Shoulder");
    rightShoulder->setChecked(true);
    rightUpperArm = new QCheckBox("Right Upper Arm");
    rightUpperArm->setChecked(true);
    rightLowerArm = new QCheckBox("Right Lower Arm");
    rightLowerArm->setChecked(true);
    rightHand = new QCheckBox("Right Hand");
    rightHand->setChecked(true);
    left->addWidget(leftShoulder);
    left->addWidget(leftUpperArm);
    left->addWidget(leftLowerArm);
    left->addWidget(leftHand);
    right->addWidget(rightShoulder);
    right->addWidget(rightUpperArm);
    right->addWidget(rightLowerArm);
    right->addWidget(rightHand);
    // Buttons on bottom of settings
    QHBoxLayout *settingsButtons = new QHBoxLayout;
    calibrate = new QPushButton("Calibrate");
    connectBands = new QPushButton("Connect Bands");
    ok = new QPushButton("OK");
    cancel = new QPushButton("Cancel");
    settingsButtons->addWidget(calibrate);
    settingsButtons->addWidget(connectBands);
    settingsButtons->addWidget(cancel);
    settingsButtons->addWidget(ok);
    settingsLayout->addLayout(settingsButtons);

    ow->setLayout(settingsLayout);
    connect(this, SIGNAL(resizedWindow()), videoSlider, SLOT(resized()));
    connect(this, SIGNAL(resizedWindow()), ow, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), overlay, SLOT(resizeWindow()));
    connect(ok, SIGNAL(released()), this, SLOT(saveSettings()));
    connect(cancel, SIGNAL(released()), this, SLOT(cancelSettings()));

    setWindowTitle(tr("WAG bands"));
    setMinimumSize(750, 500);
    setMaximumSize(1000, 600);
}

MainWindow::~MainWindow() {}

void MainWindow::createActions(QMenuBar *menu)
{
    newAct = new QAction(tr("New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
//    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openAct = new QAction(tr("Open"), this);
    openAct->setShortcuts(QKeySequence::Open);
    openAct->setStatusTip(tr("Open an existing file"));
//    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

    saveAct = new QAction(tr("Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current file"));
//    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the current file to a new name"));
//    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));

    exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the program"));
//    connect(saveAct, SIGNAL(triggered()), this, SLOT(exit()));

    playbackAct = new QAction(tr("Playback Mode"), this);
    playbackAct->setStatusTip(tr("Enter playback mode"));
    connect(playbackAct, SIGNAL(triggered()), this, SLOT(playbackMode()));

    recordAct = new QAction(tr("Record Mode"), this);
    recordAct->setStatusTip(tr("Enter record mode"));
    connect(recordAct, SIGNAL(triggered()), this, SLOT(recordMode()));

    settingsAct = new QAction(tr("Settings"), this);
    settingsAct->setStatusTip(tr("Open settings window"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(openSettings()));

    helpAct = new QAction(tr("Help"), this);
    helpAct->setStatusTip(tr("Help!"));
//    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    fileMenu = menu->addMenu(tr("File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(settingsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    modeMenu = menu->addMenu(tr("Mode"));
    modeMenu->addAction(playbackAct);
    modeMenu->addAction(recordAct);

    helpMenu = menu->addMenu(tr("Help"));
    helpMenu->addAction(helpAct);
}

void MainWindow::playbackMode() {
    connect(playPause, SIGNAL(released()), playbackControls, SLOT (togglePlay()));
    editOptions->setVisible(false);
    playbackOptions->setVisible(true);
}

void MainWindow::recordMode() {
    disconnect(playPause, SIGNAL(released()), playbackControls, 0);
    editOptions->setVisible(true);
    playbackOptions->setVisible(false);
}

void MainWindow::resizeEvent(QResizeEvent* r) {
    QMainWindow::resizeEvent(r);
    overlay->resize(this->width(), this->height());
    emit resizedWindow();
}

void MainWindow::openSettings(){
    overlay->show();
    ow->show();
}

void MainWindow::cancelSettings() {
    overlay->hide();
    ow->hide();
}

void MainWindow::saveSettings() {
    cancelSettings();
}

void MainWindow::updateSpeedSliderText(QString playbackModeString) {
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
