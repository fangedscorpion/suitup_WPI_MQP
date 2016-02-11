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

    return editOptions;
}

void TabContent::createMotionInfoWindow() {

    motionInfoWidget = new OverlayWidget(this, "Edit Motion Information");
    QVBoxLayout *layout = motionInfoWidget->getLayout();

    // Filename: textbox
    QHBoxLayout *f = new QHBoxLayout;
    QLabel *l1 = new QLabel("Name: ");
    l1->setMinimumWidth(100);
    l1->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    f->addWidget(l1, -1);
    infoMotionNameTextEdit = new QLineEdit;
    infoMotionNameTextEdit->setStyleSheet(textInputStyleWhite);
    f->addWidget(infoMotionNameTextEdit);

    // description
    QHBoxLayout *d = new QHBoxLayout;
    QLabel *l2 = new QLabel("Description: ");
    l2->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l2->setMinimumWidth(100);
    d->addWidget(l2, -1);
    infoMotionDescription = new QTextEdit;
    infoMotionDescription->setStyleSheet(textInputStyleWhite);
    infoMotionDescription->setMinimumHeight(150);
    d->addWidget(infoMotionDescription);

    // tags input
    QHBoxLayout *t = new QHBoxLayout;
    QLabel *l3 = new QLabel("Keywords: ");
    l3->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
    l3->setMinimumWidth(100);
    t->addWidget(l3, -1);
    infoMotionTagsTextEdit = new QLineEdit;
    infoMotionTagsTextEdit->setStyleSheet(textInputStyleWhite);
    t->addWidget(infoMotionTagsTextEdit);
    addTagBtn = new QPushButton("Add Keyword");
    addTagBtn->setEnabled(false);
    addTagBtn->setMinimumHeight(buttonHeight);
    // tags list
    QHBoxLayout *t2 = new QHBoxLayout;
    QLabel *l4 = new QLabel;
    l4->setMinimumWidth(100);
    t2->addWidget(l4, -1);
    t2->addWidget(addTagBtn, -1);
    infoMotionTagsLabel = new QLabel();
    t2->addWidget(infoMotionTagsLabel, 1);

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
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    layout->addLayout(btns);

    // only connect handleUserOptions when the user selection window is visible to user
    connect(saveMotionInfoBtn, SIGNAL(released()), this, SLOT(saveMotionInfo()));
    connect(cancel, SIGNAL(released()), this, SLOT(closeMotionInfo()));
    connect(addTagBtn, SIGNAL(released()), this, SLOT(addTag()));
    connect(infoMotionTagsTextEdit, SIGNAL(returnPressed()), this, SLOT(addTag()));
    connect(infoMotionNameTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleNewMotionRequiredInput(QString)));
    connect(infoMotionDescription, SIGNAL(textChanged()), this, SLOT(handleNewMotionRequiredInput()));
    connect(infoMotionTagsTextEdit, SIGNAL(textChanged(QString)), this, SLOT(handleNewMotionRequiredInput(QString)));
    connect(this, SIGNAL(resizedWindow()), motionInfoWidget, SLOT(resizeWindow()));
    emit this->resizedWindow();
}

void TabContent::launchMotionInfo() {
    overlay->show();
    motionInfoWidget->show();
    motionInfoWidget->raise();

    infoMotionNameTextEdit->setText(motion->getName().mid(0, motion->getName().length()-5));
    infoMotionNameTextEdit->selectAll();
    infoMotionNameTextEdit->setFocus();
    infoMotionDescription->setText(motion->getDescription());
//    infoMotionTagsLabel->setText(motion->getTagsString());

}

void TabContent::saveMotionInfo() {
    WAGFile* w = new WAGFile(infoMotionNameTextEdit->text(), infoMotionDescription->toPlainText(),
                             motion->getAuthor(), infoMotionTagsLabel->text().split("; ").toVector());
    updateMotion(w);
    closeMotionInfo();
}

void TabContent::closeMotionInfo() {
    overlay->hide();
    motionInfoWidget->hide();
}

// Needed for QLineEdit's textChanged(QString) SIGNAL
void TabContent::handleNewMotionRequiredInput(QString) {
    handleNewMotionRequiredInput();
}

// Used by QTextEdit's textChanged() SIGNAL
void TabContent::handleNewMotionRequiredInput() {
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
    // enables the create button if the description AND filename text boxes are filled
    saveMotionInfoBtn->setEnabled(!infoMotionDescription->toPlainText().isEmpty() &&
                                 !infoMotionNameTextEdit->text().isEmpty());
}
