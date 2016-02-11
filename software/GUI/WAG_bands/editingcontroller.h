#ifndef EDITINGCONTROLLER_H
#define EDITINGCONTROLLER_H

#include <QObject>
#include "wagfile.h"

class EditingController : public QObject
{
    Q_OBJECT
public:
    EditingController();
    void setActiveMotion(WAGFile *newMotion);

signals:
    void editingPlayStateChanged(bool);
    void changeSliderMax(qint32);
public slots:
    void beginningSliderChanged(int);
    void endSliderChanged(int);
    void togglePlay();

private:
    bool playing;
    WAGFile * activeMotion;
};

#endif // EDITINGCONTROLLER_H
