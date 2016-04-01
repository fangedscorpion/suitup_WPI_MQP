#pragma once

#include <QFrame>
#include <QWidget>
#include <QLabel>

class ClosableLabel : public QLabel
{
    Q_OBJECT
public:
    ClosableLabel(QWidget* parent, QString text);
    ~ClosableLabel(){}
    QString getText() { return label;}
    void closeLabel();

private:
    QString label;
};
