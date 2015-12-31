#pragma once

#include <QPushButton>
#include "mainwindow.h"

/*
 * This button has an associated USER to make the button more dynamic
 */
class smartPushButton : public QPushButton
{
    Q_OBJECT
public:
    smartPushButton(const char* name, USER owner);

private:
    USER user;

signals:
    void released(USER);

private slots:
    void catchReleased();
};
