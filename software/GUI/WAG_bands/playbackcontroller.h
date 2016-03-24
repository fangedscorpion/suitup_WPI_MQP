#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <QObject>
#include "wagfile.h"
#include <QString>
#include "band/suit.h"
#include "model/model.h"
#include "frameupdater.h"

class PlaybackController:public QObject {
    Q_OBJECT
public:
    PlaybackController(Suit *);
    void setActiveMotion(WAGFile *);

protected:
    void timerEvent(QTimerEvent *);

public slots: // some of these might be better as normal functions
    // but for now, I'm making them slots so they can be called asynchronously
    void togglePlay();
    void setStepThroughMode(bool steppingThrough);
    void toggleVoiceControl(bool enable);
    void toggleSuitActive(bool active);
    void currentFrameChanged(int currentFrameSliderPos);
    void modifyHoldTime(double holdSeconds);
    void updateStepThroughTolerance(int sliderValue);
    void speedChanged(int sliderValue);
    void positionMet();
    void beginningSliderChanged(int);
    void endSliderChanged(int);
    void catchFrameUpdate(qint32 newFrame);
    void catchVoiceControlCommand(MessageType vcCommandInstruction);
    void catchFrameNumsChanged(qint32 newLastNum);

signals:
    void frameChanged(qint32 newFrame);
    void beginningTimeChanged(qint32 beginTime);
    void endTimeChanged(qint32 endTime);
    void totalTimeChanged(qint32 totalTime);

    void startPlayback();
    void stopPlayback();
    void endOfTimeRange();
    void metPosition();
    void playbackStateChanged(bool playing);
    void goToSnapshot(PositionSnapshot*);
    void changeSliderMax(qint32);
    void toleranceChanged(int);
    void changeSliderVal(int);
    void playingOnSuit(bool playing);



private:
    bool playing;
    bool stepThrough;
    float frameRate;
    bool voiceControl;
    bool suitActive;
   // int currentFrame;
    int timeToHoldFrameMillis;
    int stepThroughInterval; // number of frames to jump between poses to match in step through mode
    QPointer<WAGFile> activeMotion;
    //Time timeToHoldFinalFrame;
    //Time stepThroughInterval;
    int stepThroughTolerance;
    int timerId;
    int lastFrameNum;
    QPointer<Suit> suitObj;

    // These indicate the positions of the beginning and ending sliders on the super slider
    // should be 0 - 100
    int beginningPointer;
    int endPointer;

    void changeFrameRate(float newFrameRate);
    void setStepThroughInterval(int newInterval);
    void setStepThroughTolerance(float newTolerance);
    void startPlaying();
    void stopPlaying();
    void reachedEndOfTimeRange();
    void updateFrameWithoutSuitNotification(int newFrame);
    FrameUpdater *updater;
};

#endif // PLAYBACKCONTROLLER_H
