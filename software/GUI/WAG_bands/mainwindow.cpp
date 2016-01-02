#include "mainwindow.h"
#include "superslider.h"
#include "tabcontent.h"
#include "visualization/glwidget.h"
#include "playbackcontroller.h"

#include "smartpushbutton.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // application
    applicationWidget = new QWidget;
    setCentralWidget(applicationWidget);
    QVBoxLayout *applicationLayout = new QVBoxLayout;
    applicationWidget->setLayout(applicationLayout);

    setWindowTitle(tr("WAG bands"));
    setMinimumSize(750, 500);
    setMaximumSize(1000, 600);

    titleFont = QFont( "Arial", 15, QFont::Bold);
    titleStyleSheet = "QGroupBox{ border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; subcontrol-origin: margin; left: 10px; padding: 25px 3px 0 3px;}";

    // users
    USER u("Trainer", "A trainer can record and save motions for others to use");
    u.addAction(EDIT);
    u.addAction(RECORD);

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
    tabs->setTabsClosable(true);
    tabs->addTab(createUserSelectionWindow(users), "User selection");
    tabs->clearFocus();

    overlay = new Overlay(this);
    overlay->makeSemiTransparent();
    overlay->hide();
    createSettings();
//    createSaveAs();

    // connections for window resizing
    connect(this, SIGNAL(resizedWindow()), settingsWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), overlay, SLOT(resizeWindow()));

    applicationLayout->setMargin(5);
    applicationLayout->addWidget(menu);
    applicationLayout->addWidget(line);
    applicationLayout->addWidget(tabs, 1);
}

MainWindow::~MainWindow() {}

void MainWindow::addTab(USER u, QString filename, ACTION_TYPE a) {
    TabContent *tab = new TabContent(this, filename, u, a);
    connect(this, SIGNAL(resizedWindow()), tab, SLOT(applicationResized()));
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
    saveBtn = new smartPushButton("Save Motion");
    saveBtn->hide();
    settingsBtn = new smartPushButton("Settings");
    connect(settingsBtn, SIGNAL(released()), this, SLOT(launchSettings()));
    helpBtn = new smartPushButton("Help");
//    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    QHBoxLayout* menuLayout = new QHBoxLayout;
    menuLayout->addWidget(newBtn);
    menuLayout->addWidget(saveBtn);
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
    for(i=0; i < u.size(); i++) {
        smartPushButton *btn = new smartPushButton(u[i].getName(), u[i]);
        QLabel *lbl = new QLabel(u[i].getDescription());
        lbl->setWordWrap(true);
        lbl->setAlignment(Qt::AlignCenter);
        QVBoxLayout *v = new QVBoxLayout;
        v->addWidget(btn);
        v->addWidget(lbl);
        v->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
        l->addLayout(v);
        w->setLayout(l);
        connect(btn, SIGNAL(released(USER)), this, SLOT(launchUserOptions(USER)));
    }
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
    QPushButton *calibrate = new QPushButton("Calibrate");
    QPushButton *connectBands = new QPushButton("Connect Bands");
    QPushButton *ok = new QPushButton("OK");
    QPushButton *cancel = new QPushButton("Cancel");
    settingsButtons->addWidget(calibrate);
    settingsButtons->addWidget(connectBands);
    settingsButtons->addWidget(cancel);
    settingsButtons->addWidget(ok);
    settingsLayout->addLayout(settingsButtons);

    connect(ok, SIGNAL(released()), this, SLOT(saveSettings()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeSettings()));
    // TODO: connect calibrate and connectBands
}

// Save As overlay
// TODO: add * to required fields, and check them.
//void MainWindow::createSaveAs() {
//    //Save As menu
//    saveAsWidget = new OverlayWidget(this, "Save As");
//    QVBoxLayout *saveAsLayout = saveAsWidget->getLayout();

//    // Filename: textbox
//    QHBoxLayout *f = new QHBoxLayout;
//    QLabel *l1 = new QLabel("Name: ");
//    l1->setMinimumWidth(100);
//    f->addWidget(l1, -1);
//    saveAsFilenameTextEdit = new QLineEdit;
//    f->addWidget(saveAsFilenameTextEdit);

//    // description
//    QHBoxLayout *d = new QHBoxLayout;
//    QLabel *l2 = new QLabel("Description: ");
//    l2->setMinimumWidth(100);
//    d->addWidget(l2, -1);
//    saveAsDescription = new QTextEdit;
//    saveAsDescription->setMinimumHeight(150);
//    d->addWidget(saveAsDescription);

