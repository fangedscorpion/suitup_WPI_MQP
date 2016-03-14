#ifndef RECORDINGCONTROLLER_H
#define RECORDINGCONTROLLER_H

#include <QObject>
#include "band/suit.h"
#include "wagfile.h"
#include "model/model.h"

class RecordingController : public QObject
{
    Q_OBJECT
public:
    RecordingController(Suit *newSuitObj, Model *m);

    void stopRecording();

    void startRecording();

    void setActiveMotion(WAGFile *motion);
private:
    Suit *suitObj;
    Model *model;
    QHash<qint32, PositionSnapshot> currentMotionData;
    WAGFile *activeMotion;
    bool voiceEnabled;
    bool recording;

signals:

public slots:
    void addSnapshotToMotion(qint32, PositionSnapshot);
    void catchVoiceControlCommand(MessageType);
    void toggleVoiceControl(bool voiceControlOn);


};

#endif // RECORDINGCONTROLLER_H
