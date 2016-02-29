#include "tabcontent.h"


// Edit Recording options and controller
// TODO: add edit file name and description in here
QWidget* TabContent::createEditOptionsAndControls() {
    editingControls = new EditingController();
    editingControls->setActiveMotion(motion);
    connect(editingControls, SIGNAL(changeSliderMax(qint32)), editMotionViewer, SLOT(changeSliderRange(qint32)));
    // Edit recording options
    QGroupBox *editOptions = new QGroupBox("Editing Options");
    editOptions->setStyleSheet(groupboxStyleSheet);
    editOptions->setFont(titleFont);
    editOptions->setAlignment(Qt::AlignRight);
    QPushButton *undoBtn = new QPushButton;
    undoBtn->setIcon(undoIcon);
    undoBtn->setIconSize(QSize(51,25));
    undoBtn->setMinimumHeight(buttonHeight);
    undoBtn->setText("Undo");
    undoBtn->setEnabled(false);
    QPushButton *cropBtn = new QPushButton;
    cropBtn->setIcon(cropIcon);
    cropBtn->setIconSize(QSize(49,25));
    cropBtn->setMinimumHeight(buttonHeight);
    cropBtn->setText("Crop");
    QPushButton *splitBtn = new QPushButton;
    splitBtn->setIcon(splitIcon);
    splitBtn->setIconSize(QSize(62,25));
    splitBtn->setMinimumHeight(buttonHeight);
    splitBtn->setText("Split");
    QPushButton *MotionInfoBtn = new QPushButton;
    MotionInfoBtn->setIcon(editIcon);
    MotionInfoBtn->setIconSize(QSize(62,25));
    MotionInfoBtn->setMinimumHeight(buttonHeight);
    MotionInfoBtn->setText("Edit Motion Information");
    QPushButton *saveBtn = new QPushButton;
//    saveBtn->setIcon(cropIcon);
//    saveBtn->setIconSize(QSize(49,25));
    saveBtn->setMinimumHeight(buttonHeight);
    saveBtn->setText("Save");
    QVBoxLayout *recordPlaybackLayout = new QVBoxLayout;
    QVBoxLayout *buttons = new QVBoxLayout;
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    buttons->addWidget(undoBtn);
    buttons->addWidget(cropBtn);
    buttons->addWidget(splitBtn);
    buttons->addWidget(MotionInfoBtn);
    buttons->addWidget(saveBtn);
    recordPlaybackLayout->addLayout(buttons, 1);
    recordPlaybackLayout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    editOptions->setLayout(recordPlaybackLayout);

    connect(MotionInfoBtn, SIGNAL(released()), this, SLOT(launchMotionInfo()));
    connect(saveBtn, SIGNAL(released()), this, SLOT(saveMotion()));
    // add connections to crop and split and undo
    connect(editingControls, SIGNAL(editingPlayStateChanged(bool)), editMotionViewer, SLOT(playToggled(bool)));
    connect(editMotionViewer->getPlayPauseBtn(), SIGNAL(released()), editingControls, SLOT (togglePlay()));
    connect(editMotionViewer->getSlider(), SIGNAL(alt_valueChanged(int)), editingControls, SLOT(beginningSliderChanged(int)));
    connect(editMotionViewer->getSlider(), SIGNAL(valueChanged(int)), editingControls, SLOT(endSliderChanged(int)));
    connect(editMotionViewer->getSlider(), SIGNAL(timebarChanged(int)), editingControls, SLOT(currentFrameChanged(int)));
    connect(editingControls, SIGNAL(changeSliderVal(int)), editMotionViewer->getSlider(), SLOT(catchCurrentFrameChange(int)));

    return editOptions;
}

