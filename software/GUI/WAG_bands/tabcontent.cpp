#include "tabcontent.h"
#include <unistd.h>
#include <QTimerEvent>

TabContent::TabContent(MainWindow *in_parent, WAGFile* in_motion, USER u, ACTION_TYPE initiallyShow, Suit *sysSuit) : parent(in_parent){
    motion = in_motion;
    user = u;
    suitObj = sysSuit;

    connect(this, SIGNAL(modeChanged(ACTION_TYPE)), suitObj, SLOT(catchModeChanged(ACTION_TYPE)));
    connect(this, SIGNAL(modeChanged(ACTION_TYPE)), this, SLOT(catchModeChanged(ACTION_TYPE)));
    emit modeChanged(initiallyShow);

    titleFont = QFont( "Arial", 15, QFont::Bold);
    groupboxStyleSheet = "QGroupBox{ border: 1px solid gray; border-radius: 9px; margin-left: 0.25em; margin-right: 0.25em; margin-top: 0.5em; padding: 25px 3px 0 3px;} QGroupBox::title{subcontrol-position: top center; subcontrol-origin: margin;}";
    textInputStyleRed = "QLineEdit {border: 1px solid red; background: white;} QTextEdit {border: 1px solid red; background: white;}";
    textInputStyleWhite = "QLineEdit {background: white;} QTextEdit {background: white;}";
    buttonHeight = 35;

    createIcons();
    createMotionInfoWindow();

    optionsStack = new QStackedWidget;
    viewerStack = new QStackedWidget;
    if (u.hasAction(RECORD)) {
        optionsStack->addWidget(createRecordOptionsAndController());
        viewerStack->addWidget(createRecordingWindow());
    }
    if (u.hasAction(EDIT)) {
        // createViewer must come first
        viewerStack->addWidget(createViewer(EDIT));
        optionsStack->addWidget(createEditOptionsAndControls());
    }
    if (u.hasAction(PLAYBACK)) {
        // createViewer must come first
        viewerStack->addWidget(createViewer(PLAYBACK));
        optionsStack->addWidget(createPlaybackOptionsAndControls());
    }

    // contains both options and viewer
    QHBoxLayout *splitPanes = new QHBoxLayout;
    QVBoxLayout *vl = new QVBoxLayout;

    vl->addWidget(optionsStack);
    vl->addWidget(createModeRadios(u));
    this->show(initiallyShow);

    splitPanes->addLayout(vl, 1);
    splitPanes->addWidget(viewerStack, 2);

    this->setLayout(splitPanes);
    overlay = new Overlay(this);
    overlay->makeSemiTransparent();
    overlay->hide();
    connect(this, SIGNAL(resizedWindow()), overlay, SLOT(resizeWindow()));

    // timers
    recordCountdownTimer = new QTimer(this);
    recordStopwatchTimer = new QTimer(this);
    playbackCountdownTimer = new QTimer(this);
    connect(recordCountdownTimer, SIGNAL(timeout()), this, SLOT(recordCountdownTimerEvent()));
    connect(recordStopwatchTimer, SIGNAL(timeout()), this, SLOT(recordStopwatchTimerEvent()));
    connect(playbackCountdownTimer, SIGNAL(timeout()), this, SLOT(playbackCountdownTimerEvent()));

    saveMotion();
}

TabContent::~TabContent() {}

void TabContent::createIcons() {
    recordIcon = QIcon(QPixmap(":/icons/record.png"));
    cropIcon = QIcon(QPixmap(":/icons/crop.png"));
    splitIcon = QIcon(QPixmap(":/icons/split.png"));
    undoIcon = QIcon(QPixmap(":/icons/undo.png"));
    editIcon = QIcon(QPixmap(":/icons/edit.png"));
    stopIcon = QIcon(QPixmap(":/icons/stop.png"));
    resetIcon = QIcon(QPixmap(":/icons/reset.png"));
}

// event for when the main window is resized
void TabContent::resizeEvent(QResizeEvent* r) {
    QWidget::resizeEvent(r);
    // resize the overlay to cover the whole window
    overlay->resize(this->width(), this->height());
    emit resizedWindow();
}

