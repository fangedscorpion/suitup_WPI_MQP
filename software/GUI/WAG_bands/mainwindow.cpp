#include "mainwindow.h"
#include "customWidgets/superslider.h"
#include "tabcontent.h"
#include "visualization/glwidget.h"
#include "playbackcontroller.h"
#include "band/absband.h"
#include "model/modelloader.h"
#include <QTableWidget>
#include <QHeaderView>

#include <dirent.h>
#include <fstream>
#include <errno.h>
//#include <unistd.h>
#include <sys/stat.h>
//#include <sys/types.h>

#include "customWidgets/smartpushbutton.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    this->setStyleSheet("QMainWindow {background-color: #C8E6C9;}"); // grey rgb(234, 237, 237)
    // application
    QWidget *applicationWidget = new QWidget;
    setCentralWidget(applicationWidget);
    QVBoxLayout *applicationLayout = new QVBoxLayout;
    applicationWidget->setLayout(applicationLayout);

    setWindowTitle(tr("WAG bands"));
    setMinimumSize(1100, 650);
    setMaximumSize(1100, 650);

    titleFont = QFont( "Arial", 15, QFont::Bold);
    titleStyleSheet = "QGroupBox{ border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; subcontrol-origin: margin; left: 10px; padding: 25px 3px 0 3px;}";
    textInputStyleRed = "QLineEdit {border: 1px solid red; background: white;} QTextEdit {border-radius: 2px; border: 1px solid red; background: white;}";
    textInputStyleWhite = "QLineEdit {background: white;} QTextEdit {border-radius: 2px; border: 1px outset grey; background: white;}";

    // users
    USER u("Trainer", "A trainer can record and save motions for others to use");
    u.addAction(EDIT);
    u.addAction(RECORD);
    u.addAction(PLAYBACK);

    USER u2("Trainee", "A trainee can playback motions");
    u2.addAction(PLAYBACK);
    std::vector<USER> users;
    users.push_back(u);
    users.push_back(u2);

    // line under menubar
    QFrame *line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(320, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    // tabs
    tabs = new QTabWidget;
    tabs->setFocusPolicy(Qt::NoFocus);
    tabs->addTab(createUserSelectionWindow(users), "User selection");
    tabs->clearFocus();
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    connect(tabs, SIGNAL(currentChanged(int)), this, SLOT(catchTabChange(int)));

    overlay = new Overlay(this);
    overlay->makeSemiTransparent();
    overlay->hide();
    createSettings();

    applicationLayout->setMargin(5);
    applicationLayout->addLayout(createMenuButtons());
    applicationLayout->addLayout(createStatusBar());
//    applicationLayout->addWidget(line);
    applicationLayout->addWidget(tabs, 1);

    wifiMan = new WifiManager();
    ModelLoader modelLoader = ModelLoader();
    fullSuit = new Suit(wifiMan,modelLoader.load());
    connectedBands = QSet<BandType>();
    connect(wifiMan, SIGNAL(connectionStatusChanged(BandType,ConnectionStatus)), this, SLOT(indicateConnectionStatusChange(BandType, ConnectionStatus)));
    connect(this, SIGNAL(modeChanged(ACTION_TYPE)), fullSuit, SLOT(catchModeChanged(ACTION_TYPE)));
    connect(wifiMan, SIGNAL(connectionStatusChanged(BandType,ConnectionStatus)), this, SLOT(updateConnectionStatus(BandType, ConnectionStatus)));
    connect(fullSuit, SIGNAL(bandHasLowBattery(BandType)), this, SLOT(catchLowBatterySignal(BandType)));
    connect(fullSuit, SIGNAL(bandConnectionStatusChanged(BandType,ConnectionStatus)), this, SLOT(updateConnectionStatus(BandType,ConnectionStatus)));

}

MainWindow::~MainWindow() {}

void MainWindow::setCurrentTabName(QString s) {
    tabs->setTabText(tabs->currentIndex(), s);
}

void MainWindow::addTab(USER u, WAGFile* w, ACTION_TYPE a) {
    TabContent *tab = new TabContent(this, w, u, a, fullSuit);
    tabs->addTab(tab, tab->getFilename());
    tabs->setCurrentIndex(tabs->indexOf(tab));
    tabs->clearFocus();
}

