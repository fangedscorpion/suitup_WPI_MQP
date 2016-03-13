#ifndef STYLEDGROUPBOX_H
#define STYLEDGROUPBOX_H

#include <QWidget>
#include <QLayout>
#include <QLabel>

class StyledGroupBox : public QWidget {
    Q_OBJECT
public:
    StyledGroupBox(QString title, QWidget *parent = 0);
    ~StyledGroupBox();
    QVBoxLayout* getLayout() {return static_cast<QVBoxLayout*>(this->layout());}
    void setTitle(QString t);
private:
    QLabel *label;
    QString title;
    void paintEvent(QPaintEvent *);
};

#endif // STYLEDGROUPBOX_H