void TabContent::createMotionInfoWindow() {
    motionInfoWidget = new OverlayWidget(this, "Edit Motion Information");
    QVBoxLayout *layout = motionInfoWidget->getLayout();

    int inputMaxWidth = 780;
    int labelMaxWidth = 110;

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    f->setAlignment(Qt::AlignLeft);
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(labelMaxWidth);
    l1->setMaximumWidth(labelMaxWidth);
    l1->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    f->addWidget(l1, -1);
    infoMotionNameTextEdit = new QLineEdit;
    infoMotionNameTextEdit->setStyleSheet(textInputStyleWhite);
    infoMotionNameTextEdit->setText(motion->getName().mid(0, motion->getName().length()-5));
    infoMotionNameTextEdit->selectAll();
    infoMotionNameTextEdit->setFocus();
    infoMotionNameTextEdit->setMaximumWidth(inputMaxWidth);
    f->addWidget(infoMotionNameTextEdit);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    d->setAlignment(Qt::AlignLeft);
    QLabel *l2 = new QLabel("Description: ");
    l2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l2->setMinimumWidth(labelMaxWidth);
    l2->setMaximumWidth(labelMaxWidth);
    d->addWidget(l2, -1);
    infoMotionDescription = new QTextEdit;
    infoMotionDescription->setStyleSheet(textInputStyleWhite);
    infoMotionDescription->setMinimumHeight(150);
    infoMotionDescription->setText(motion->getDescription());
    infoMotionDescription->setMaximumWidth(inputMaxWidth);
    d->addWidget(infoMotionDescription);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    t->setAlignment(Qt::AlignLeft);
    QLabel *l3 = new QLabel("Keywords: ");
    l3->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l3->setMinimumWidth(labelMaxWidth);
    l3->setMaximumWidth(labelMaxWidth);
    t->addWidget(l3, -1);
    infoMotionTagsTextEdit = new QLineEdit;
    infoMotionTagsTextEdit->setStyleSheet(textInputStyleWhite);
    infoMotionTagsTextEdit->setMaximumWidth(inputMaxWidth);
    t->addWidget(infoMotionTagsTextEdit);
    addTagBtn = new QPushButton("Add Keyword");
    addTagBtn->setEnabled(false);
    addTagBtn->setMinimumHeight(buttonHeight);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *spacer = new QLabel;
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
    QLabel *spacer2 = new QLabel;
    spacer2->setMinimumWidth(65);
    spacer2->setMaximumWidth(65);
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
    QRadioButton *infoMotionLibRadio = new QRadioButton("Motion Library");
    QHBoxLayout *r = new QHBoxLayout;
    r->setContentsMargins(0,0,0,0);
    infoMotionCompRadio = new QRadioButton("Local Computer");
    infoMotionBrowseBtn = new QPushButton("Select Save Location");
    infoMotionSaveLocation = new QLabel("");
    if (motion->getSaveLocation() == LOCALLY) {
        infoMotionLibRadio->setChecked(false);
        infoMotionCompRadio->setChecked(true);
        infoMotionSaveLocation->setText(motion->getPathWithoutFile());
    } else {
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

    QHBoxLayout *btns = new QHBoxLayout;
    saveMotionInfoBtn = new smartPushButton("Save");
    QPushButton *cancel = new QPushButton("Cancel");
    cancel->setMinimumHeight(buttonHeight);
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
    connect(this, SIGNAL(resizedWindow()), motionInfoWidget, SLOT(resizeWindow()));
    connect(infoMotionBrowseBtn, SIGNAL(released()), this, SLOT(handleinfoMotionRadios()));
    connect(infoMotionCompRadio, SIGNAL(toggled(bool)), infoMotionBrowseBtn, SLOT(setEnabled(bool)));
    connect(infoMotionCompRadio, SIGNAL(released()), this, SLOT(handleInfoMotionRequiredInput()));
    connect(infoMotionLibRadio, SIGNAL(released()), this, SLOT(handleInfoMotionRequiredInput()));
    connect(infoMotionCompRadio, SIGNAL(toggled(bool)), infoMotionSaveLocation, SLOT(setVisible(bool)));
    emit this->resizedWindow();
}

void TabContent::launchMotionInfo() {
    overlay->show();
    createMotionInfoWindow();
}

void TabContent::saveMotionInfo() {
    // UPDATE THIS LATER. vvv LOCALLY needs to be based on user input
    WAGFile* w = new WAGFile(infoMotionNameTextEdit->text(), infoMotionDescription->toPlainText(),
                             motion->getAuthor(), infoMotionTagsLayout, LOCALLY);
    // new motion info
    delete motion;
    motion = w;
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
        infoMotionBrowseBtn->setStyleSheet("QPushButton { border-radius: 2px; border-style: outset; border-width: 1px; border-color: red; padding-left: 9px; padding-right: 9px; padding-top: 4px; padding-bottom: 4px;}");
    } else {
        infoMotionBrowseBtn->setStyleSheet("QPushButton { }");
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

    infoMotionTagsLayout->addWidget(new ClosableLabel(infoMotionTagsTextEdit->text()));
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
