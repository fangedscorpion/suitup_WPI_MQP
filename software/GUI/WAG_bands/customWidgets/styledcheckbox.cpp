#include "styledcheckbox.h"

StyledCheckBox::StyledCheckBox(QString title, QWidget *parent) :
    QCheckBox(title, parent) {
    this->setStyleSheet("StyledCheckBox:hover{background-color: #C8E6C9;}");
    this->setFocusPolicy(Qt::NoFocus);
}