// Menubar actions
// TODO: finish actions
QHBoxLayout* MainWindow::createMenuButtons() {
    newBtn = new smartPushButton("Record New Motion");
    newBtn->hide();
    openBtn = new smartPushButton("Load Motion");
    openBtn->hide();
    settingsBtn = new smartPushButton("Settings");
    connect(settingsBtn, SIGNAL(released()), this, SLOT(launchSettings()));
    smartPushButton *helpBtn = new smartPushButton("Help");
    //    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    QHBoxLayout* menuLayout = new QHBoxLayout;
    menuLayout->addWidget(newBtn);
    menuLayout->addWidget(openBtn);
    menuLayout->addWidget(settingsBtn);
    menuLayout->addWidget(helpBtn);
    return menuLayout;
}

QHBoxLayout* MainWindow::createStatusBar() {
    QHBoxLayout *l = new QHBoxLayout;
    connectionStatus = new QLabel("7 Bands Disconnected");
    connectionStatus->setStyleSheet("QLabel { color : red; }");
    // make settingsBtn red.
    settingsBtn->setRed(true);

    connectionStatus->setAlignment(Qt::AlignLeft);
    batteryStatus = new QLabel("Battery full");
    batteryStatus->setStyleSheet("QLabel { color : green; }");
    batteryStatus->setAlignment(Qt::AlignRight);
    l->addWidget(connectionStatus);
    l->addWidget(batteryStatus);
    return l;
}

// The first window a user sees on launch.
QWidget* MainWindow::createUserSelectionWindow(std::vector<USER> u) {
    QWidget *w = new QWidget;
    QHBoxLayout *l = new QHBoxLayout;
    int i;
    for(i=0; i < (int)u.size(); i++) {
        smartPushButton *btn = new smartPushButton(u[i].getName(), u[i]);
        QLabel *lbl = new QLabel(u[i].getDescription());
        lbl->setWordWrap(true);
        lbl->setAlignment(Qt::AlignCenter);
        QVBoxLayout *v = new QVBoxLayout;
        v->addWidget(btn);
        v->addWidget(lbl);
        v->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
        l->addLayout(v);
        connect(btn, SIGNAL(released(USER)), this, SLOT(launchUserOptions(USER)));
    }
    w->setLayout(l);
    return w;
}

// settings overlay
// TODO: add a person
// TODO: add a connected bands indicator
// TODO: add pop up if they did not calibrate or connect bands
void MainWindow::createSettings() {
    // Settings widget
    settingsWidget = new OverlayWidget(this, "Settings");
    QVBoxLayout *settingsLayout = settingsWidget->getLayout();

    QHBoxLayout *h = new QHBoxLayout;
    // Graphic of bands
    QGraphicsView *view = new QGraphicsView;
    view->setMinimumHeight(250);
    view->setMinimumWidth(350);
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
    rightShoulder = new QCheckBox("Right Shoulder");
    rightShoulder->setChecked(true);
    rightUpperArm = new QCheckBox("Right Upper Arm");
    rightUpperArm->setChecked(true);
    rightLowerArm = new QCheckBox("Right Lower Arm");
    rightLowerArm->setChecked(true);
    left->addWidget(leftShoulder);
    left->addWidget(leftUpperArm);
    left->addWidget(leftLowerArm);
    right->addWidget(rightShoulder);
    right->addWidget(rightUpperArm);
    right->addWidget(rightLowerArm);
    // Buttons on bottom of settings
    QHBoxLayout *settingsButtons = new QHBoxLayout;
    smartPushButton *calibrate = new smartPushButton("Calibrate Bands");
    calibrate->setRed(true);
    connectBands = new smartPushButton("Connect Bands");
    connectBands->setRed(true);
    smartPushButton *done = new smartPushButton("Done");
    settingsButtons->addWidget(connectBands);
    settingsButtons->addWidget(calibrate);
    settingsButtons->addWidget(done);
    settingsLayout->addLayout(settingsButtons);

    connect(done, SIGNAL(released()), this, SLOT(closeSettings()));
    connect(done, SIGNAL(released()), this, SLOT(handleConnectedBands()));
    connect(leftLowerArm, SIGNAL(released()), this, SLOT(handleConnectedBands()));
    connect(leftUpperArm, SIGNAL(released()), this, SLOT(handleConnectedBands()));
    connect(leftShoulder, SIGNAL(released()), this, SLOT(handleConnectedBands()));
    connect(rightLowerArm, SIGNAL(released()), this, SLOT(handleConnectedBands()));
    connect(rightUpperArm, SIGNAL(released()), this, SLOT(handleConnectedBands()));
    connect(rightShoulder, SIGNAL(released()), this, SLOT(handleConnectedBands()));
    connect(connectBands, SIGNAL(released()), this, SLOT(connectCheckedBands()));
    // TODO: connect calibrate
}

