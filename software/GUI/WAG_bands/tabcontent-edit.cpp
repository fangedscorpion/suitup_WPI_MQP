#include "tabcontent.h"
#include "customWidgets/styledgroupbox.h"

// Edit Recording options and controller
// TODO: add edit file name and description in here
StyledGroupBox* TabContent::createEditOptionsAndControls() {
    editingControls = new EditingController(this);
    editingControls->setActiveMotion(motion);

    connect(editingControls, SIGNAL(changeSliderMax(qint32)), editMotionViewer, SLOT(changeSliderRange(qint32)));    
    connect(editingControls, SIGNAL(goToSnapshot(PositionSnapshot*)), editModel, SLOT(updatePose(PositionSnapshot*)));

    // Edit recording options
    StyledGroupBox* editOptions = new StyledGroupBox(this, "Editing Options");

    SmartPushButton* undoBtn = new SmartPushButton(this, "Undo");
    undoBtn->setIcon(undoIcon);
    undoBtn->setIconSize(QSize(51,25));
    undoBtn->setEnabled(false);
    SmartPushButton* cropBtn = new SmartPushButton(this, "Crop");
    cropBtn->setIcon(cropIcon);
    cropBtn->setIconSize(QSize(49,25));
    SmartPushButton* MotionInfoBtn = new SmartPushButton(this, "Edit Motion Information");
    MotionInfoBtn->setIcon(editIcon);
    MotionInfoBtn->setIconSize(QSize(62,25));
    QVBoxLayout* recordPlaybackLayout = editOptions->getLayout();
    recordPlaybackLayout->setContentsMargins(20,20,20,20);
    QVBoxLayout* buttons = new QVBoxLayout();
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(undoBtn);
    buttons->addWidget(cropBtn);
    buttons->addWidget(MotionInfoBtn);
    recordPlaybackLayout->addLayout(buttons, 1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));

    connect(MotionInfoBtn, SIGNAL(released()), this, SLOT(launchMotionInfo()));
    // add connections to crop and split and undo
    connect(editingControls, SIGNAL(editingPlayStateChanged(bool)), editMotionViewer, SLOT(playToggled(bool)));
    connect(editMotionViewer->getPlayPauseBtn(), SIGNAL(released()), editingControls, SLOT (togglePlay()));
    connect(editMotionViewer->getSlider(), SIGNAL(alt_valueChanged(int)), editingControls, SLOT(beginningSliderChanged(int)));
    connect(editMotionViewer->getSlider(), SIGNAL(valueChanged(int)), editingControls, SLOT(endSliderChanged(int)));
    connect(editMotionViewer->getSlider(), SIGNAL(timebarChanged(int)), editingControls, SLOT(currentFrameChanged(int)));
    connect(editingControls, SIGNAL(changeSliderVal(int)), editMotionViewer->getSlider(), SLOT(catchCurrentFrameChange(int)));
    connect(editingControls, SIGNAL(frameChanged(qint32)), editMotionViewer, SLOT(updateFirstLabel(qint32)));
    connect(editingControls, SIGNAL(totalTimeChanged(qint32)), editMotionViewer, SLOT(updateLastLabel(qint32)));


    return editOptions;
}

