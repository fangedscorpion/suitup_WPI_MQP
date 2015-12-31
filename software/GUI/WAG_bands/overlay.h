#pragma once
#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QPoint>
#include <QStyleOption>


class Overlay : public QWidget {
    Q_OBJECT
public:
    Overlay(QWidget *parent);
    ~Overlay();
    void paintEvent(QPaintEvent *);
    void makeSemiTransparent();

private:


public slots:
    void resizeWindow();
};