//    // tags input
//    QHBoxLayout *t = new QHBoxLayout;
//    QLabel *l3 = new QLabel("Tags: ");
//    l3->setMinimumWidth(100);
//    t->addWidget(l3, -1);
//    saveAsTagsTextEdit = new QLineEdit;
//    t->addWidget(saveAsTagsTextEdit);
//    QPushButton *add = new QPushButton("Add");
//    t->addWidget(add);
//    // tags list
//    QHBoxLayout *t2 = new QHBoxLayout;
//    QLabel *l4 = new QLabel;
//    l4->setMinimumWidth(100);
//    t2->addWidget(l4, -1);
//    saveAsTagsLabel = new QLabel;
//    t2->addWidget(saveAsTagsLabel);

//    // buttons
//    QHBoxLayout *b = new QHBoxLayout;
//    QPushButton *saveLocally = new QPushButton("Save to computer");
//    QPushButton *saveToLibrary = new QPushButton("Save to library");
//    QPushButton *cancel = new QPushButton("Cancel");
//    b->addWidget(cancel);
//    b->addWidget(saveLocally);
//    b->addWidget(saveToLibrary);

//    connect(add, SIGNAL(released()), this, SLOT(addTag()));
//    connect(saveLocally, SIGNAL(released()), this, SLOT(launchSaveToComputer()));
//    connect(cancel, SIGNAL(released()), this, SLOT(closeSaveAs()));
//    connect(saveToLibrary, SIGNAL(released()), this, SLOT(save()));

//    saveAsLayout->addLayout(f);
//    saveAsLayout->addLayout(d);
//    saveAsLayout->addLayout(t);
//    saveAsLayout->addLayout(t2);
//    saveAsLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
//    saveAsLayout->addLayout(b);
//}


// create new file overlay
void MainWindow::createNewFile(USER u) {
    newFileWidget = new OverlayWidget(this, "Create New File");
    QVBoxLayout *layout = newFileWidget->getLayout();

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(100);
    f->addWidget(l1, -1);
    newFilenameTextEdit = new QLineEdit;
    f->addWidget(newFilenameTextEdit);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    QLabel *l2 = new QLabel("Description: ");
    l2->setMinimumWidth(100);
    d->addWidget(l2, -1);
    newFileDescription = new QTextEdit;
    newFileDescription->setMinimumHeight(150);
    d->addWidget(newFileDescription);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    QLabel *l3 = new QLabel("Tags: ");
    l3->setMinimumWidth(100);
    t->addWidget(l3, -1);
    newFileTagsTextEdit = new QLineEdit;
    t->addWidget(newFileTagsTextEdit);
    QPushButton *add = new QPushButton("Add");
    t->addWidget(add);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *l4 = new QLabel;
    l4->setMinimumWidth(100);
    t2->addWidget(l4, -1);
    newFileTagsLabel = new QLabel;
    t2->addWidget(newFileTagsLabel);

    QHBoxLayout *btns = new QHBoxLayout;
    smartPushButton *create = new smartPushButton("Create", u);
    QPushButton *cancel = new QPushButton("Cancel");
    btns->addWidget(cancel);
    btns->addWidget(create);
    layout->addLayout(f);
    layout->addLayout(d);
    layout->addLayout(t);
    layout->addLayout(t2);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    layout->addLayout(btns);

    // only connect handleUserOptions when the user selection window is visible to user
    if (userOptionsWidget->isVisible())
        connect(create, SIGNAL(released(USER)), this, SLOT(handleUserOptions(USER)));
    connect(create, SIGNAL(released(USER)), this, SLOT(saveNewFile(USER)));
    connect(cancel, SIGNAL(released()), this, SLOT(closeNewFile()));
    connect(add, SIGNAL(released()), this, SLOT(addTag()));
    connect(this, SIGNAL(resizedWindow()), newFileWidget, SLOT(resizeWindow()));
    emit this->resizedWindow();
}

// Open from library overlay
void MainWindow::createOpenFromLib(USER u) {
    openFromLibWidget = new OverlayWidget(this, "Open From Library");
    QVBoxLayout *layout = openFromLibWidget->getLayout();

    QHBoxLayout *btns = new QHBoxLayout;
    smartPushButton *open = new smartPushButton("Open", u);
    QPushButton *cancel = new QPushButton("Cancel");
    btns->addWidget(cancel);
    btns->addWidget(open);
    layout->addLayout(btns);

    // only connect handleUserOptions when the user selection window is visible to user
    if (userOptionsWidget->isVisible())
        connect(open, SIGNAL(released(USER)), this, SLOT(handleUserOptions(USER)));
    connect(open, SIGNAL(released(USER)), this, SLOT(openFromLibrary(USER)));
    connect(cancel, SIGNAL(released()), this, SLOT(closeOpenFromLibrary()));
    connect(this, SIGNAL(resizedWindow()), openFromLibWidget, SLOT(resizeWindow()));
    emit this->resizedWindow();
}