void TabContent::createMotionInfoWindow() {
    motionInfoWidget = new OverlayWidget(this, "Edit Motion Information");
    QVBoxLayout* layout = motionInfoWidget->getLayout();

    int inputMaxWidth = 780;
    int labelMaxWidth = 110;

    // Filename: textbox
    QHBoxLayout* f = new QHBoxLayout();
    f->setAlignment(Qt::AlignLeft);
    QLabel* l1 = new QLabel("Name: ", this);
    l1->setMinimumWidth(labelMaxWidth);
    l1->setMaximumWidth(labelMaxWidth);
    l1->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    f->addWidget(l1, -1);
    infoMotionNameTextEdit = new QLineEdit(this);
    infoMotionNameTextEdit->setStyleSheet(textInputStyleWhite);
    infoMotionNameTextEdit->setText(motion->getName().mid(0, motion->getName().length()));
    infoMotionNameTextEdit->selectAll();
    infoMotionNameTextEdit->setMaximumWidth(inputMaxWidth);
    f->addWidget(infoMotionNameTextEdit);

    // description
    QHBoxLayout* d = new QHBoxLayout();
    d->setAlignment(Qt::AlignLeft);
    QLabel* l2 = new QLabel("Description: ", this);
    l2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l2->setMinimumWidth(labelMaxWidth);
    l2->setMaximumWidth(labelMaxWidth);
    d->addWidget(l2, -1);
    infoMotionDescription = new QTextEdit(this);
    infoMotionDescription->setStyleSheet(textInputStyleWhite);
    infoMotionDescription->setMinimumHeight(150);
    infoMotionDescription->setText(motion->getDescription());
    infoMotionDescription->setMaximumWidth(inputMaxWidth);
    d->addWidget(infoMotionDescription);

    // tags input
    QHBoxLayout* t = new QHBoxLayout();
    t->setAlignment(Qt::AlignLeft);
    QLabel* l3 = new QLabel("Keywords: ", this);
    l3->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l3->setMinimumWidth(labelMaxWidth);
    l3->setMaximumWidth(labelMaxWidth);
    t->addWidget(l3, -1);
    infoMotionTagsTextEdit = new QLineEdit(this);
    infoMotionTagsTextEdit->setStyleSheet(textInputStyleWhite);
    infoMotionTagsTextEdit->setMaximumWidth(inputMaxWidth);
    t->addWidget(infoMotionTagsTextEdit);
    addTagBtn = new SmartPushButton(this, "Add Keyword");
    addTagBtn->setEnabled(false);
    // tags list
    QHBoxLayout* t2 = new QHBoxLayout();
    QLabel* spacer = new QLabel(this);
    spacer->setMinimumWidth(labelMaxWidth);
    spacer->setMaximumWidth(labelMaxWidth);
    t2->addWidget(spacer, -1);
    t2->addWidget(addTagBtn);
    infoMotionTagsLayout = new QHBoxLayout();
    // populate the motion tags
    QVector<ClosableLabel*> c = motion->getTagLabels();
    for (int i=0; i < c.size(); i++)
        infoMotionTagsLayout->addWidget(c[i]);
    t2->addLayout(infoMotionTagsLayout, 2);
    t2->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum));
    QLabel* spacer2 = new QLabel(this);
    spacer2->setMinimumWidth(65);
    spacer2->setMaximumWidth(65);
    t2->addWidget(spacer2);

    // save to options
    QHBoxLayout* s = new QHBoxLayout();
    s->setAlignment(Qt::AlignLeft);
    QLabel* l5 = new QLabel("Save to: ", this);
    l5->setMinimumWidth(labelMaxWidth);
    l5->setMaximumWidth(labelMaxWidth);
    l5->setAlignment(Qt::AlignRight);
    s->addWidget(l5, -1);
    QVBoxLayout* s1 = new QVBoxLayout();
    SmartRadioButton* infoMotionLibRadio = new SmartRadioButton(this, "Motion Library");
    QHBoxLayout* r = new QHBoxLayout();
    r->setContentsMargins(0,0,0,0);
    infoMotionCompRadio = new SmartRadioButton(this, "Local Computer");
    infoMotionBrowseBtn = new SmartPushButton(this, "Select Save Location");
    infoMotionSaveLocation = new QLabel("", this);
    if (motion->getSaveLocation() == LOCALLY) {
        qDebug() << "tabContent-edit locally";
        infoMotionLibRadio->setChecked(false);
        infoMotionCompRadio->setChecked(true);
        infoMotionSaveLocation->setText(motion->getPathWithoutFile());
    } else {
        qDebug() << "tabContent-edit library";
        infoMotionLibRadio->setChecked(true);
        infoMotionCompRadio->setChecked(false);
        infoMotionBrowseBtn->setEnabled(false);
    }
    r->addWidget(infoMotionBrowseBtn);
    r->addWidget(infoMotionSaveLocation);
    s1->addWidget(infoMotionLibRadio);
    s1->addWidget(infoMotionCompRadio);
    s1->addLayout(r);
    s->addLayout(s1);

    QHBoxLayout* btns = new QHBoxLayout();
    saveMotionInfoBtn = new SmartPushButton(this, "Save");
    SmartPushButton* cancel = new SmartPushButton(this, "Cancel");
    btns->addWidget(cancel);
    btns->addWidget(saveMotionInfoBtn);
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

    motionInfoWidget->show();

    // only connect handleUserOptions when the user selection window is visible to user
    connect(saveMotionInfoBtn, SIGNAL(released()), this, SLOT(saveMotionInfo()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeMotionInfo()));
    connect(addTagBtn, SIGNAL(released()), this, SLOT(addTag()));
    connect(infoMotionTagsTextEdit, SIGNAL(returnPressed()), this, SLOT(addTag()));
    connect(infoMotionNameTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleInfoMotionRequiredInput(QString)));
    connect(infoMotionDescription, SIGNAL(textChanged()), this, SLOT(handleInfoMotionRequiredInput()));
    connect(infoMotionTagsTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleInfoMotionRequiredInput(QString)));
    connect(infoMotionBrowseBtn, SIGNAL(released()), this, SLOT(handleInfoMotionRadios()));
    connect(infoMotionCompRadio, SIGNAL(toggled(bool)), infoMotionBrowseBtn, SLOT(setEnabled(bool)));
    connect(infoMotionCompRadio, SIGNAL(released()), this, SLOT(handleInfoMotionRequiredInput()));
    connect(infoMotionLibRadio, SIGNAL(released()), this, SLOT(handleInfoMotionRequiredInput()));
    connect(infoMotionCompRadio, SIGNAL(toggled(bool)), infoMotionSaveLocation, SLOT(setVisible(bool)));
}

