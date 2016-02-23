#ifndef PLAYBACKCONTROLLER_H
#define PLAYBACKCONTROLLER_H

#include <QObject>
#include "wagfile.h"
#include <QString>
#include "band/suit.h"

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
    void toggleVoiceControl();
    void toggleSuitActive(bool active);
    void moveFramePointer(int newFrame);
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
    void startPlayback();
    void stopPlayback();
    void endOfTimeRange();
    void metPosition();
    void playbackStateChanged(bool playing);
    void goToSnapshot(PositionSnapshot);
    void changeSliderMax(qint32);
    void toleranceChanged(int);
    void changeSliderVal(int);



private:
    bool playing;
    bool stepThrough;
    float frameRate;
    bool voiceControl;
    bool suitActive;
    int currentFrame;
    int timeToHoldFrameMillis;
    int stepThroughInterval; // number of frames to jump between poses to match in step through mode
    WAGFile *activeMotion;
    //Time timeToHoldFinalFrame;
    //Time stepThroughInterval;
    int stepThroughTolerance;
    int timerId;
    int lastFrameNum;
    Suit *suitObj;

    void changeFrameRate(float newFrameRate);
    void setStepThroughInterval(int newInterval);
    void setStepThroughTolerance(float newTolerance);
    void startPlaying();
    void stopPlaying();
};

#endif // PLAYBACKCONTROLLER_H
