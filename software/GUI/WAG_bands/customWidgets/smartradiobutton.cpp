#include "smartradiobutton.h"

SmartRadioButton::SmartRadioButton(QWidget* parent, const char *name) : QRadioButton(name, parent) {
    setTheme();
}

SmartRadioButton::SmartRadioButton(QWidget* parent, const char *name, ACTION_TYPE a) : QRadioButton(name, parent) {
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
