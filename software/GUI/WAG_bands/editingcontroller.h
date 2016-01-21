#ifndef EDITINGCONTROLLER_H
#define EDITINGCONTROLLER_H

#include <QObject>

class EditingController : public QObject
{
    Q_OBJECT
public:
    EditingController();

signals:
    void editingPlayStateChanged(bool);
public slots:
    void beginningSliderChanged(int);
    void endSliderChanged(int);
    void togglePlay();

private:
    bool playing;
};

#endif // EDITINGCONTROLLER_H
