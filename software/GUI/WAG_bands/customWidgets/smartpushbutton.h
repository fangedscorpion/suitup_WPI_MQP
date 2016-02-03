#pragma once

#include <QPushButton>
#include "user.h"

/*
 * This button has an associated USER to make the button more dynamic
 */
class smartPushButton : public QPushButton
{
    Q_OBJECT
public:
    smartPushButton(const char* name, USER owner);
    smartPushButton(const char* name);
    void setUser(USER u);

private:
    USER user;

signals:
    void released(USER);

private slots:
    void releaseUser();
};
