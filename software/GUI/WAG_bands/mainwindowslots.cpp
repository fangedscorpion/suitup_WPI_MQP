#include "mainwindow.h"
#include "customWidgets/smartpushbutton.h"
#include "tabcontent.h"
#include "customWidgets/closablelabel.h"
#include <sstream>
#include <QTableWidgetItem>

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

    newMotionTagsLayout->addWidget(new ClosableLabel(newMotionTagsTextEdit->text()));
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
    // based on file, open and get metadata
    QModelIndexList selection = openFromLibTable->selectionModel()->selectedRows();
    if (selection.count() != 1) {
        // throw error
        return;
    }
    QTableWidgetItem* motionInfo = openFromLibTable->item(selection.at(0).row(), 3);
    addTab(u, new WAGFile(motionInfo->text()), EDIT);
    closeOpenFromLibrary();
    closeOpenMotionOptions();
}

// closes the open from library overlay
void MainWindow::closeOpenFromLibrary() {
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
    SAVE_LOCATION s;
    QString filename = newMotionNameTextEdit->text();
    if (newMotionCompRadio->isChecked()) {
        s = LOCALLY;
        filename = newMotionSaveLocation->text() + "/" + filename;
    } else {
        s = LIBRARY;
    }
    WAGFile* w = new WAGFile(filename, newMotionDescription->toPlainText(),
                             QString("author"), newMotionTagsLayout, s);
    addTab(u, w, RECORD);
    closeNewMotion();
}

// close the new file overlay
void MainWindow::closeNewMotion() {
    if (userOptionsWidget == NULL)
        overlay->hide();
    delete newMotionWidget;
}

// makes sure that all required info is entered by the user
// this prototype is required for Qt SLOTs
void MainWindow::handleNewMotionRequiredInput(QString) {
    handleNewMotionRequiredInput();
}

// makes sure that all required info is entered by the user
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
    if (newMotionCompRadio->isChecked() && newMotionSaveLocation->text().isEmpty()) {
        newMotionBrowseBtn->setStyleSheet("QPushButton { border-radius: 2px; border-style: outset; border-width: 2px; border-color: red; padding-left: 9px; padding-right: 9px; padding-top: 4px; padding-bottom: 4px;}");
    } else {
        newMotionBrowseBtn->setStyleSheet("QPushButton { }");
    }

    // enables the create button if the description AND filename text boxes are filled
    createNewMotionBtn->setEnabled(!newMotionDescription->toPlainText().isEmpty() &&
                                   !newMotionNameTextEdit->text().isEmpty() &&
                                   ((newMotionCompRadio->isChecked() && !newMotionSaveLocation->text().isEmpty()) ||
                                   !newMotionCompRadio->isChecked()));
}

// handles the behavior of the radio buttons on the new motion window.
void MainWindow::handleNewMotionRadios() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Save Location", "",
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);
    if (!dir.trimmed().isEmpty()) { // user clicked "open"
        newMotionSaveLocation->setText(dir.trimmed()); // do something with this
    }
    handleNewMotionRequiredInput();
}

// updates the band connection status label
void MainWindow::updateConnectionStatus(BandType b, ConnectionStatus c) {
    if (c == CONNECTED)
        connectedBands.insert(b);
    else
        connectedBands.remove(b);
    handleConnectedBands();
}

void MainWindow::handleConnectedBands() {
    // count active bands
    int totalActiveBands = rightLowerArm->isChecked() + rightUpperArm->isChecked() + rightShoulder->isChecked();
    totalActiveBands += leftLowerArm->isChecked() + leftUpperArm->isChecked() + leftShoulder->isChecked() + 1;

    if ((connectedBands.size() > totalActiveBands) || (connectedBands.size() < 0)) {
        // throw error
    }

    if (connectedBands.size() < totalActiveBands) {
        QString redBtn = "QPushButton { color : red; border-style: outset; border-width: 2px; border-color: red; }";
        settingsBtn->setStyleSheet(redBtn);
        connectBands->setStyleSheet(redBtn);
        connectionStatus->setText(QString::number(totalActiveBands - connectedBands.size()) + " Bands Disconnected");
        connectionStatus->setStyleSheet("QLabel { color : red; }");
    } else {
        settingsBtn->setStyleSheet("QPushButton { }");
        connectBands->setStyleSheet("QPushButton { }");
        connectionStatus->setText("All Bands Connected");
        connectionStatus->setStyleSheet("QLabel { color : green; }");
    }
}

// updates the band battery level label
void MainWindow::updateBatteryStatus() {
    settingsBtn->setStyleSheet("QPushButton { color : red; border-style: outset; border-width: 2px; border-color: red; }");
    batteryStatus->setStyleSheet("QLabel { color : red; }");
}

void MainWindow::lockOnPlayOrRecord(bool suitRecording) {
    // lock wifi button
    // lock help button
    // prevent user from switching tabs
    settingsBtn->setEnabled(!suitRecording);
    newBtn->setEnabled(!suitRecording);
    openBtn->setEnabled(!suitRecording);
    int currentTabIndex = tabs->currentIndex();

    for (int i = 0; i < tabs->count(); i++) {
        if (i != currentTabIndex) {
            tabs->setTabEnabled(i, !suitRecording);
        }
    }

}

void MainWindow::catchLowBatterySignal(BandType lowBatteryBand) {
    static QSet<BandType> lowBatteryBands = QSet<BandType>();
    if (!(lowBatteryBands.contains(lowBatteryBand))) {
        lowBatteryBands<<lowBatteryBand;
        QString lowBatteryText = "Low Battery: ";
        QList<BandType> bandList = lowBatteryBands.toList();
        if (bandList.size() == 1) {
            lowBatteryText +=(AbsBand::bandTypeToString(bandList[0]));
        } else {
            for (int i = 0; i < bandList.size() - 1; i++) {
                lowBatteryText += AbsBand::bandTypeToString(bandList[i]);
                lowBatteryText += ", ";
            }
            lowBatteryText += "and ";
            lowBatteryText += AbsBand::bandTypeToString(bandList[bandList.size() - 1]);
        }
        batteryStatus->setText(lowBatteryText);
        updateBatteryStatus();
    }
}

// enables the "load" button on the openFromLib widget if one of the motions is selected.
void MainWindow::handleOpenFromLibBtn(int, int) {
    openFromLibBtn->setEnabled(!openFromLibTable->selectedItems().isEmpty());
}

void MainWindow::handleOpenFromLibFilter(QString) {
    QRegExp rx("(\\ |\\,)");
    QStringList filterByList = openFromLibFilterBar->text().trimmed().toLower().split(rx);
    int col = openFromLibFilterOpts->currentIndex();
    for( int i = 0; i < openFromLibTable->rowCount(); ++i ) {
        bool match = true;
        QTableWidgetItem *item = openFromLibTable->item( i, col );
        for (int j=0; j < filterByList.size(); j++) {
            if(!item->text().toLower().contains(filterByList[j]) ) {
                match = false;
                break;
            }
        }

        if (!match)
            openFromLibTable->hideRow(i);
        else
            openFromLibTable->showRow(i);
    }
}
