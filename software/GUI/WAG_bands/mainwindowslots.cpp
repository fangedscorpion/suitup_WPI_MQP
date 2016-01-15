#include "mainwindow.h"
#include "smartpushbutton.h"
#include <sstream>

// event for when the main window is resized
void MainWindow::resizeEvent(QResizeEvent* r) {
    QMainWindow::resizeEvent(r);
    // resize the overlay to cover the whole window
    overlay->resize(this->width(), this->height());
    emit resizedWindow();
}

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
        connect(newMotion, SIGNAL(released(USER)), this, SLOT(launchNewFile(USER))); // finish this
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
    smartPushButton* back = new smartPushButton("Back", u);
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
        connect(newBtn, SIGNAL(released(USER)), this, SLOT(launchNewFile(USER)));
    }
    if (u.hasAction(EDIT) || u.hasAction(PLAYBACK)) {
        openBtn->setUser(u);
        openBtn->show();
        connect(openBtn, SIGNAL(released(USER)), this, SLOT(launchOpenFromComputer(USER)));
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
    if (newFileTagsTextEdit->text().isEmpty())
        return;
    newFileTagsLabel->setText(newFileTagsTextEdit->text() + "; " + newFileTagsLabel->text());
    newFileTagsTextEdit->clear();
    addTagBtn->setEnabled(false);
}

// opens the dialog box to find a local file
void MainWindow::launchOpenFromComputer(USER u) {
    // File browser
    QString f = QFileDialog::getOpenFileName(this, "Open File", "/");
    if (!f.trimmed().isEmpty()) { // user clicked "open"
        addTab(u, f.trimmed(), EDIT);
    }
}

// opens the user determined file
void MainWindow::openFromLibrary(USER u) {
    // TODO: open file somehow...
    addTab(u, "filename.wagz", EDIT);
    closeOpenFromLibrary();
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

// launch the new file overlay
void MainWindow::launchNewFile(USER u) {
    createNewFile(u);
    overlay->show();
    newFileWidget->show();
}

// creates the new file and opens a tab
void MainWindow::saveNewFile(USER u) {
    addTab(u, newFilenameTextEdit->text() + ".wagz", RECORD);
    closeNewFile();
}

// close the new file overlay
void MainWindow::closeNewFile() {
    if (!userOptionsWidget->isVisible())
        overlay->hide();
    delete newFileWidget;
}
void MainWindow::handleNewFileRequiredInput(QString) {
    handleNewFileRequiredInput();
}

void MainWindow::handleNewFileRequiredInput() {
    // sets the border color of filename textbox depending on if it has text or not
    (newFilenameTextEdit->text().isEmpty() ?
                newFilenameTextEdit->setStyleSheet(textInputStyleRed) :
                newFilenameTextEdit->setStyleSheet(textInputStyleWhite));
    // sets the border color of description textbox depending on if it has text or not
    (newFileDescription->toPlainText().isEmpty() ?
                newFileDescription->setStyleSheet(textInputStyleRed) :
                newFileDescription->setStyleSheet(textInputStyleWhite));
    // makes the add tags button only enabled when there is text in the tags box
    addTagBtn->setEnabled(!newFileTagsTextEdit->text().isEmpty());
    // enables the create button if the description AND filename text boxes are filled
    createNewFileBtn->setEnabled(!newFileDescription->toPlainText().isEmpty() &&
                                 !newFilenameTextEdit->text().isEmpty());
}
