#ifndef FRAMEUPDATER_H
#define FRAMEUPDATER_H

#include <QObject>
#include <QMutex>
#include <QTimer>
#include <QThread>

class FrameUpdater : public QObject
{
    Q_OBJECT
public:
    FrameUpdater(int newFrameIncrement);
    bool startFrameUpdates(int timerDelay);
    bool stopFrameUpdates();
    qint32 getCurrentFrameNum();
    void setCurrentFrameNum(qint32);

signals:
    void stopTimer();
public slots:
    void issueFrameUpdate();
private:
    qint32 currentFrame;
    bool playing;
    QThread *workerThread;
    mutable QMutex currentFrameMutex;
    int frameIncrement;
    QTimer *frameUpdateTimer;
};

#endif // FRAMEUPDATER_H
