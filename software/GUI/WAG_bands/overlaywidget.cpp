#include "overlaywidget.h"

OverlayWidget::OverlayWidget(QWidget *parent) : Overlay(parent){

    CustomShadowEffect *bodyShadow = new CustomShadowEffect();
    this->setAutoFillBackground(true);
    this->setGraphicsEffect(bodyShadow);

    QPalette Pal(palette());
    // set background
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
