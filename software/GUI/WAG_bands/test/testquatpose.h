#ifndef TESTQUATPOSE_H
#define TESTQUATPOSE_H

#include <QtTest/QtTest>

class TestQuatPose: public QObject
{
    Q_OBJECT
private slots:
    void testQuatPose();
    void testUpdate();

    void testCalibrate();
    void testError();
    void testQQinv();
};

#endif // TESTQUATPOSE_H
