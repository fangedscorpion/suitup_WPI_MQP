#pragma once

#include <QFrame>
#include <QWidget>
#include <QLabel>

class ClosableLabel : public QLabel
{
    Q_OBJECT
public:
    ClosableLabel(QString text);
    QString getText() { return label;}

private:
    QString label;

};
