#ifndef TESTQUATERROR_H
#define TESTQUATERROR_H

#include <QtTest/QtTest>

class TestQuatError: public QObject
{
    Q_OBJECT
private slots:
    void testQuatError();
    void testToMessage();
    void testWithinTolerance();
};

#endif // TESTQUATERROR_H
