#include "styledgroupbox.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QStyleOption>
#include <QPainter>

StyledGroupBox::StyledGroupBox(QWidget* parent, QString in_title) : QWidget(parent) {
    title = in_title;

    QVBoxLayout* layout = new QVBoxLayout();
    layout->setContentsMargins(20,20,20,20);

    label = new QLabel(title, this);
    label->setAlignment(Qt::AlignCenter);
    label->setFont(QFont( "Arial", 15, QFont::Bold));

    layout->addWidget(label);

    this->setObjectName("group");
    this->setStyleSheet("StyledGroupBox#group{background-color: #C8E6C9; border: 1px solid gray; border-radius: 9px; margin-left: 0.25em; margin-right: 0.25em; margin-top: 0.5em;}");
    this->setLayout(layout);
}

StyledGroupBox::~StyledGroupBox() {}

void StyledGroupBox::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void StyledGroupBox::setTitle(QString t) {
    title = t;
    label->setText(title);
}
