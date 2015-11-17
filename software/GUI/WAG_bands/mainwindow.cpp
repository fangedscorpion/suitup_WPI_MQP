#include "mainwindow.h"
#include "superslider.h"

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
    QLabel *title = new QLabel;
    title->setText("Edit Recording");
    title->setAlignment(Qt::AlignCenter);
    QPushButton *undoBtn = new QPushButton;
    undoBtn->setText("Undo");
    QPushButton *cropBtn = new QPushButton;
    cropBtn->setText("Crop");
    QPushButton *splitBtn = new QPushButton;
    splitBtn->setText("Split");
    QVBoxLayout *recordPlaybackLayout = new QVBoxLayout;
    QVBoxLayout *buttons = new QVBoxLayout;
    recordPlaybackLayout->addWidget(title, -1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(undoBtn);
    buttons->addWidget(cropBtn);
    buttons->addWidget(splitBtn);
    recordPlaybackLayout->addLayout(buttons, 1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Filename: boop.xxx
    QHBoxLayout *viewerTitle = new QHBoxLayout;
    viewerTitle->addWidget(new QLabel("Filename: "));
    QLabel *filename = new QLabel;
    filename->setText("boop.xxx");
    viewerTitle->addWidget(filename, 1);

    // viewer window
    QOpenGLWidget *viewer = new QOpenGLWidget;

    // playback controls
    QHBoxLayout *controls = new QHBoxLayout;
    QPushButton *playPause = new QPushButton;
//    QSlider *videoSlider = new QSlider(Qt::Horizontal);
//    videoSlider->setStyleSheet("QSlider::handle { image: url(:/icons/handle.png); }");
//    videoSlider->paintEvent(new QPaintEvent(new QRegion()));
    SuperSlider *videoSlider = new SuperSlider;
    QIcon playIcon(QPixmap(":/icons/play.png"));
    QLabel *curTime = new QLabel("00:00");
    QLabel *totalTime = new QLabel("00:10");
    playPause->setIcon(playIcon);
    playPause->setIconSize(QSize(15,15));
    controls->addWidget(playPause);
    controls->addWidget(curTime);
    controls->addWidget(videoSlider);
    controls->addWidget(totalTime);

    // viewer side of the GUI
    QVBoxLayout *viewerPane = new QVBoxLayout;
    viewerPane->addLayout(viewerTitle);
    viewerPane->addWidget(viewer,1);
    viewerPane->addLayout(controls);

    // contains both options and viewer
    QHBoxLayout *splitPanes = new QHBoxLayout;
    splitPanes->addLayout(recordPlaybackLayout, 1);
    splitPanes->addLayout(viewerPane, 2);

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
    setMinimumSize(160, 160);
    resize(600, 400);
}

MainWindow::~MainWindow()
{
    delete ui;
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
//    connect(playbackAct, SIGNAL(triggered()), this, SLOT(playbackMode()));

    recordAct = new QAction(tr("Record Mode"), this);
    recordAct->setStatusTip(tr("Enter record mode"));
//    connect(recordAct, SIGNAL(triggered()), this, SLOT(recordMode()));

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

//void MainWindow::createRecordPlaybackPane(QWidget* mainWidget) {
//}
