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

protected:
    void timerEvent(QTimerEvent *);

public slots: // some of these might be better as normal functions
    // but for now, I'm making them slots so they can be called asynchronously
    void togglePlay();
    void setStepThroughMode(bool steppingThrough);
    void toggleVoiceControl();
    void toggleSuitActive(bool active);
    void moveFramePointer(int newFrame);
    void modifyHoldTime(double holdSeconds);
    void updateStepThroughTolerance(int sliderValue);
    void speedChanged(int sliderValue);
    void positionMet();
    void computeTimeInFile(int frameNum);

signals:
    void frameChanged(int newFrame);
    void startPlayback();
    void stopPlayback();
    void endOfTimeRange();
    void timeChanged(int millis);
    void playbackStateChanged(bool playing);

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
    int timerId;
    int lastFrameNum;

    void changeFrameRate(float newFrameRate);
    void setStepThroughInterval(int newInterval);
    void setStepThroughTolerance(float newTolerance);
    void startPlaying();
    void stopPlaying();
};

#endif // PLAYBACKCONTROLLER_H
