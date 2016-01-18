#include "smartpushbutton.h"

smartPushButton::smartPushButton(const char *name, USER owner) : QPushButton(name) {
    user = owner;
    connect(this, SIGNAL(released()), this, SLOT(releaseUser()));
    this->setMinimumHeight(35);
}

smartPushButton::smartPushButton(const char* name) : QPushButton(name) {
    this->setMinimumHeight(35);
}

void smartPushButton::setUser(USER u) {
    user = u;
    connect(this, SIGNAL(released()), this, SLOT(releaseUser()));
}

void smartPushButton::releaseUser() {
    emit released(user);
}