void TabContent::show(ACTION_TYPE a) {
    // if the user has the action a
    if (user.hasAction(a)) {
        optionsStack->setCurrentIndex(a);
        viewerStack->setCurrentIndex(a);
        emit modeChanged(a);
    // EDIT is used when opening a file, so if the user doesn't have edit ability, check playback
    } else if (a == EDIT && user.hasAction(PLAYBACK)) {
        optionsStack->setCurrentIndex(PLAYBACK);
        viewerStack->setCurrentIndex(PLAYBACK);
        emit modeChanged(PLAYBACK);
    // otherwise show any one of the available actions.
    } else {
        std::set<ACTION_TYPE>::iterator it=user.getActions().begin();
        if (it != user.getActions().end())
            show(*it);
        // else ERROR
        // TODO: throw exception here
        return;
    }

    if (a == RECORD && user.hasAction(RECORD)) {
        recordRadio->setChecked(true);
    } else if (a == EDIT && user.hasAction(EDIT)) {
        editRadio->setChecked(true);
    } else if ((a == PLAYBACK || a == EDIT) && user.hasAction(PLAYBACK)) {
        playbackRadio->setChecked(true);
    } else {
        std::set<ACTION_TYPE>::iterator it=user.getActions().begin();
        if (it != user.getActions().end())
            show(*it);
        // else ERROR
        // TODO: throw exception here
        return;
    }
}

// The groupbox of Mode radio buttons
QWidget* TabContent::createModeRadios(USER u) {
    modeRadiosGroup = new QGroupBox("Modes");
    modeRadiosGroup->setAlignment(Qt::AlignHCenter);
    modeRadiosGroup->setStyleSheet(groupboxStyleSheet);
    modeRadiosGroup->setFont(titleFont);
    QVBoxLayout *vl = new QVBoxLayout;
    recordRadio = new smartRadioButton("Record Motion", RECORD);
    editRadio = new smartRadioButton("Edit Motion", EDIT);
    playbackRadio = new smartRadioButton("Playback Motion", PLAYBACK);

    // if there is only 1 action this user can perform, don't show any modes
    int count = 0;
    if (u.hasAction(RECORD))
        count++;
    if (u.hasAction(EDIT))
        count++;
    if (u.hasAction(PLAYBACK))
        count++;

    if (count == 1) {
        modeRadiosGroup->hide();
        return modeRadiosGroup;
    }

    vl->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    if (u.hasAction(RECORD)) {
        vl->addWidget(recordRadio);
        connect(recordRadio, SIGNAL(released(ACTION_TYPE)), this, SLOT(show(ACTION_TYPE)));
    }
    if (u.hasAction(EDIT)) {
        vl->addWidget(editRadio);
        connect(editRadio, SIGNAL(released(ACTION_TYPE)), this, SLOT(show(ACTION_TYPE)));
    }
    if (u.hasAction(PLAYBACK)) {
        vl->addWidget(playbackRadio);
        connect(playbackRadio, SIGNAL(released(ACTION_TYPE)), this, SLOT(show(ACTION_TYPE)));
    }
    vl->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    modeRadiosGroup->setLayout(vl);
    return modeRadiosGroup;
}

void TabContent::updateMotion(WAGFile* file) {
    motion = file;
    if (user.hasAction(PLAYBACK))
        static_cast<QGroupBox*>(viewerStack->widget(PLAYBACK))->setTitle(QString("Playing: ") + motion->getName());
    if (user.hasAction(EDIT))
        static_cast<QGroupBox*>(viewerStack->widget(EDIT))->setTitle(QString("Editing: ") + motion->getName());
    if (user.hasAction(RECORD))
        recordGroup->setTitle(QString("Recording: ") + motion->getName());
}

// save the motion file
void TabContent::saveMotion() {
    if (motion->getSaveLocation() == LIBRARY) {

    } else if (motion->getSaveLocation() == LOCALLY) {
        //     * QFileDialog
        QFile f( motion->getName() );
        f.open( QIODevice::WriteOnly );
        // store data in f
        f.close();
    }

//    closeSaveAs();
    // TODO: Save da file!
}

// OpenGL Motion Viewer window with video slider
QWidget* TabContent::createViewer(ACTION_TYPE t) {
    QGroupBox* v;
    QVBoxLayout *viewerPane = new QVBoxLayout;

    if (t == EDIT) {
        v = new QGroupBox("Editing: " + motion->getName());
        editMotionViewer = new MotionViewer(this);
        viewerPane->addWidget(editMotionViewer);
    } else {
        v= new QGroupBox("Playing: " + motion->getName());
        playbackMotionViewer = new MotionViewer(this);
        viewerPane->addWidget(playbackMotionViewer);
    }

    v->setStyleSheet(groupboxStyleSheet);
    v->setFont(titleFont);
    v->setLayout(viewerPane);
    return v;
}

void TabContent::catchModeChanged(ACTION_TYPE newMode)  {
    currentMode = newMode;
}

ACTION_TYPE TabContent::getCurrentMode() {
    return currentMode;
}
