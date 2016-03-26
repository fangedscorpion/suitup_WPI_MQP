#include "closablelabel.h"
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QSharedPointer>

ClosableLabel::ClosableLabel(QWidget *parent, QString text) : QLabel(parent) {
    QPushButton* close = new QPushButton("×", this);
    close->setStyleSheet("QPushButton{font-weight: bold; border:none; } QPushButton:hover{border: 1px solid grey; border-radius: 2px;}");
    close->setFocusPolicy(Qt::NoFocus);
    close->setMaximumWidth(16);
    close->setMinimumWidth(16);
    label = text;
    QLabel* lbl = new QLabel(text, this);
    QFontMetrics fm(this->font());
    lbl->setMaximumWidth(fm.width(text));
    lbl->setMinimumWidth(fm.width(text));

    this->setMargin(0);
    this->setStyleSheet("ClosableLabel{padding: 0px 0px 0px 0px; border: 1px solid gray; border-radius: 4px; background-color: #A5D6A7;}");
    this->setIndent(0);

    QHBoxLayout* h = new QHBoxLayout(this);
    h->setSpacing(0);
    h->addWidget(lbl, -1, Qt::AlignLeft);
    h->addSpacing(10);
    h->addWidget(close, -1, Qt::AlignRight);
    h->setSpacing(0);
    h->setContentsMargins(5,2,5,2);
    this->setLayout(h);
    this->setMaximumWidth(h->minimumSize().width());
    this->setMaximumHeight(33); // buttonHeight-2

    connect(close, SIGNAL(released()), this, SLOT(close()));
    this->setAttribute(Qt::WA_DeleteOnClose);
}

// Can be used in a resize event to add '...' after text
//QString text("some long text without elipsis");
//QFontMetrics metrics(label->font());
//QString elidedText = metrics.elidedText(text, Qt::ElideRight, label->width());
//label->setText(elidedText);

