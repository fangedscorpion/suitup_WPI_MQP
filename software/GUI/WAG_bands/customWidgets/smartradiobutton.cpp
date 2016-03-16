#include "smartradiobutton.h"

SmartRadioButton::SmartRadioButton(const char *name) : QRadioButton(name) {
    setTheme();
}

SmartRadioButton::SmartRadioButton(const char *name, ACTION_TYPE a) : QRadioButton(name) {
    setTheme();
    action = a;
    connect(this, SIGNAL(released()), this, SLOT(releaseAction()));
}

void SmartRadioButton::setTheme() {
    this->setStyleSheet("SmartRadioButton:hover{background-color: #C8E6C9;}");
    this->setFocusPolicy(Qt::NoFocus);
}

void SmartRadioButton::setAction(ACTION_TYPE a) {
    action = a;
    connect(this, SIGNAL(released()), this, SLOT(releaseAction()));
}

void SmartRadioButton::releaseAction() {
    emit released(action);
}
