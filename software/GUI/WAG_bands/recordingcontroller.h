#ifndef RECORDINGCONTROLLER_H
#define RECORDINGCONTROLLER_H

#include "motion.h"

#include <QObject>
#include "band/suit.h"

class RecordingController : public QObject
{
    Q_OBJECT
public:
    RecordingController();

    void setSuit(Suit *newSuitObj);

    Motion stopRecording();

    void startRecording();
private:
    Suit *suitObj;
    QHash<qint64, PositionSnapshot> currentMotion;

signals:

public slots:
    void addSnapshotToMotion(qint64, PositionSnapshot);
};

#endif // RECORDINGCONTROLLER_H
