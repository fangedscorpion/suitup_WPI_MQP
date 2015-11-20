#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <QObject>
#include "motion.h"
#include <QString>

class PlaybackController:public QObject {
    Q_OBJECT
public:
    PlaybackController();
    Motion loadMotionFrom(QString fileLocation);
    bool playMotion(Motion motionToPlay); // play from currentFrame to end

public slots: // some of these might be better as normal functions
    // but for now, I'm making them slots so they can be called asynchronously
    void togglePlay();
    void setStepThrough(bool shouldStepThrough);
    void changeFrameRate(float newFrameRate);
    void toggleVoiceControl();
    void toggleSuitActive();
    void moveFramePointer(int newFrame);
    void modifyHoldTime(int newHoldTimeMillis);
    void setStepThroughInterval(int newInterval);
    void setStepThroughTolerance(float newTolerance);

signals:
    void frameChanged(int newFrame);

private:
    bool playing;
    bool stepThrough;
    float frameRate;
    bool voiceControl;
    bool suitActive;
    int currentFrame;
    int timeToHoldFrameMillis;
    int stepThroughInterval; // number of frames to jump between poses to match in step through mode
    //Time timeToHoldFinalFrame;
    //Time stepThroughInterval;
    float stepThroughTolerance;
};

#endif // PLAYBACKCONTROLLER_H
