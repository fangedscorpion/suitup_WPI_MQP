#ifndef ABSHAPTICGENERATOR_H
#define ABSHAPTICGENERATOR_H

#include <QObject>


//  Matt, Chas, is the computer is determining the haptic feedback signal, or if the bands are doing it?
// if the computer is doing it, I made this class, so we can make one implementation but it could theoretically be swapped out




// classes actually implementing haptic feedback should extend this
// (want to make it flexible so different haptic feedback schemes are possible)

class AbsHapticGenerator : public QObject
{
    Q_OBJECT
public:
    AbsHapticGenerator();
    // virtual method with haptic feedback command as return and desired position and current position as inputs

    // @Matt - this is dependent on your stuff(again, if computer is generating haptic feedback signals)
    // virtual (return type [haptic response]) generateFeedback([either overall postion (desired/actual), single limb position (desired/actual), or error ])


signals:

public slots:

};

#endif // ABSHAPTICGENERATOR_H
