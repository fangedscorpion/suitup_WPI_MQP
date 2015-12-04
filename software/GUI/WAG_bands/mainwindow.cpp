#include "mainwindow.h"
#include "superslider.h"
#include "visualization/glwidget.h"
#include "playbackcontroller.h"
#include "tabcontent.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    // application
    applicationWidget = new QWidget;
    setCentralWidget(applicationWidget);
    QVBoxLayout *applicationLayout = new QVBoxLayout;
    applicationWidget->setLayout(applicationLayout);
    // the window opens onto the settings page
    setWindowTitle(tr("WAG bands"));
    setMinimumSize(750, 500);
    setMaximumSize(1000, 600);

    titleFont = QFont( "Arial", 15, QFont::Bold);
    titleStyleSheet = "QGroupBox{ border: 1px solid gray; border-radius: 9px; margin-top: 0.5em; subcontrol-origin: margin; left: 10px; padding: 25px 3px 0 3px;}";

    // menubar
    QMenuBar *menu = new QMenuBar;
    menu->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    createMenuActions(menu);
    // line under menubar
    QFrame *line = new QFrame();
    line->setObjectName(QString::fromUtf8("line"));
    line->setGeometry(QRect(320, 150, 118, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);

    // tabs
    tabs = new QTabWidget;
    tabs->setTabsClosable(true);
    tabs->addTab(createWelcomeWindow(), "Getting Started");
    tabs->clearFocus();

    // Fade out the main Window
    overlay = new Overlay(this);
    overlay->makeTransparent();
    createSettings();
    createSaveAs();
    createOpenFromLib();
    createNewFile();

    // connections for window resizing
    connect(this, SIGNAL(resizedWindow()), settingsWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), saveAsWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), openFromLibWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), newFileWidget, SLOT(resizeWindow()));
    connect(this, SIGNAL(resizedWindow()), overlay, SLOT(resizeWindow()));

    applicationLayout->setMargin(5);
    applicationLayout->addWidget(menu);
    applicationLayout->addWidget(line);
    applicationLayout->addWidget(tabs, 1);
}

MainWindow::~MainWindow() {}

