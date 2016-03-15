#ifndef RECORDINGCONTROLLER_H
#define RECORDINGCONTROLLER_H

#include <QObject>
#include "band/suit.h"
#include "wagfile.h"

class RecordingController : public QObject
{
    Q_OBJECT
public:
    RecordingController(Suit *newSuitObj);

    void stopRecording();

    void startRecording();

    void setActiveMotion(WAGFile *motion);
private:
    Suit *suitObj;
    QHash<qint32, PositionSnapshot> currentMotionData;
    WAGFile *activeMotion;
    bool voiceEnabled;
    bool recording;

signals:
    void vcChangeState(bool);
public slots:
    void addSnapshotToMotion(qint32, PositionSnapshot);
    void catchVoiceControlCommand(MessageType);
    void toggleVoiceControl(bool voiceControlOn);


};

#endif // RECORDINGCONTROLLER_H
