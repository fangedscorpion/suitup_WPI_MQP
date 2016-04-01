#ifndef STYLEDGROUPBOX_H
#define STYLEDGROUPBOX_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QSharedPointer>

class StyledGroupBox : public QWidget {
    Q_OBJECT
public:
    StyledGroupBox(QWidget* parent, QString title);
    ~StyledGroupBox(){}
    QVBoxLayout* getLayout() {return static_cast<QVBoxLayout*>(this->layout());}
    void setTitle(QString t);
private:
    QLabel* label;
    QString title;
    void paintEvent(QPaintEvent *);
};

#endif // STYLEDGROUPBOX_H
