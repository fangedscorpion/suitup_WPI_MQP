#ifndef TESTQUATPOSE_H
#define TESTQUATPOSE_H

#include <QtTest/QtTest>

class TestQuatPose: public QObject
{
    Q_OBJECT
private slots:
    void testQuatPose();
    void testUpdate();
    void testGetCalibrationState();
    void testGetState();
    void testGetEndpoint();

    void testCalibrate();
    void testAdjust();
    void testError();
    void testUpdatePoints();
    void testQQinv();
};

#endif // TESTQUATPOSE_H
