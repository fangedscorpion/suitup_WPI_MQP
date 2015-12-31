#include "overlay.h"

Overlay::Overlay(QWidget* parent) : QWidget(parent) {
}

void Overlay::resizeWindow() {
    // centered
    QPoint global = parentWidget()->rect().center();
    this->move(global.x() - this->width() / 2, global.y() - this->height() / 2);
}

Overlay::~Overlay(){}

void Overlay::paintEvent(QPaintEvent *)
{
 QStyleOption opt;
 opt.init(this);
 QPainter p(this);
 style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void Overlay::makeSemiTransparent() {
    this->setStyleSheet("background-color: rgba(255, 255, 255, 190);");
}
