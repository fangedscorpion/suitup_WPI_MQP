#ifndef STYLEDCHECKBOX_H
#define STYLEDCHECKBOX_H

#include <QCheckBox>

class StyledCheckBox : public QCheckBox {
    Q_OBJECT
public:
    StyledCheckBox(QString title, QWidget *parent = 0);
};

#endif // STYLEDCHECKBOX_H
