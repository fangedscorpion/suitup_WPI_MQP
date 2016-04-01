#pragma once
#include <QRadioButton>
#include "user.h"

/*
 * This button has an associated USER to make the button more dynamic
 */
class SmartRadioButton : public QRadioButton
{
    Q_OBJECT
public:
    SmartRadioButton(QWidget* parent, const char* name, ACTION_TYPE a);
    SmartRadioButton(QWidget* parent, const char* name);
    ~SmartRadioButton(){}
    void setAction(ACTION_TYPE a);

private:
    ACTION_TYPE action;
    void setTheme();

signals:
    void released(ACTION_TYPE);

private slots:
    void releaseAction();
};
