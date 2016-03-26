#include "overlaywidget.h"
#include <QFont>

OverlayWidget::OverlayWidget(QWidget* parent, std::string title) : Overlay(parent){

    // title and layout
    QFont titleFont = QFont( "Arial", 15, QFont::Bold);
    QVBoxLayout* layout = new QVBoxLayout();
    QLabel* lbl = new QLabel(title.c_str(), this);
    lbl->setFont(titleFont);
    lbl->setAlignment(Qt::AlignCenter);
    layout->addWidget(lbl);
    layout->addSpacerItem(new QSpacerItem(500, 1, QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding));
    this->setLayout(layout);

    // drop shadow
    CustomShadowEffect* bodyShadow = new CustomShadowEffect(this);
    this->setAutoFillBackground(true);
//    this->setGraphicsEffect(bodyShadow);
    // sorry samee :(

    // set background
    QPalette Pal(palette());
    Pal.setColor(QPalette::Background, QColor(235,235,235));
    this->setAutoFillBackground(true);
    this->setPalette(Pal);
    this->resize(parentWidget()->width()-100, parentWidget()->height()-60);

    this->hide();
    this->resize(parentWidget()->width()-100, this->height());
    Overlay::resizeWindow();
}

OverlayWidget::~OverlayWidget(){}

void OverlayWidget::resizeWindow() {
    this->resize(parentWidget()->width()-100, this->height());
    Overlay::resizeWindow();
}

QVBoxLayout* OverlayWidget::getLayout() {
    return static_cast<QVBoxLayout*>(this->layout());
}
