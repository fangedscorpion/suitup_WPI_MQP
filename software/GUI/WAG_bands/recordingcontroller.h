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

signals:

public slots:
    void addSnapshotToMotion(qint64, PositionSnapshot);
};

#endif // RECORDINGCONTROLLER_H