// create new file overlay
void MainWindow::createNewMotion(USER u) {
    newMotionWidget = new OverlayWidget(this, "New Motion");
    QVBoxLayout *layout = newMotionWidget->getLayout();

    int inputMaxWidth = 800;
    int labelMaxWidth = 110;

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    f->setAlignment(Qt::AlignLeft);
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(labelMaxWidth);
    l1->setMaximumWidth(labelMaxWidth);
    l1->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    f->addWidget(l1, -1);
    newMotionNameTextEdit = new QLineEdit;
    newMotionNameTextEdit->setStyleSheet(textInputStyleRed);
    newMotionNameTextEdit->setMaximumWidth(inputMaxWidth);
    f->addWidget(newMotionNameTextEdit);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    d->setAlignment(Qt::AlignLeft);
    QLabel *l2 = new QLabel("Description: ");
    l2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l2->setMinimumWidth(labelMaxWidth);
    l2->setMaximumWidth(labelMaxWidth);
    d->addWidget(l2, -1);
    newMotionDescription = new QTextEdit;
    newMotionDescription->setStyleSheet(textInputStyleRed);
    newMotionDescription->setMinimumHeight(150);
    newMotionDescription->setMaximumWidth(inputMaxWidth);
    newMotionDescription->setTabChangesFocus(true);
    d->addWidget(newMotionDescription);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    t->setAlignment(Qt::AlignLeft);
    QLabel *l3 = new QLabel("Keywords: ");
    l3->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l3->setMinimumWidth(labelMaxWidth);
    l3->setMaximumWidth(labelMaxWidth);
    t->addWidget(l3, -1);
    newMotionTagsTextEdit = new QLineEdit;
    newMotionTagsTextEdit->setStyleSheet(textInputStyleWhite);
    newMotionTagsTextEdit->setMaximumWidth(inputMaxWidth);
    t->addWidget(newMotionTagsTextEdit);
    addTagBtn = new smartPushButton("Add Keyword");
    addTagBtn->setEnabled(false);
    addTagBtn->setMaximumWidth(110);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *spacer = new QLabel;
    spacer->setMinimumWidth(labelMaxWidth);
    spacer->setMaximumWidth(labelMaxWidth);
    t2->addWidget(spacer, -1);
    t2->addWidget(addTagBtn);
    newMotionTagsLayout = new QHBoxLayout();
    t2->addLayout(newMotionTagsLayout, 2);
    t2->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
    QLabel *spacer2 = new QLabel;
    spacer2->setMinimumWidth(60);
    spacer2->setMaximumWidth(60);
    t2->addWidget(spacer2);

    // save to options
    QHBoxLayout *s = new QHBoxLayout;
    s->setAlignment(Qt::AlignLeft);
    QLabel *l5 = new QLabel("Save to: ");
    l5->setMinimumWidth(labelMaxWidth);
    l5->setMaximumWidth(labelMaxWidth);
    l5->setAlignment(Qt::AlignRight);
    s->addWidget(l5, -1);
    QVBoxLayout *s1 = new QVBoxLayout;
    QRadioButton *newMotionLibRadio = new QRadioButton("Motion Library");
    newMotionLibRadio->setChecked(true);
    QHBoxLayout *c = new QHBoxLayout;
    c->setContentsMargins(0,0,0,0);
    newMotionCompRadio = new QRadioButton("Local Computer");
    newMotionBrowseBtn = new smartPushButton("Select Save Location");
    newMotionBrowseBtn->setEnabled(false);
    newMotionSaveLocation = new QLabel("");
    c->addWidget(newMotionBrowseBtn);
    c->addWidget(newMotionSaveLocation);
    s1->addWidget(newMotionLibRadio);
    s1->addWidget(newMotionCompRadio);
    s1->addLayout(c);
    s->addLayout(s1);

    QHBoxLayout *btns = new QHBoxLayout;
    createNewMotionBtn = new smartPushButton("Create", u);
    createNewMotionBtn->setEnabled(false);
    smartPushButton *cancel = new smartPushButton("Cancel");
    btns->addWidget(cancel);
    btns->addWidget(createNewMotionBtn);
    layout->setAlignment(Qt::AlignLeft);
    layout->addLayout(f);
    layout->addSpacing(10);
    layout->addLayout(d);
    layout->addSpacing(10);
    layout->addLayout(t);
    layout->addLayout(t2);
    layout->addSpacing(10);
    layout->addLayout(s);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    layout->addLayout(btns);

    // only connect handleUserOptions when the user selection window is visible to user
    if (userOptionsWidget != NULL)
        connect(createNewMotionBtn, SIGNAL(released(USER)), this, SLOT(handleUserOptions(USER)));
    connect(createNewMotionBtn, SIGNAL(released(USER)), this, SLOT(saveNewMotion(USER)));
    connect(cancel, SIGNAL(released()), this, SLOT(closeNewMotion()));
    connect(addTagBtn, SIGNAL(released()), this, SLOT(addTag()));
    connect(newMotionTagsTextEdit, SIGNAL(returnPressed()), this, SLOT(addTag()));
    connect(newMotionNameTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleNewMotionRequiredInput(QString)));
    connect(newMotionDescription, SIGNAL(textChanged()), this, SLOT(handleNewMotionRequiredInput()));
    connect(newMotionTagsTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleNewMotionRequiredInput(QString)));
    connect(newMotionBrowseBtn, SIGNAL(released()), this, SLOT(handleNewMotionRadios()));
    connect(newMotionCompRadio, SIGNAL(toggled(bool)), newMotionBrowseBtn, SLOT(setEnabled(bool)));
    connect(newMotionCompRadio, SIGNAL(released()), this, SLOT(handleNewMotionRequiredInput()));
    connect(newMotionLibRadio, SIGNAL(released()), this, SLOT(handleNewMotionRequiredInput()));
    connect(newMotionCompRadio, SIGNAL(toggled(bool)), newMotionSaveLocation, SLOT(setVisible(bool)));
}

