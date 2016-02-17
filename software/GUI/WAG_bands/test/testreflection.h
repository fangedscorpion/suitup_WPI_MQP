#ifndef TESTREFLECTION_H
#define TESTREFLECTION_H

#include <QtTest/QtTest>

class TestReflection: public QObject
{
    Q_OBJECT
private slots:
    void testVector();
    void testQuaternion();
};

bool qFuzzyCompare2(QQuaternion q1, QQuaternion q2);

#endif // TESTREFLECTION_H
