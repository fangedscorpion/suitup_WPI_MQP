#include "mainwindow.h"
#include "customWidgets/smartpushbutton.h"
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

    overlay->show();
    userOptionsWidget->show();
    emit this->resizedWindow();
}

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
        createOpenMotionOptions(u);
        connect(openBtn, SIGNAL(released(USER)), this, SLOT(launchOpen(USER)));
    }
    overlay->hide();
    delete userOptionsWidget;
    userOptionsWidget = NULL;
    tabs->removeTab(0);
    tabs->setTabsClosable(true);
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
    QString f = QFileDialog::getOpenFileName(this, "Open File", "", "WAG Files (*.wagz)");
    // open f and get metadata
    if (!f.trimmed().isEmpty()) { // user clicked "open"
        if (userOptionsWidget != NULL)
            handleUserOptions(u); // setup the top bar
        WAGFile* w = new WAGFile(f.trimmed());
        addTab(u, w, EDIT);
        closeOpenMotionOptions();
    }    
}

// opens the user determined file
void MainWindow::openFromLibrary(USER u) {
    // TODO: open file somehow...
    // based on file, open and get metadata
    WAGFile* w = new WAGFile(QString("filename"), QString("desc"), QString("author"), QVector<QString>());
    addTab(u, w, EDIT);
    closeOpenFromLibrary();
    closeOpenMotionOptions();
}

// closes the open from library overlay
void MainWindow::closeOpenFromLibrary() {
//    if (userOptionsWidget == NULL)
//        overlay->hide();
    delete openFromLibWidget;
}

// launches the open from library overlay
void MainWindow::launchOpenFromLibrary(USER u) {
    createOpenFromLib(u);
    overlay->show();
    openFromLibWidget->show();
}

void MainWindow::launchOpen(USER u) {
    createOpenMotionOptions(u);
    overlay->show();
    openWidget->show();
}

void MainWindow::closeOpenMotionOptions() {
    overlay->hide();
    if (userOptionsWidget == NULL) {
        delete openWidget;
        openWidget = NULL;
    }
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
    if (userOptionsWidget == NULL)
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

static int disconnectedBands = 7;
void MainWindow::updateConnectionStatus(BandType b, ConnectionStatus c) {
    if (c == CONNECTED) {
        disconnectedBands--;
    } else {
        disconnectedBands++;
    }

    if ((disconnectedBands > 7) || (disconnectedBands < 0)) {
        // throw error
    }

    if (disconnectedBands > 0) {
        settingsBtn->setStyleSheet("QPushButton { color : red; border-style: outset; border-width: 2px; border-color: red; }");
        connectionStatus->setText(QString::number(disconnectedBands) + " Bands Disconnected");
        connectionStatus->setStyleSheet("QLabel { color : red; }");
    } else {
        settingsBtn->setStyleSheet("QPushButton { }");
        connectionStatus->setText("All Bands Connected");
        connectionStatus->setStyleSheet("QLabel { color : green; }");
    }

}

void MainWindow::updateBatteryStatus() {
    settingsBtn->setStyleSheet("QPushButton { color : red; border-style: outset; border-width: 2px; border-color: red; }");
    batteryStatus->setStyleSheet("QLabel { color : red; }");
}
