#ifndef EDITINGCONTROLLER_H
#define EDITINGCONTROLLER_H

#include <QObject>
#include "motion.h"

class EditingController : public QObject
{
    Q_OBJECT
public:
    EditingController();
    void setActiveMotion(Motion *newMotion);

signals:
    void editingPlayStateChanged(bool);
public slots:
    void beginningSliderChanged(int);
    void endSliderChanged(int);
    void togglePlay();

private:
    bool playing;
    Motion * activeMotion;
};

#endif // EDITINGCONTROLLER_H
