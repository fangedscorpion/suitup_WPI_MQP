#ifndef EDITINGCONTROLLER_H
#define EDITINGCONTROLLER_H

#include <QObject>
#include "wagfile.h"
#include "model/model.h"

class EditingController : public QObject
{
    Q_OBJECT
public:
    EditingController();
    void setActiveMotion(WAGFile *newMotion);
signals:
    void editingPlayStateChanged(bool);
    void changeSliderMax(qint32);
    void frameChanged(qint32);
    void beginningTimeChanged(qint32 beginTime);
    void endTimeChanged(qint32 endTime);
    void totalTimeChanged(qint32 totalTime);


    void stopPlayback();
    void startPlayback();
    void endOfTimeRange();
    void changeSliderVal(int);
    void goToSnapshot(PositionSnapshot);

public slots:
    void beginningSliderChanged(int);
    void endSliderChanged(int);
    void togglePlay();
    void catchFrameNumsChanged(qint32 newLastNum);
    void catchFrameUpdate(qint32 newFrame);
    void currentFrameChanged(int currentFrameSliderPos);

private:
    bool playing;
    int beginningPointer;
    int endPointer;
    WAGFile * activeMotion;
    qint32 currentFrame;
    qint32 lastFrameNum;
    int timerId;

    void reachedEndOfTimeRange();
    void stopPlaying();
    void startPlaying();

protected:
    void timerEvent(QTimerEvent *event);
};

#endif // EDITINGCONTROLLER_H
