#include "mainwindow.h"
#include "smartpushbutton.h"
#include "tabcontent.h"
#include <sstream>

// Launch window with user's options based on which type of user was selected
void MainWindow::launchUserOptions(USER u) {
    std::stringstream ss;
    ss << u.getName() << " Options";
    userOptionsWidget = new OverlayWidget(this, ss.str());
    QVBoxLayout* layout = userOptionsWidget->getLayout();

    QLabel* lbl = new QLabel(u.getDescription());
    lbl->setAlignment(Qt::AlignCenter);
    layout->addWidget(lbl);
    if (u.hasAction(RECORD)) {
        smartPushButton* newMotion = new smartPushButton("Record a new motion", u);
        connect(newMotion, SIGNAL(released(USER)), this, SLOT(launchNewMotion(USER))); // finish this
        layout->addWidget(newMotion);
    }
    // if a user can only edit (can't playback other user's motions
    if (u.hasAction(EDIT) && !u.hasAction(PLAYBACK)) {
        smartPushButton* openFromLib = new smartPushButton("Open your motion from the library", u);
        smartPushButton* openFromComp = new smartPushButton("Open your motion from your computer", u);
        connect(openFromLib, SIGNAL(released(USER)), this, SLOT(launchOpenFromLibrary(USER))); // finish this
        connect(openFromComp, SIGNAL(released(USER)), this, SLOT(launchOpenFromComputer(USER))); // finish this
        layout->addWidget(openFromLib);
        layout->addWidget(openFromComp);
    }
    if (u.hasAction(PLAYBACK)) {
        smartPushButton* openFromLib = new smartPushButton("Open a motion from the library", u);
        smartPushButton* openFromComp = new smartPushButton("Open a motion from your computer", u);
        connect(openFromLib, SIGNAL(released(USER)), this, SLOT(launchOpenFromLibrary(USER))); // finish this
        connect(openFromComp, SIGNAL(released(USER)), this, SLOT(launchOpenFromComputer(USER))); // finish this
        layout->addWidget(openFromLib);
        layout->addWidget(openFromComp);
    }
    smartPushButton* back = new smartPushButton("Cancel", u);
    layout->addWidget(back);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::Expanding, QSizePolicy::Expanding));
    connect(back, SIGNAL(released()), this, SLOT(closeUserOptions()));

    connect(this, SIGNAL(resizedWindow()), userOptionsWidget, SLOT(resizeWindow()));
    overlay->show();
    userOptionsWidget->show();
    emit this->resizedWindow();
}

// close window with user options
void MainWindow::closeUserOptions() {
    overlay->hide();
    delete userOptionsWidget;
}

// configures the menubar buttons according to which type of user was selected
void MainWindow::handleUserOptions(USER u) {
    if (u.hasAction(RECORD)) {
        newBtn->setUser(u);
        newBtn->show();
        connect(newBtn, SIGNAL(released(USER)), this, SLOT(launchNewMotion(USER)));
    }
    if (u.hasAction(EDIT) || u.hasAction(PLAYBACK)) {
        openBtn->setUser(u);
        openBtn->show();
        createOpen(u);
        connect(openBtn, SIGNAL(released()), this, SLOT(launchOpen()));
    }
    // only set once even if u has both RECORD and EDIT
    if (u.hasAction(RECORD) || u.hasAction(EDIT)) {
        saveBtn->setUser(u);
        saveBtn->show();
        connect(saveBtn, SIGNAL(released(USER)), this, SLOT(save()));
    }
    userOptionsWidget->hide();
    overlay->hide();
    tabs->removeTab(0);
}

// save the motion file
void MainWindow::save() {
    /*
     * QFileDialog
    QFile f( filename );
    f.open( QIODevice::WriteOnly );
    // store data in f
    f.close();
*/
//    closeSaveAs();
    // TODO: Save da file!
}