void MainWindow::createOpenMotionOptions(USER u) {
    openWidget = new OverlayWidget(this, "Load Motion");
    QVBoxLayout *layout = openWidget->getLayout();
    smartPushButton *openLib = new smartPushButton("Load Motion From Library", u);
    smartPushButton *openComp = new smartPushButton("Load Motion From Computer", u);
    smartPushButton *cancel = new smartPushButton("Cancel");

    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addWidget(openLib);
    layout->addWidget(openComp);
    layout->addWidget(cancel);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    connect(openLib, SIGNAL(released(USER)), this, SLOT(launchOpenFromLibrary(USER)));
    connect(openComp, SIGNAL(released(USER)), this, SLOT(launchOpenFromComputer(USER)));
    connect(cancel, SIGNAL(released()), this, SLOT(closeOpenMotionOptions()));
}

// Open from library overlay
void MainWindow::createOpenFromLib(USER u) {
    openFromLibWidget = new OverlayWidget(this, "Load Motion From Library");
    QVBoxLayout *layout = openFromLibWidget->getLayout();

    // Search bar
    QHBoxLayout *s = new QHBoxLayout;
    s->setAlignment(Qt::AlignRight);
    // choose a category
    openFromLibFilterOpts = new QComboBox;
    openFromLibFilterOpts->setMaximumWidth(120);
    openFromLibFilterOpts->addItem("All");
    openFromLibFilterOpts->addItem("Name");
    openFromLibFilterOpts->addItem("Description");
    openFromLibFilterOpts->addItem("Keywords");
    s->addWidget(openFromLibFilterOpts);
    // search box
    openFromLibFilterBar = new QLineEdit;
    openFromLibFilterBar->setStyleSheet(textInputStyleWhite);
    openFromLibFilterBar->setMaximumWidth(300);
    openFromLibFilterBar->setPlaceholderText("Filter motions by text");
    s->addWidget(openFromLibFilterBar);
    layout->addLayout(s, -1);
    layout->addSpacing(15);

    // get motion lib directory
    std::vector<WAGFile*> libraryFiles;
    std::ifstream myfile;
    myfile.open ("../WAG_bands/.WAGConfig");
    std::string library;
    std::getline(myfile, library);
    myfile.close();
    // open all motion files
    std::string filepath;
    DIR *dp;
    struct dirent *dirp;
    struct stat filestat;
    dp = opendir( library.c_str() );
    if (dp == NULL) {
        qDebug() << "Error(" << errno << ") opening " << library.c_str();
        return;
    }
    while ((dirp = readdir( dp ))) {
        filepath = library + "/" + dirp->d_name;

        // If the file is a directory (or is in some way invalid) we'll skip it
        if (stat( filepath.c_str(), &filestat )) continue;
        if (S_ISDIR( filestat.st_mode ))         continue;

        // TODO: check that this works... catch error
        libraryFiles.push_back(new WAGFile(filepath.c_str(), true));
    }
    closedir( dp );

    openFromLibTable = new QTableWidget();
    openFromLibTable->setColumnCount(4); // name, desc, tags
    openFromLibTable->setRowCount(libraryFiles.size());
    openFromLibTable->setMinimumHeight(400);
    openFromLibTable->verticalHeader()->setVisible(false);
    QStringList tableHeader;
    tableHeader << "Name" << "Description" << "Keywords";
    QHeaderView *headerView = new QHeaderView(Qt::Horizontal, openFromLibTable);
    openFromLibTable->setHorizontalHeader(headerView);
    headerView->setSectionResizeMode(QHeaderView::Stretch);
    headerView->setStyleSheet("QHeaderView { font-weight: bold; }");

    openFromLibTable->setHorizontalHeaderLabels(tableHeader);
    openFromLibTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    openFromLibTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    openFromLibTable->setSelectionMode(QAbstractItemView::SingleSelection);

    for (int i=0; i < (int)libraryFiles.size(); i++) {
        WAGFile *curFile = libraryFiles[i];
        openFromLibTable->setItem(i,0, new QTableWidgetItem(curFile->getName()));
        openFromLibTable->setItem(i,1, new QTableWidgetItem(curFile->getDescription()));
        openFromLibTable->setItem(i,2, new QTableWidgetItem(curFile->getTagString()));
        openFromLibTable->setItem(i,3, new QTableWidgetItem(curFile->getPathString()));
    }

    openFromLibTable->hideColumn(3);

    layout->addWidget(openFromLibTable);
    layout->addSpacing(20);

    QHBoxLayout *btns = new QHBoxLayout;
    openFromLibBtn = new smartPushButton("Load", u);
    openFromLibBtn->setEnabled(false);
    smartPushButton *cancel = new smartPushButton("Cancel");
    btns->addWidget(cancel);
    btns->addWidget(openFromLibBtn);
    layout->addLayout(btns);

    // only connect handleUserOptions when the user selection window is visible to user
    if (userOptionsWidget != NULL)
        connect(openFromLibBtn, SIGNAL(released(USER)), this, SLOT(handleUserOptions(USER)));
    connect(openFromLibBtn, SIGNAL(released(USER)), this, SLOT(openFromLibrary(USER)));
    connect(cancel, SIGNAL(released()), this, SLOT(closeOpenFromLibrary()));
    connect(openFromLibTable, SIGNAL(cellClicked(int,int)), this, SLOT(handleOpenFromLibBtn(int,int)));
    connect(openFromLibFilterBar, SIGNAL(textChanged(QString)), this, SLOT(handleOpenFromLibFilter(QString)));
    connect(openFromLibFilterOpts, SIGNAL(currentIndexChanged(QString)), this, SLOT(handleOpenFromLibFilter(QString)));
}

