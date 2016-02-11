#include "mainwindow.h"
#include "customWidgets/superslider.h"
#include "tabcontent.h"
#include "visualization/glwidget.h"
#include "playbackcontroller.h"
#include "band/absband.h"

#include "customWidgets/smartpushbutton.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // application
    applicationWidget = new QWidget;
    setCentralWidget(applicationWidget);
    QVBoxLayout *applicationLayout = new QVBoxLayout;
    applicationWidget->setLayout(applicationLayout);

    setWindowTitle(tr("WAG bands"));
    setMinimumSize(1100, 650);
    setMaximumSize(1100, 650);

    titleFont = QFont( "Arial", 15, QFont::Bold);
    titleStyleSheet = "QGroupBox{ border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; subcontrol-origin: margin; left: 10px; padding: 25px 3px 0 3px;}";
    textInputStyleRed = "QLineEdit {border: 1px solid red; background: white;} QTextEdit {border: 1px solid red; background: white;}";
    textInputStyleWhite = "QLineEdit {background: white;} QTextEdit {background: white;}";
    buttonHeight = 35;

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

    // menubar
    menu = new QWidget;
    createMenuButtons();

    // line under menubar
    QFrame *line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(320, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    // tabs
    tabs = new QTabWidget;
    tabs->addTab(createUserSelectionWindow(users), "User selection");
    tabs->clearFocus();
    connect(tabs, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    overlay = new Overlay(this);
    overlay->makeSemiTransparent();
    overlay->hide();
    createSettings();

    // connections for window resizing
    connect(this, SIGNAL(resizedWindow()), settingsWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), overlay, SLOT(resizeWindow()));

    applicationLayout->setMargin(5);
    applicationLayout->addWidget(menu);
    applicationLayout->addWidget(line);
    applicationLayout->addWidget(tabs, 1);
    wifiMan = new WifiManager();
    fullSuit = new Suit(wifiMan);
    connect(wifiMan, SIGNAL(connectionStatusChanged(BandType,ConnectionStatus)), this, SLOT(indicateConnectionStatusChange(BandType, ConnectionStatus)));
}

MainWindow::~MainWindow() {}

void MainWindow::addTab(USER u, WAGFile* w, ACTION_TYPE a) {
    TabContent *tab = new TabContent(this, w, u, a);
    tabs->addTab(tab, tab->getFilename());
    tabs->setCurrentIndex(tabs->indexOf(tab));
    tabs->clearFocus();
}