void TabContent::launchMotionInfo() {
    overlay->show();
    createMotionInfoWindow();
    infoMotionNameTextEdit->setFocus();
}

void TabContent::saveMotionInfo() {
    SAVE_LOCATION s;
    QString filename = infoMotionNameTextEdit->text();
    if (infoMotionCompRadio->isChecked()) {
        s = LOCALLY;
        filename = infoMotionSaveLocation->text() + "/" + filename;
    } else {
        s = LIBRARY;
    }

    motion->updateWAGFile(filename, infoMotionDescription->toPlainText(),
                             motion->getAuthor(), infoMotionTagsLayout, s);
    // new motion info
    updateMotion();
    saveMotion();
    closeMotionInfo();
}

void TabContent::closeMotionInfo() {
    overlay->hide();
    delete motionInfoWidget;
}

// Needed for QLineEdit's textChanged(QString) SIGNAL
void TabContent::handleInfoMotionRequiredInput(QString) {
    handleInfoMotionRequiredInput();
}

// Used by QTextEdit's textChanged() SIGNAL
void TabContent::handleInfoMotionRequiredInput() {
    // sets the border color of filename textbox depending on if it has text or not
    (infoMotionNameTextEdit->text().isEmpty() ?
                infoMotionNameTextEdit->setStyleSheet(textInputStyleRed) :
                infoMotionNameTextEdit->setStyleSheet(textInputStyleWhite));
    // sets the border color of description textbox depending on if it has text or not
    (infoMotionDescription->toPlainText().isEmpty() ?
                infoMotionDescription->setStyleSheet(textInputStyleRed) :
                infoMotionDescription->setStyleSheet(textInputStyleWhite));
    // makes the add tags button only enabled when there is text in the tags box
    addTagBtn->setEnabled(!infoMotionTagsTextEdit->text().isEmpty());
    if (infoMotionCompRadio->isChecked() && infoMotionSaveLocation->text().isEmpty()) {
        infoMotionBrowseBtn->setRed(true);
    } else {
        infoMotionBrowseBtn->setRed(false);
    }

    // enables the create button if the description AND filename text boxes are filled
    saveMotionInfoBtn->setEnabled(!infoMotionDescription->toPlainText().isEmpty() &&
                                  !infoMotionNameTextEdit->text().isEmpty() &&
                                  ((infoMotionCompRadio->isChecked() && !infoMotionSaveLocation->text().isEmpty()) ||
                                  !infoMotionCompRadio->isChecked()));
}

void TabContent::addTag() {
    if (infoMotionTagsTextEdit->text().isEmpty())
        return;

    infoMotionTagsLayout->addWidget(new ClosableLabel(this, infoMotionTagsTextEdit->text()));
    infoMotionTagsTextEdit->clear();
    addTagBtn->setEnabled(false);
}

// handles the behavior of the radio buttons on the new motion window.
void TabContent::handleInfoMotionRadios() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Save Location", "",
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    if (!dir.trimmed().isEmpty()) { // user clicked "open"
        infoMotionSaveLocation->setText(dir.trimmed()); // do something with this
    }
    handleInfoMotionRequiredInput();
}
