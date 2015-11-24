#include "mainwindow.h"
#include "superslider.h"
#include "glwidget.h"
#include "playbackcontroller.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    mainWidget = new QWidget;
    setCentralWidget(mainWidget);
    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainWidget->setLayout(mainLayout);
    // the window opens onto the settings page
    setWindowTitle(tr("WAG bands - Record motion"));
    setMinimumSize(750, 500);
    setMaximumSize(1000, 600);

    playbackControls = new PlaybackController;
    playbackEditOptions = new QStackedWidget;
    filename = QString("untitled.wagz");

    // menubar
    QMenuBar *menu = new QMenuBar;
    menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    createActions(menu);
    // line under menubar
    QFrame *line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(320, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    createEditRecordingOptionsAndControls();
    createPlaybackOptionsAndControls();
    createViewer();

    // contains both options and viewer
    QHBoxLayout *splitPanes = new QHBoxLayout;
    splitPanes->addWidget(playbackEditOptions, 1);
    splitPanes->addLayout(viewerPane, 2);

    // Fade out the main Window
    overlay = new Overlay(this);
    overlay->makeTransparent();
    createSettings();
    createSaveAs();

    //connections
    connect(this, SIGNAL(resizedWindow()), videoSlider, SLOT(resized()));
    connect(this, SIGNAL(resizedWindow()), settingsWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), saveAsWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), overlay, SLOT(resizeWindow()));
    connect(ok, SIGNAL(released()), this, SLOT(saveSettings()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeSettings()));

    mainLayout->setMargin(5);
    mainLayout->addWidget(menu);
    mainLayout->addWidget(line);
    mainLayout->addLayout(splitPanes, 1);
}

MainWindow::~MainWindow() {}