// Menubar actions
// TODO: finish actions
void MainWindow::createMenuButtons()
{
    newBtn = new smartPushButton("Record New Motion");
    newBtn->hide();
    openBtn = new smartPushButton("Load Motion");
    openBtn->hide();
    settingsBtn = new smartPushButton("Settings");
    connect(settingsBtn, SIGNAL(released()), this, SLOT(launchSettings()));
    helpBtn = new smartPushButton("Help");
//    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    QHBoxLayout* menuLayout = new QHBoxLayout;
    menuLayout->addWidget(newBtn);
    menuLayout->addWidget(openBtn);
    menuLayout->addWidget(settingsBtn);
    menuLayout->addWidget(helpBtn);
    menu->setLayout(menuLayout);
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
// TODO: add a profile section where we ask for username, gender, height
// TODO: add pop up if they did not calibrate or connect bands
void MainWindow::createSettings() {
    // Settings widget
    settingsWidget = new OverlayWidget(this, "Settings");
    QVBoxLayout *settingsLayout = settingsWidget->getLayout();

    QHBoxLayout *h = new QHBoxLayout;
    QVBoxLayout *o = new QVBoxLayout;
    // Add user input here
//    o->addWidget(voiceControl);
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
    QPushButton *calibrate = new QPushButton("Calibrate");
    calibrate->setMinimumHeight(buttonHeight);
    connectBands = new QPushButton("Connect Bands");
    connectBands->setMinimumHeight(buttonHeight);
    QPushButton *ok = new QPushButton("OK");
    ok->setMinimumHeight(buttonHeight);
    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setMinimumHeight(buttonHeight);
    settingsButtons->addWidget(connectBands);
    settingsButtons->addWidget(calibrate);
    settingsButtons->addWidget(cancel);
    settingsButtons->addWidget(ok);
    settingsLayout->addLayout(settingsButtons);

    connect(ok, SIGNAL(released()), this, SLOT(saveSettings()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeSettings()));
    connect(connectBands, SIGNAL(released()), this, SLOT(connectCheckedBands()));
    // TODO: connect calibrate and connectBands
}

// create new file overlay
void MainWindow::createNewMotion(USER u) {
    newMotionWidget = new OverlayWidget(this, "New Motion");
    QVBoxLayout *layout = newMotionWidget->getLayout();

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(100);
    l1->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    f->addWidget(l1, -1);
    newMotionNameTextEdit = new QLineEdit;
    newMotionNameTextEdit->setStyleSheet(textInputStyleRed);
    f->addWidget(newMotionNameTextEdit);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    QLabel *l2 = new QLabel("Description: ");
    l2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l2->setMinimumWidth(100);
    d->addWidget(l2, -1);
    newMotionDescription = new QTextEdit;
    newMotionDescription->setStyleSheet(textInputStyleRed);
    newMotionDescription->setMinimumHeight(150);
    newMotionDescription->setTabChangesFocus(true);
    d->addWidget(newMotionDescription);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    QLabel *l3 = new QLabel("Keywords: ");
    l3->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l3->setMinimumWidth(100);
    t->addWidget(l3, -1);
    newMotionTagsTextEdit = new QLineEdit;
    newMotionTagsTextEdit->setStyleSheet(textInputStyleWhite);
    t->addWidget(newMotionTagsTextEdit);
    addTagBtn = new QPushButton("Add Keyword");
    addTagBtn->setEnabled(false);
    addTagBtn->setMinimumHeight(buttonHeight);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *l4 = new QLabel;
    l4->setMinimumWidth(100);
    t2->addWidget(l4, -1);
    t2->addWidget(addTagBtn, -1);
    newMotionTagsLabel = new QLabel;
    t2->addWidget(newMotionTagsLabel, 1);

    QHBoxLayout *btns = new QHBoxLayout;
    createNewMotionBtn = new smartPushButton("Create", u);
    createNewMotionBtn->setEnabled(false);
    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setMinimumHeight(buttonHeight);
    btns->addWidget(cancel);
    btns->addWidget(createNewMotionBtn);
    layout->addLayout(f);
    layout->addSpacing(10);
    layout->addLayout(d);
    layout->addSpacing(10);
    layout->addLayout(t);
    layout->addLayout(t2);
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
//    connect(this, SIGNAL(resizedWindow()), newMotionWidget, SLOT(resizeWindow()));
//    emit this->resizedWindow();
}

void MainWindow::createOpenMotionOptions(USER u) {
    openWidget = new OverlayWidget(this, "Load Motion");
    QVBoxLayout *layout = openWidget->getLayout();
    smartPushButton *openLib = new smartPushButton("Load Motion From Library", u);
    smartPushButton *openComp = new smartPushButton("Load Motion From Computer", u);
    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setMinimumHeight(buttonHeight);

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

    QHBoxLayout *btns = new QHBoxLayout;
    smartPushButton *open = new smartPushButton("Load", u);
    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setMinimumHeight(buttonHeight);
    btns->addWidget(cancel);
    btns->addWidget(open);
    layout->addLayout(btns);

    // only connect handleUserOptions when the user selection window is visible to user
    if (userOptionsWidget != NULL)
        connect(open, SIGNAL(released(USER)), this, SLOT(handleUserOptions(USER)));
    connect(open, SIGNAL(released(USER)), this, SLOT(openFromLibrary(USER)));
    connect(cancel, SIGNAL(released()), this, SLOT(closeOpenFromLibrary()));
//    connect(this, SIGNAL(resizedWindow()), openFromLibWidget, SLOT(resizeWindow()));
//    emit this->resizedWindow();
}

// event for when the main window is resized
void MainWindow::resizeEvent(QResizeEvent* r) {
    QMainWindow::resizeEvent(r);
    // resize the overlay to cover the whole window
    overlay->resize(this->width(), this->height());
    emit resizedWindow();
}

void MainWindow::connectCheckedBands() {
    // get checked bands
    //    call functio nin wifiman

    QList<BandType> connectBands = QList<BandType>();

    connectBands<<CHEST;

    if (leftShoulder->isChecked()) {
        connectBands<<LEFT_SHOULDER;
    }
    if (leftUpperArm->isChecked()) {
        connectBands<<LEFT_UPPER_ARM;
    }
    if (leftLowerArm->isChecked()) {
        connectBands<<LEFT_LOWER_ARM;
    }
    if (rightShoulder->isChecked()) {
        connectBands<<RIGHT_SHOULDER;
    }
    if (rightUpperArm->isChecked()) {
        connectBands<<RIGHT_UPPER_ARM;
    }
    if (rightLowerArm->isChecked()) {
        connectBands<<RIGHT_LOWER_ARM;
    }

    qDebug()<<connectBands.count();

    wifiMan->initiateConnection(connectBands);


}

// Samee, connection status changes indicated here, (CONNECTED/DISCONNECTED)
void MainWindow::indicateConnectionStatusChange(BandType changedBand, ConnectionStatus updatedStatus) {
    qDebug()<<"Band "<<changedBand<<" switched conenection status to " <<updatedStatus;
}
