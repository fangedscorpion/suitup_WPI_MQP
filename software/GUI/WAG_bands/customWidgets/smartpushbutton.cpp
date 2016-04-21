#include "smartpushbutton.h"

SmartPushButton::SmartPushButton(QWidget *parent, const char *name, USER owner) : QPushButton(name, parent) {
    setTheme();

    user = owner;
    connect(this, SIGNAL(released()), this, SLOT(releaseUser()));
}

SmartPushButton::SmartPushButton(QWidget *parent, const char* name) : QPushButton(name, parent) {
    setTheme();
}

void SmartPushButton::setTheme() {
    QString base = "QPushButton{ border: 1px solid gray; border-radius: 4px; padding-left: 7px; padding-right: 7px;} ";
    blueTheme = base + "QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(214, 234, 248), stop: 1 rgb(133, 193, 233));} QPushButton:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(174, 214, 241), stop: 1 rgb(93, 173, 226));} QPushButton:disabled{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(235, 245, 251), stop: 1 rgb(174, 214, 241));}";
    redTheme = base + "QPushButton { background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(250, 219, 216), stop: 1 rgb(236, 112, 99));} QPushButton:hover {background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(241, 148, 138), stop: 1 rgb(231, 76, 60));} QPushButton:disabled{background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 rgb(253, 237, 236), stop: 1 rgb(245, 183, 177));}";

    this->setStyleSheet(blueTheme);
    this->setFocusPolicy(Qt::NoFocus);
    this->setMinimumHeight(35);
    this->setAutoDefault(true);
}

void SmartPushButton::setUser(USER u) {
    user = u;
    connect(this, SIGNAL(released()), this, SLOT(releaseUser()));
}

void SmartPushButton::releaseUser() {
    emit released(user);
}

void SmartPushButton::setRed(bool t) {
    if (t) {
        this->setStyleSheet(redTheme);

    } else {
        this->setStyleSheet(blueTheme);

    }
}

void SmartPushButton::makeGreen(void) {
    this->setRed(false);
}