// Menubar actions
// TODO: finish actions
void MainWindow::createMenuActions(QMenuBar *menu)
{
    QAction *newAct = new QAction(tr("New"), this);
    newAct->setShortcuts(QKeySequence::New);
    newAct->setStatusTip(tr("Create a new file"));
    connect(newAct, SIGNAL(triggered()), this, SLOT(launchNewFile()));

    QAction *openFromCompAct = new QAction(tr("Open from computer"), this);
    openFromCompAct->setStatusTip(tr("Open an existing file from your computer"));
    connect(openFromCompAct, SIGNAL(triggered()), this, SLOT(launchOpenFromComputer()));

    QAction *openFromLibAct = new QAction(tr("Open from motion library"), this);
    openFromLibAct->setShortcuts(QKeySequence::Open);
    openFromLibAct->setStatusTip(tr("Open an existing file from the library"));
    connect(openFromLibAct, SIGNAL(triggered()), this, SLOT(launchOpenFromLibrary()));

    saveAct = new QAction(tr("Save"), this);
    saveAct->setShortcuts(QKeySequence::Save);
    saveAct->setStatusTip(tr("Save the current file"));
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAct = new QAction(tr("Save As"), this);
    saveAsAct->setShortcuts(QKeySequence::SaveAs);
    saveAsAct->setStatusTip(tr("Save the current file to a new name"));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(launchSaveAs()));

    QAction *exitAct = new QAction(tr("Exit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    exitAct->setStatusTip(tr("Exit the program"));
//    connect(exitAct, SIGNAL(triggered()), this, SLOT(exit()));

    QAction *settingsAct = new QAction(tr("Settings"), this);
    settingsAct->setStatusTip(tr("Open settings window"));
    connect(settingsAct, SIGNAL(triggered()), this, SLOT(launchSettings()));

    QAction *helpAct = new QAction(tr("Help"), this);
    helpAct->setStatusTip(tr("Help!"));
//    connect(helpAct, SIGNAL(triggered()), this, SLOT(help()));

    QMenu *fileMenu = menu->addMenu(tr("File"));
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

    QMenu *helpMenu = menu->addMenu(tr("Help"));
    helpMenu->addAction(helpAct);
}

// The first window a user sees on launch.
QWidget* MainWindow::createWelcomeWindow() {
    QLabel *welcome = new QLabel("To start, open an existing file or create a new file from the menu above");
    welcome->setWordWrap(true);
    welcome->setAlignment(Qt::AlignCenter);
    welcome->setMargin(10);
    saveAct->setEnabled(false);
    saveAsAct->setEnabled(false);
    return welcome;
}

// settings overlay
// TODO: add a person
// TODO: add a connected bands indicator
// TODO: add a profile section where we ask for username, gender, height
// TODO: add pop up if they did not calibrate or connect bands
void MainWindow::createSettings() {
    // Settings widget
    settingsWidget = new OverlayWidget(this);
    QVBoxLayout *settingsLayout = new QVBoxLayout;
    QLabel *lbl = new QLabel("Settings");
    lbl->setFont(titleFont);
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
    settingsWidget->setLayout(settingsLayout);
}


// Save As overlay
// TODO: add * to required fields, and check them.
void MainWindow::createSaveAs() {
    //Save As menu
    saveAsWidget = new OverlayWidget(this);
    saveAsWidget->hide();

    QVBoxLayout *saveAsLayout = new QVBoxLayout;
    QLabel *lbl = new QLabel("Save As");
    lbl->setFont(titleFont);
    lbl->setAlignment(Qt::AlignCenter);
    lbl->setMinimumHeight(50);
    saveAsLayout->addWidget(lbl);
    saveAsLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(100);
    f->addWidget(l1, -1);
    saveAsFilenameTextEdit = new QLineEdit;
    f->addWidget(saveAsFilenameTextEdit);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    QLabel *l2 = new QLabel("Description: ");
    l2->setMinimumWidth(100);
    d->addWidget(l2, -1);
    saveAsDescription = new QTextEdit;
    saveAsDescription->setMinimumHeight(150);
    d->addWidget(saveAsDescription);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    QLabel *l3 = new QLabel("Tags: ");
    l3->setMinimumWidth(100);
    t->addWidget(l3, -1);
    saveAsTagsTextEdit = new QLineEdit;
    t->addWidget(saveAsTagsTextEdit);
    QPushButton *add = new QPushButton("Add");
    t->addWidget(add);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *l4 = new QLabel;
    l4->setMinimumWidth(100);
    t2->addWidget(l4, -1);
    saveAsTagsLabel = new QLabel;
    t2->addWidget(saveAsTagsLabel);

    // buttons
    QHBoxLayout *b = new QHBoxLayout;
    QPushButton *saveLocally = new QPushButton("Save to computer");
    QPushButton *saveToLibrary = new QPushButton("Save to library");
    QPushButton *cancel = new QPushButton("Cancel");
    b->addWidget(cancel);
    b->addWidget(saveLocally);
    b->addWidget(saveToLibrary);

    connect(add, SIGNAL(released()), this, SLOT(addTag()));
    connect(saveLocally, SIGNAL(released()), this, SLOT(launchSaveToComputer()));
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


// create new file overlay
void MainWindow::createNewFile() {
    newFileWidget = new OverlayWidget(this);
    newFileWidget->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *lbl = new QLabel("Create new file");
    lbl->setFont(titleFont);
    lbl->setAlignment(Qt::AlignCenter);
    layout->addWidget(lbl);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

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
    QPushButton *create = new QPushButton("Create");
    QPushButton *cancel = new QPushButton("Cancel");
    btns->addWidget(cancel);
    btns->addWidget(create);
    layout->addLayout(f);
    layout->addLayout(d);
    layout->addLayout(t);
    layout->addLayout(t2);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    layout->addLayout(btns);

    connect(create, SIGNAL(released()), this, SLOT(saveNewFile()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeNewFile()));
    connect(add, SIGNAL(released()), this, SLOT(addTag()));
    newFileWidget->setLayout(layout);
}


// Open from library overlay
void MainWindow::createOpenFromLib() {
    openFromLibWidget = new OverlayWidget(this);
    openFromLibWidget->hide();

    QVBoxLayout *layout = new QVBoxLayout;
    QLabel *lbl = new QLabel("Open from library");
    lbl->setFont(titleFont);
    lbl->setAlignment(Qt::AlignCenter);
    layout->addWidget(lbl);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    QHBoxLayout *btns = new QHBoxLayout;
    QPushButton *open = new QPushButton("Open");
    QPushButton *cancel = new QPushButton("Cancel");
    btns->addWidget(cancel);
    btns->addWidget(open);
    layout->addLayout(btns);

    connect(open, SIGNAL(released()), this, SLOT(openFromLibrary()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeOpenFromLibrary()));
    openFromLibWidget->setLayout(layout);
}

void MainWindow::addTab(ACTION_TYPE t, QString filename) {
    TabContent *tab = new TabContent(filename, t);
    connect(this, SIGNAL(resizedWindow()), tab, SLOT(applicationResized()));
    tabs->addTab(tab, tab->getFilename());
    tabs->setCurrentIndex(tabs->indexOf(tab));
    tabs->clearFocus();
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
void MainWindow::launchSaveToComputer() {
    // TODO: add filter for .wagz
    QString f = QFileDialog::getSaveFileName(this, "Save File", "/" + saveAsFilenameTextEdit->text()+ ".wagz");
    if (!f.trimmed().isEmpty()) { // user clicked "save"
        static_cast<TabContent*>(tabs->widget(tabs->currentIndex()))->updateWithNewFilename(f.trimmed());
        tabs->setTabText(tabs->currentIndex(), f);
        closeSaveAs();
    }
    // TODO: Actually save...
}

// in saveAs window, adds a tag to the tags list
// TODO: hitting enter key should trigger the Add button
void MainWindow::addTag() {
    saveAsTagsLabel->setText(saveAsTagsTextEdit->text() + "; " + saveAsTagsLabel->text());
    saveAsTagsTextEdit->clear();
}

// opens the dialog box to find a local file
void MainWindow::launchOpenFromComputer() {
    // File browser
    QString f = QFileDialog::getOpenFileName(this, "Open File", "/");
    if (!f.trimmed().isEmpty()) { // user clicked "open"
        saveAct->setEnabled(true);
        saveAsAct->setEnabled(true);
        addTab(PLAYBACK, f.trimmed());
    }
}

// opens the user determined file
void MainWindow::openFromLibrary() {
    // TODO: open file somehow...
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    closeOpenFromLibrary();
}

// launches the open from library overlay
void MainWindow::launchOpenFromLibrary() {
    overlay->show();
    openFromLibWidget->show();
}

// closes the open from library overlay
void MainWindow::closeOpenFromLibrary() {
    overlay->hide();
    openFromLibWidget->hide();
}



// event for when the main window is resized
void MainWindow::resizeEvent(QResizeEvent* r) {
    QMainWindow::resizeEvent(r);
    // resize the overlay to cover the whole window
    overlay->resize(this->width(), this->height());
    emit resizedWindow();
}

// opens the settings window
void MainWindow::launchSettings(){
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

// opens the Save As window
void MainWindow::launchSaveAs() {
    overlay->show();
    saveAsWidget->show();
    //TODO: set focus on the "Save to library" button
}

// save a new file
void MainWindow::saveSaveAs() {
    // TODO: save the file and settings
    closeSaveAs();
}

// closes the Save As window
void MainWindow::closeSaveAs() {
    overlay->hide();
    saveAsWidget->hide();
}

void MainWindow::launchNewFile() {
    overlay->show();
    newFileWidget->show();
    newFileWidget->raise();
}

void MainWindow::saveNewFile() {
    saveAct->setEnabled(true);
    saveAsAct->setEnabled(true);
    addTab(RECORD, newFilenameTextEdit->text() + ".wagz");
    closeNewFile();
}

void MainWindow::closeNewFile() {
    overlay->hide();
    newFileWidget->hide();
}
