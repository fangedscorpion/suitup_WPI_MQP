#include "frameupdater.h"

#include <QMutexLocker>
#include <QTimer>
#include <QDebug>

FrameUpdater::FrameUpdater(int newFrameIncrement) : QObject() {
    currentFrame = 0;
    playing = false;
    frameIncrement = newFrameIncrement;
}

FrameUpdater::~FrameUpdater() {
    delete frameUpdateTimer;
}

bool FrameUpdater::startFrameUpdates(int timerDelay) {
    if (!playing) {

        playing = true;
        // start second thread
        // in second thread, start timer
        workerThread = new QThread(this);
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

void FrameUpdater::setCurrentFrameNum(qint32 newFrameNum) {
    if (playing) {
        QMutexLocker locker(&currentFrameMutex);
    }
    currentFrame = newFrameNum;

}

void FrameUpdater::issueFrameUpdate() {
    setCurrentFrameNum(getCurrentFrameNum() + frameIncrement);
}

