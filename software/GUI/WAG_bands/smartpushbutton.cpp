#include "smartpushbutton.h"

smartPushButton::smartPushButton(const char *name, USER owner) : QPushButton(name) {
    user = owner;
    connect(this, SIGNAL(released()), this, SLOT(catchReleased()));
}

smartPushButton::smartPushButton(const char* name) : QPushButton(name) {}

void smartPushButton::setUser(USER u) {
    user = u;
    connect(this, SIGNAL(released()), this, SLOT(catchReleased()));
}

void smartPushButton::catchReleased() {
    emit released(user);
}
