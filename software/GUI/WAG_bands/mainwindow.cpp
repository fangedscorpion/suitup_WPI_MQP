#include "mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    QWidget *widget = new QWidget;
    setCentralWidget(widget);
    QVBoxLayout *layout = new QVBoxLayout;

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
    stepThrough->addItem("Step Through mode");
    stepThrough->addItem("Timed mode");
    playbackLayout->addWidget(playbackTitle, -1);
    playbackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    options->addWidget(checkbox);
    options->addWidget(stepThrough);
    // speed/frame slider
    QVBoxLayout *speedSliderLayout = new QVBoxLayout;
    QLabel *sfi = new QLabel("Speed/Frame interval");
    sfi->setAlignment(Qt::AlignCenter);
    speedSliderLayout->addWidget(sfi);
    speedSlider = new QSlider(Qt::Horizontal);
    speedSlider->setValue(speedSlider->maximum()/2);
    speedSliderLayout->addWidget(speedSlider);
    QHBoxLayout *speeds = new QHBoxLayout;
    speeds->addWidget(new QLabel("1/4x"));
    QLabel *x1 = new QLabel("1x");
    x1->setAlignment(Qt::AlignCenter);
    speeds->addWidget(x1);
    QLabel *x4 = new QLabel("4x");
    x4->setAlignment(Qt::AlignRight);
    speeds->addWidget(x4);
    speedSliderLayout->addLayout(speeds);
    //hold last frame ticket
    QHBoxLayout *holdLast = new QHBoxLayout;
    holdLast->addWidget(new QLabel("Hold last frame for"));
    seconds = new QSpinBox;
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

    // Filename: example.wagz
    QHBoxLayout *viewerTitle = new QHBoxLayout;
    viewerTitle->addWidget(new QLabel("Filename: "));
    filename = new QLabel;
    filename->setText("example.wagz");
    viewerTitle->addWidget(filename, 1);

    // viewer window
    viewer = new QOpenGLWidget;

    // video controls
    QHBoxLayout *controls = new QHBoxLayout;
    playPause = new QPushButton;
    videoSlider = new SuperSlider;
    connect(this, SIGNAL(resizedWindow()), videoSlider, SLOT(resized()));
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
    centralWidget()->setLayout(layout);

    setWindowTitle(tr("WAG bands"));
    setMinimumSize(600, 300);
    setMaximumSize(1000, 600);
    resize(600, 400);
}

MainWindow::~MainWindow()
{
}

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
//    connect(settingsAct, SIGNAL(triggered()), this, SLOT(settings()));

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
    editOptions->setVisible(false);
    playbackOptions->setVisible(true);
}

void MainWindow::recordMode() {
    editOptions->setVisible(true);
    playbackOptions->setVisible(false);
}

void MainWindow::resizeEvent(QResizeEvent* r) {
    QMainWindow::resizeEvent(r);
    emit resizedWindow();
}
