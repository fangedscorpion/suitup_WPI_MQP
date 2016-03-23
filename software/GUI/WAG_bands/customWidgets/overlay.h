#pragma once
#include <QWidget>
#include <QLayout>
#include <QPainter>
#include <QLabel>
#include <QPushButton>
#include <QPalette>
#include <QPoint>
#include <QPointer>
#include <QStyleOption>


class Overlay : public QWidget {
    Q_OBJECT
public:
    Overlay(QPointer<QWidget> parent);
    ~Overlay();
    void paintEvent(QPaintEvent *);
    void makeSemiTransparent();

public slots:
    void resizeWindow();
};
