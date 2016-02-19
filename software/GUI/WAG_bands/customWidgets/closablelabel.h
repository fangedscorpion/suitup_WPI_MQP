#pragma once

#include <QFrame>
#include <QWidget>

class ClosableLabel : public QFrame
{
    Q_OBJECT
public:
    ClosableLabel(QString text);
    QString getText() { return label;}

private:
    QString label;
};
