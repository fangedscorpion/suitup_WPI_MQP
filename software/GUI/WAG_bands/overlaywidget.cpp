#include "overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent) : Overlay(parent){

    // drop shadow
    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    this->setAutoFillBackground(true);
    this->setGraphicsEffect(bodyShadow);

    // set background
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, QColor(235,235,235));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->resize(parentWidget()->width()-100, parentWidget()->height()-60);
}


OverlayWidget::~OverlayWidget(){}

void OverlayWidget::resizeWindow() {
    this->resize(parentWidget()->width()-100, this->height());
    Overlay::resizeWindow();
}