// event for when the main window is resized
void MainWindow::resizeEvent(QResizeEvent* r) {
    QMainWindow::resizeEvent(r);
    // resize the overlay to cover the whole window
    overlay->resize(this->width(), this->height());
}

void MainWindow::connectCheckedBands() {
    // get checked bands
    //    call functio nin wifiman

    QList<BandType> connectedBands = QList<BandType>();

    connectedBands<<CHEST;

    if (leftShoulder->isChecked()) {
        connectedBands<<LEFT_SHOULDER;
    }
    if (leftUpperArm->isChecked()) {
        connectedBands<<LEFT_UPPER_ARM;
    }
    if (leftLowerArm->isChecked()) {
        connectedBands<<LEFT_LOWER_ARM;
    }
    if (rightShoulder->isChecked()) {
        connectedBands<<RIGHT_SHOULDER;
    }
    if (rightUpperArm->isChecked()) {
        connectedBands<<RIGHT_UPPER_ARM;
    }
    if (rightLowerArm->isChecked()) {
        connectedBands<<RIGHT_LOWER_ARM;
    }
    wifiMan->initiateConnection(connectedBands);
}

// Samee, connection status changes indicated here, (CONNECTED/DISCONNECTED)
void MainWindow::indicateConnectionStatusChange(BandType changedBand, ConnectionStatus updatedStatus) {
    qDebug()<<"Band "<<changedBand<<" switched conenection status to " <<updatedStatus;
}

void MainWindow::catchTabChange(int tabIndex) {
    //
    QWidget *current = tabs->currentWidget();


    // there's probably a better way to do this, but I can't think of one right now
    if (TabContent *tab = qobject_cast<TabContent *>(current)) {
        ACTION_TYPE mode = tab->getCurrentMode();
        qDebug()<<"MainWindow: Mode changed to "<<mode;
        emit modeChanged(mode);
    } else {
        // not a tab
        qDebug("MainWindow: Not a tab");
    }

}
