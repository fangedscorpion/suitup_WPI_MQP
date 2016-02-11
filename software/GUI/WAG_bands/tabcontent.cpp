#include "tabcontent.h"
#include <unistd.h>
#include <QTimerEvent>

TabContent::TabContent(MainWindow *in_parent, WAGFile* in_motion, USER u, ACTION_TYPE initiallyShow, Suit *sysSuit) : parent(in_parent){
    motion = in_motion;
    user = u;
    suitObj = sysSuit;

    playbackControls = new PlaybackController(suitObj);
    editingControls = new EditingController();
    recordingControls = new RecordingController(suitObj);

    connect(playbackControls, SIGNAL(changeSliderMax(qint32)), this, SLOT(changeSliderRange(qint32)));
    connect(editingControls, SIGNAL(changeSliderMax(qint32)), this, SLOT(changeSliderRange(qint32)));

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
        optionsStack->addWidget(createEditOptionsAndControls());
        viewerStack->addWidget(createViewer(EDIT));
    }
    if (u.hasAction(PLAYBACK)) {
        optionsStack->addWidget(createPlaybackOptionsAndControls());
        viewerStack->addWidget(createViewer(PLAYBACK));
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
    playIcon = QIcon(QPixmap(":/icons/play.png"));
    pauseIcon = QIcon(QPixmap(":/icons/pause.png"));
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
    // EDIT is used when opening a file, so if the user doesn't have edit ability, check playback
    } else if (a == EDIT && user.hasAction(PLAYBACK)) {
        optionsStack->setCurrentIndex(PLAYBACK);
        viewerStack->setCurrentIndex(PLAYBACK);
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
        disconnect(playPause, SIGNAL(released()), playbackControls, 0);
        disconnect(videoSlider, SIGNAL(alt_valueChanged(int)), playbackControls, 0);
        disconnect(videoSlider, SIGNAL(valueChanged(int)), playbackControls, 0);


        connect(playPause, SIGNAL(released()), editingControls, SLOT (togglePlay()));
        connect(videoSlider, SIGNAL(alt_valueChanged(int)), editingControls, SLOT(beginningSliderChanged(int)));
        connect(videoSlider, SIGNAL(valueChanged(int)), editingControls, SLOT(endSliderChanged(int)));


        editRadio->setChecked(true);
    } else if ((a == PLAYBACK || a == EDIT) && user.hasAction(PLAYBACK)) {
        disconnect(playPause, SIGNAL(released()), editingControls, 0);
        disconnect(videoSlider, SIGNAL(alt_valueChanged(int)), editingControls, 0);
        disconnect(videoSlider, SIGNAL(valueChanged(int)), editingControls, 0);

        connect(playPause, SIGNAL(released()), playbackControls, SLOT (togglePlay()));
        connect(videoSlider, SIGNAL(alt_valueChanged(int)), playbackControls, SLOT(beginningSliderChanged(int)));
        connect(videoSlider, SIGNAL(valueChanged(int)), playbackControls, SLOT(endSliderChanged(int)));

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

void TabContent::applicationResized() {
    videoSlider->resized();
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

void TabContent::changeSliderRange(qint32 newSliderLen) {
    qDebug("asldjfhw");
    qDebug()<<videoSlider;
    //videoSlider->setRange(0, newSliderLen/SLIDER_DIVIDE_FACTOR);
    qDebug("uwoqefhdn");
}
