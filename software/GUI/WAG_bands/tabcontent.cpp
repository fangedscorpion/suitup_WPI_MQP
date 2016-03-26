#include "tabcontent.h"
#include <unistd.h>
#include <QTimerEvent>

TabContent::TabContent(MainWindow* in_parent, WAGFile* in_motion, USER u, ACTION_TYPE initiallyShow, Suit *sysSuit, ModelLoader *ml) : parent(in_parent){
    motion = in_motion;
    user = u;
    suitObj = sysSuit;
    modelLoader = ml;

    connect(this, SIGNAL(modeChanged(ACTION_TYPE)), suitObj, SLOT(catchModeChanged(ACTION_TYPE)));
    connect(this, SIGNAL(modeChanged(ACTION_TYPE)), this, SLOT(catchModeChanged(ACTION_TYPE)));
    emit modeChanged(initiallyShow);

    textInputStyleRed = "QLineEdit {border: 2px solid red; background: white; border-radius: 4px;} QTextEdit {border-radius: 4px; border: 2px solid red; background: white;}";
    textInputStyleWhite = "QLineEdit {border: 1px solid gray; border-radius: 4px; background: white;} QTextEdit {border-radius: 4px; border: 1px solid grey; background: white;}";

    createIcons();

    optionsStack = new QStackedWidget(this);
    viewerStack = new QStackedWidget(this);
    if (u.hasAction(RECORD)) {
        optionsStack->addWidget(createRecordOptionsAndController());
        viewerStack->addWidget(createRecordingWindow());
    }
    if (u.hasAction(EDIT)) {
        // createViewer must come first
        editModel = modelLoader->load();
        viewerStack->addWidget(createViewer(EDIT));
        optionsStack->addWidget(createEditOptionsAndControls());
    }
    if (u.hasAction(PLAYBACK)) {
        // createViewer must come first
        playbackModel = modelLoader->load();
        viewerStack->addWidget(createViewer(PLAYBACK));
        optionsStack->addWidget(createPlaybackOptionsAndControls());
    }

    // contains both options and viewer
    QHBoxLayout* splitPanes = new QHBoxLayout();
    QVBoxLayout* vl = new QVBoxLayout();

    vl->addWidget(optionsStack);
    vl->addWidget(createModeRadios(u));
    this->show(initiallyShow);
    if (initiallyShow == RECORD)
        modeRadiosGroup->setEnabled(false);

    splitPanes->addLayout(vl, 1);
    splitPanes->addWidget(viewerStack, 2);

    this->setLayout(splitPanes);
    overlay = new Overlay(this);
    overlay->makeSemiTransparent();
    overlay->hide();

    // timers
    recordCountdownTimer = new QTimer(this);
    recordStopwatchTimer = new QTimer(this);
    playbackCountdownTimer = new QTimer(this);
    connect(recordCountdownTimer, SIGNAL(timeout()), this, SLOT(recordCountdownTimerEvent()));
    connect(recordStopwatchTimer, SIGNAL(timeout()), this, SLOT(recordStopwatchTimerEvent()));
    connect(playbackCountdownTimer, SIGNAL(timeout()), this, SLOT(playbackCountdownTimerEvent()));

    saveMotion();
}

TabContent::~TabContent() {
    delete motion;
    if (playbackControls != 0)
        delete playbackControls;
    if (editingControls != 0)
        delete editingControls;
    if (recordingControls != 0)
        delete recordingControls;

    if (playbackMotionViewer != 0)
        delete playbackMotionViewer;
}

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
StyledGroupBox* TabContent::createModeRadios(USER u) {
    modeRadiosGroup = new StyledGroupBox(this, "Modes");

    QVBoxLayout* vl = modeRadiosGroup->getLayout();
    recordRadio = new SmartRadioButton(this, "Record Motion", RECORD);
    editRadio = new SmartRadioButton(this, "Edit Motion", EDIT);
    playbackRadio = new SmartRadioButton(this, "Playback Motion", PLAYBACK);

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

    return modeRadiosGroup;
}

void TabContent::updateMotion() {
    if (user.hasAction(PLAYBACK))
        static_cast<StyledGroupBox*>(viewerStack->widget(PLAYBACK))->setTitle(QString("Playing: ") + motion->getName());
    if (user.hasAction(EDIT))
        static_cast<StyledGroupBox*>(viewerStack->widget(EDIT))->setTitle(QString("Editing: ") + motion->getName());
    if (user.hasAction(RECORD))
        recordGroup->setTitle(QString("Recording: ") + motion->getName());
    parent->setCurrentTabName(motion->getName());
}

// save the motion file
void TabContent::saveMotion() {
    motion->saveToFile();
}

// OpenGL Motion Viewer window with video slider
StyledGroupBox* TabContent::createViewer(ACTION_TYPE t) {

    StyledGroupBox* v = new StyledGroupBox(this, "temp");
    QVBoxLayout* viewerPane = v->getLayout();

    if (t == EDIT) {
        v->setTitle("Editing: " + motion->getName());
        editMotionViewer = new MotionViewer(this, editModel);
        viewerPane->addWidget(editMotionViewer);
    } else {
        v->setTitle("Playing: " + motion->getName());
        playbackMotionViewer = new MotionViewer(this, playbackModel);
        viewerPane->addWidget(playbackMotionViewer);
    }

    return v;
}

void TabContent::catchModeChanged(ACTION_TYPE newMode)  {
    currentMode = newMode;
}

ACTION_TYPE TabContent::getCurrentMode() {
    return currentMode;
}
