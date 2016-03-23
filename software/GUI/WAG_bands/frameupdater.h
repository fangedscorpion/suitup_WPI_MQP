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
    explicit FrameUpdater(QObject *parent = 0);
    bool startFrameUpdates(int timerDelay);
    bool stopFrameUpdates();
    qint32 getCurrentFrameNum();
    void setCurrentFrameNum(qint32);
    void setFrameIncrement(int newFrameInc);

signals:
    void stopTimer();
public slots:
    void issueFrameUpdate();
private:
    qint32 currentFrame;
    bool playing;
    QThread *workerThread;
    mutable QMutex currentFrameMutex;
    mutable QMutex frameIncMutex;
    int frameIncrement;
    QTimer *frameUpdateTimer;
    int getFrameIncrement();
};

#endif // FRAMEUPDATER_H
