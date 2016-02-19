#include "closablelabel.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

ClosableLabel::ClosableLabel(QString text){
    QPushButton *close = new QPushButton("x");
    close->setMaximumWidth(20);
    label = text;
    QLabel *lbl = new QLabel(text);
    QFontMetrics fm(this->font());
    lbl->setMaximumWidth(fm.width(text));
    lbl->setMinimumWidth(fm.width(text));

    QHBoxLayout *h = new QHBoxLayout();
    h->addWidget(lbl, -1, Qt::AlignLeft);
    h->addSpacing(10);
    h->addWidget(close, -1, Qt::AlignRight);
    this->setLayout(h);
    this->setMaximumWidth(close->width()+lbl->width()+30);
    this->setMaximumHeight(33); // buttonHeight -2
    this->setFrameStyle(QFrame::Box);
//    this->setContentsMargins(1,1,1,1);
}

// Can be used in a resize event to add '...' after text
//QString text("some long text without elipsis");
//QFontMetrics metrics(label->font());
//QString elidedText = metrics.elidedText(text, Qt::ElideRight, label->width());
//label->setText(elidedText);
