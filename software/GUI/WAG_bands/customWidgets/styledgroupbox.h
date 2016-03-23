#ifndef STYLEDGROUPBOX_H
#define STYLEDGROUPBOX_H

#include <QWidget>
#include <QLayout>
#include <QLabel>
#include <QPointer>

class StyledGroupBox : public QWidget {
    Q_OBJECT
public:
    StyledGroupBox(QString title, QPointer<QWidget> parent = 0);
    ~StyledGroupBox();
    QPointer<QVBoxLayout> getLayout() {return static_cast<QVBoxLayout*>(this->layout());}
    void setTitle(QString t);
private:
    QPointer<QLabel> label;
    QString title;
    void paintEvent(QPaintEvent *);
};

#endif // STYLEDGROUPBOX_H