// in new file window, adds a tag to the tags list
void MainWindow::addTag() {
    if (newMotionTagsTextEdit->text().isEmpty())
        return;
    newMotionTagsLabel->setText(newMotionTagsTextEdit->text() + "; " + newMotionTagsLabel->text());
    newMotionTagsTextEdit->clear();
    addTagBtn->setEnabled(false);
}

// opens the dialog box to find a local file
void MainWindow::launchOpenFromComputer(USER u) {
    // File browser
    QString f = QFileDialog::getOpenFileName(this, "Open File", "/");
    // open f and get metadata
    if (!f.trimmed().isEmpty()) { // user clicked "open"
        WAGFile* w = new WAGFile(f.trimmed(), QString("desc"), QString("author"), QVector<QString>());
        addTab(u, w, EDIT);
        closeOpen();
    }    
}

// opens the user determined file
void MainWindow::openFromLibrary(USER u) {
    // TODO: open file somehow...
    // based on file, open and get metadata
    WAGFile* w = new WAGFile(QString("filename"), QString("desc"), QString("author"), QVector<QString>());
    addTab(u, w, EDIT);
    closeOpenFromLibrary();
    closeOpen();
}

// closes the open from library overlay
void MainWindow::closeOpenFromLibrary() {
    if (!userOptionsWidget->isVisible())
        overlay->hide();
    delete openFromLibWidget;
}

// launches the open from library overlay
void MainWindow::launchOpenFromLibrary(USER u) {
    createOpenFromLib(u);
    overlay->show();
    openFromLibWidget->show();
}

// opens the settings window
void MainWindow::launchSettings(){
    overlay->show();
    settingsWidget->show();
    // setup the widget
    connectBands->setFocus();
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

// launch the new file overlay
void MainWindow::launchNewMotion(USER u) {
    createNewMotion(u);
    newMotionNameTextEdit->setFocus();
    newMotionNameTextEdit->setCursorPosition(0);
    overlay->show();
    newMotionWidget->show();
}

// creates the new file and opens a tab
void MainWindow::saveNewMotion(USER u) {
    WAGFile* w = new WAGFile(newMotionNameTextEdit->text(), newMotionDescription->toPlainText(), QString("author"), newMotionTagsLabel->text().split("; ").toVector());
    addTab(u, w, RECORD);
    closeNewMotion();
}

// close the new file overlay
void MainWindow::closeNewMotion() {
    if (!userOptionsWidget->isVisible())
        overlay->hide();
    delete newMotionWidget;
}
void MainWindow::handleNewMotionRequiredInput(QString) {
    handleNewMotionRequiredInput();
}

void MainWindow::handleNewMotionRequiredInput() {
    // sets the border color of filename textbox depending on if it has text or not
    (newMotionNameTextEdit->text().isEmpty() ?
                newMotionNameTextEdit->setStyleSheet(textInputStyleRed) :
                newMotionNameTextEdit->setStyleSheet(textInputStyleWhite));
    // sets the border color of description textbox depending on if it has text or not
    (newMotionDescription->toPlainText().isEmpty() ?
                newMotionDescription->setStyleSheet(textInputStyleRed) :
                newMotionDescription->setStyleSheet(textInputStyleWhite));
    // makes the add tags button only enabled when there is text in the tags box
    addTagBtn->setEnabled(!newMotionTagsTextEdit->text().isEmpty());
    // enables the create button if the description AND filename text boxes are filled
    createNewMotionBtn->setEnabled(!newMotionDescription->toPlainText().isEmpty() &&
                                 !newMotionNameTextEdit->text().isEmpty());
}

void MainWindow::launchOpen() {
    overlay->show();
    openWidget->show();
    openWidget->raise();
}

void MainWindow::closeOpen() {
    overlay->hide();
    openWidget->hide();
}

void MainWindow::setTabContentVoiceControl(bool b) {
    TabContent *tmp=dynamic_cast<TabContent*>(tabs->widget(0));
    if(tmp!=NULL)
        ((TabContent*)tabs->widget(0))->setVoiceControl(b);
    for(int i=1; i <tabs->count(); i++) {
        ((TabContent*)tabs->widget(i))->setVoiceControl(b);
    }
}