// Menubar actions
// TODO: finish actions
void MainWindow::createActions(QMenuBar *menu)
{
    newAct = new QAction(tr("New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
//    connect(newAct, SIGNAL(triggered()), this, SLOT(newFile()));

    openFromCompAct = new QAction(tr("Open from computer"), this);
    openFromCompAct->setStatusTip(tr("Open an existing file from your computer"));
    connect(openFromCompAct, SIGNAL(triggered()), this, SLOT(openFromComputer()));

    openFromLibAct = new QAction(tr("Open from motion library"), this);
    openFromLibAct->setShortcuts(QKeySequence::Open);
    openFromLibAct->setStatusTip(tr("Open an existing file from the library"));
//    connect(openFromLibAct, SIGNAL(triggered()), this, SLOT(OpenFromLib()));

    saveAct = new QAction(tr("Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the current file to a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(openSaveAs()));

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
    QMenu *open = new QMenu("Open");
    open->addAction(openFromCompAct);
    open->addAction(openFromLibAct);
    fileMenu->addMenu(open);
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


// settings overlay
// TODO: add a person
// TODO: add a connected bands indicator
// TODO: add a profile section where we ask for username, gender, height
void MainWindow::createSettings() {
    // Settings widget
    settingsWidget = new OverlayWidget(this);
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
    // verticle line between voice control and active bands
    QFrame *vLine = new QFrame();
    vLine->setObjectName(QString::fromUtf8("line"));
    vLine->setGeometry(QRect(3, 3, 3, 3));
    vLine->setFrameShape(QFrame::VLine);
    vLine->setFrameShadow(QFrame::Raised);
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

    settingsWidget->setLayout(settingsLayout);
}

// Playback motion options and controller
void MainWindow::createPlaybackOptionsAndControls() {
    // Playback options
    QWidget *playbackOptions = new QWidget;
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

    playbackEditOptions->addWidget(playbackOptions);
}

// Edit Recording options and controller
void MainWindow::createEditRecordingOptionsAndControls() {
    // Edit recording options
    QWidget *editOptions = new QWidget;
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

    playbackEditOptions->addWidget(editOptions);
}

// OpenGL Motion Viewer window with video slider
void MainWindow::createViewer() {
    // Filename: example.wagz
    QHBoxLayout *viewerTitle = new QHBoxLayout;
    viewerTitle->addWidget(new QLabel("Filename: "));
    currentLoadedFilename = new QLabel;
    currentLoadedFilename->setText(filename);
    viewerTitle->addWidget(currentLoadedFilename, 1);

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
    viewerPane = new QVBoxLayout;
    viewerPane->addLayout(viewerTitle);
    viewerPane->addWidget(viewer,1);
    viewerPane->addLayout(controls);
}

// Save As overlay
// TODO: add * to required fields, and check them.
void MainWindow::createSaveAs() {
    //Save As menu
    saveAsWidget = new OverlayWidget(this);
    saveAsWidget->hide();

    QVBoxLayout *saveAsLayout = new QVBoxLayout;
    QLabel *lbl = new QLabel("Save As");
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setMinimumHeight(50);
    saveAsLayout->addWidget(lbl);
    saveAsLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(100);
    f->addWidget(l1, -1);
    saveAsFilename = new QLineEdit;
    f->addWidget(saveAsFilename);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    QLabel *l2 = new QLabel("Description: ");
    l2->setMinimumWidth(100);
    d->addWidget(l2, -1);
    description = new QTextEdit;
    description->setMinimumHeight(150);
    d->addWidget(description);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    QLabel *l3 = new QLabel("Tags: ");
    l3->setMinimumWidth(100);
    t->addWidget(l3, -1);
    inputTags = new QLineEdit;
    t->addWidget(inputTags);
    QPushButton *add = new QPushButton("Add");
    t->addWidget(add);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *l4 = new QLabel;
    l4->setMinimumWidth(100);
    t2->addWidget(l4, -1);
    tags = new QLabel;
    t2->addWidget(tags);

    // buttons
    QHBoxLayout *b = new QHBoxLayout;
    QPushButton *saveLocally = new QPushButton("Save to computer");
    QPushButton *saveToLibrary = new QPushButton("Save to library");
    QPushButton *cancel = new QPushButton("Cancel");
    b->addWidget(saveLocally);
    b->addWidget(saveToLibrary);
    b->addWidget(cancel);

    connect(add, SIGNAL(released()), this, SLOT(addTag()));
    connect(saveLocally, SIGNAL(released()), this, SLOT(saveToComputer()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeSaveAs()));
    connect(saveToLibrary, SIGNAL(released()), this, SLOT(save()));

    saveAsLayout->addLayout(f);
    saveAsLayout->addLayout(d);
    saveAsLayout->addLayout(t);
    saveAsLayout->addLayout(t2);
    saveAsLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    saveAsLayout->addLayout(b);
    saveAsWidget->setLayout(saveAsLayout);
}

// save the motion file!
void MainWindow::save() {
    /*
     * QFileDialog
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    // store data in f
    f.close();
*/
    closeSaveAs();
    // TODO: Save da file!
}

// File > saveAs > Save to computer. opens a dialog box
void MainWindow::saveToComputer() {
    // TODO: add filter for .wagz
    filename = QFileDialog::getSaveFileName(this, "Save File", "~/" + saveAsFilename->text()+ ".wagz");
    if (filename.compare(QString("")) == 0) {
        filename = "untitled.wagz";
    } else {
        closeSaveAs();
    }
    // TODO: Actually save...
}

// in saveAs window, adds a tag to the tags list
// TODO: hitting enter key should trigger the Add button
void MainWindow::addTag() {
    tags->setText(inputTags->text() + "; " + tags->text());
    inputTags->clear();
}

// opens the dialog box to find a local file
void MainWindow::openFromComputer() {
    // File browser
    filename = QFileDialog::getOpenFileName(this, "Open File");
    if (filename.compare(QString("")) == 0) {
        filename = "untitled.wagz";
    }
}

void MainWindow::playbackMode() {
    connect(playPause, SIGNAL(released()), playbackControls, SLOT (togglePlay()));
    playbackEditOptions->setCurrentIndex(1);
    setWindowTitle(tr("WAG bands - Play motion"));
}

void MainWindow::recordMode() {
    disconnect(playPause, SIGNAL(released()), playbackControls, 0);
    playbackEditOptions->setCurrentIndex(0);
    // updates the main window's title
    setWindowTitle(tr("WAG bands - Record motion"));
}

// event for when the main window is resized
void MainWindow::resizeEvent(QResizeEvent* r) {
    QMainWindow::resizeEvent(r);
    // resize the overlay to cover the whole window
    overlay->resize(this->width(), this->height());
    emit resizedWindow();
}

// opens the settings window
void MainWindow::openSettings(){
    overlay->show();
    settingsWidget->show();
    //TODO: set focus on the "OK button"
}

// closes the settings window
void MainWindow::closeSettings() {
    overlay->hide();
    settingsWidget->hide();
    // TODO: show message box if user has not calibrated the suit yet
}

// saves the new user settings
void MainWindow::saveSettings() {
    // TODO: save user settings here...
    closeSettings();
}

// updates the playback mode's speed slider labels
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

// opens the Save As window
void MainWindow::openSaveAs() {
    overlay->show();
    saveAsWidget->show();
    //TODO: set focus on the "Save to library" button
}

// save a new file
void MainWindow::saveSaveAs() {
    // TODO: save the file and settings.
    closeSaveAs();
}

// closes the Save As window
void MainWindow::closeSaveAs() {
    overlay->hide();
    saveAsWidget->hide();
    // updates the current filename
    currentLoadedFilename->setText(filename);
}
