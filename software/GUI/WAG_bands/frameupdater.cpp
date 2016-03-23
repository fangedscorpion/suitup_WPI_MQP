#include "frameupdater.h"

#include <QMutexLocker>
#include <QTimer>
#include <QDebug>

FrameUpdater::FrameUpdater(QObject *parent) : QObject(parent)
{
    currentFrame = 0;
    playing = false;
    frameIncrement = 100;
    qDebug()<<"FrameUpdater: main thread"<<QThread::currentThreadId();
}

bool FrameUpdater::startFrameUpdates(int timerDelay) {
    if (!playing) {

        playing = true;
        // start second thread
        // in second thread, start timer
        qDebug()<<"FrameUpdater: starting timer";
        workerThread = new QThread(this);
        qDebug()<<"FrameUpdater: thread created: "<<workerThread->currentThreadId();
        frameUpdateTimer = new QTimer(0);
        connect(frameUpdateTimer, SIGNAL(timeout()), this, SLOT(issueFrameUpdate()), Qt::DirectConnection);
        connect(this, SIGNAL(stopTimer()), frameUpdateTimer, SLOT(stop()));
        frameUpdateTimer->setInterval(timerDelay);

        frameUpdateTimer->moveToThread(workerThread);

        connect(workerThread, SIGNAL(started()), frameUpdateTimer, SLOT(start()));
        workerThread->start(QThread::HighPriority);

        return true;
    }
    return false;
}

bool FrameUpdater::stopFrameUpdates() {
    if (playing) {
        // stop second thread
        qDebug()<<"FrameUpdater: stopping timer";
        //frameUpdateTimer->stop();
        emit stopTimer();
        connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));
        workerThread->quit();
        playing = false;
        return true;
    }
    return false;
}

qint32 FrameUpdater::getCurrentFrameNum() {
    if (playing) {
        QMutexLocker locker(&currentFrameMutex);
    }
    return currentFrame;
}

void FrameUpdater::setCurrentFrameNum(qint32 newFrameNufm) {
    if (playing) {
        QMutexLocker locker(&currentFrameMutex);
    }
    currentFrame = newFrameNum;

}

void FrameUpdater::issueFrameUpdate() {
    qDebug()<<"FrameUpdater: current thread "<<QThread::currentThreadId();
    setCurrentFrameNum(getCurrentFrameNum() + getFrameIncrement());
}

void FrameUpdater::setFrameIncrement(int newFrameInc) {
    if (playing) {
        QMutexLocker locker(&frameIncMutex);
    }
    frameIncrement = newFrameInc;
}

int FrameUpdater::getFrameIncrement() {
    if (playing) {
        QMutexLocker locker(&frameIncMutex);
    }
    return frameIncrement;
}
