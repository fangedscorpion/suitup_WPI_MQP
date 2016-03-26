#ifndef STYLEDCHECKBOX_H
#define STYLEDCHECKBOX_H

#include <QCheckBox>

class StyledCheckBox : public QCheckBox {
    Q_OBJECT
public:
    StyledCheckBox(QWidget *parent, QString title);
};

#endif // STYLEDCHECKBOX_H
