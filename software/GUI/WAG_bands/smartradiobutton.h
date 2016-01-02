#pragma once
#include <QRadioButton>
#include "user.h"

/*
 * This button has an associated USER to make the button more dynamic
 */
class smartRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    smartRadioButton(const char* name, ACTION_TYPE a);
    smartRadioButton(const char* name);
    void setAction(ACTION_TYPE a);

private:
    ACTION_TYPE action;

signals:
    void released(ACTION_TYPE);

private slots:
    void releaseAction();
};
