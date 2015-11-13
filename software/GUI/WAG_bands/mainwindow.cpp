#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
//    ui->setupUi(this);
//    ui->recordGroup->setVisible(false);
    QWidget *widget = new QWidget;
    setCentralWidget(widget);

    QMenuBar *menu = new QMenuBar;
    menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(menu);
    centralWidget()->setLayout(layout);

    createActions(menu);

    setWindowTitle(tr("Menus"));
    setMinimumSize(160, 160);
    resize(480, 320);
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
