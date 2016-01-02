#include "smartradiobutton.h"

smartRadioButton::smartRadioButton(const char *name) : QRadioButton(name) {}

smartRadioButton::smartRadioButton(const char *name, ACTION_TYPE a) : QRadioButton(name)
{
    action = a;
    connect(this, SIGNAL(released()), this, SLOT(releaseAction()));
}

void smartRadioButton::setAction(ACTION_TYPE a) {
    action = a;
    connect(this, SIGNAL(released()), this, SLOT(releaseAction()));
}

void smartRadioButton::releaseAction() {
    emit released(action);
}
