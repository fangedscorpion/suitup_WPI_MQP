#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include "glwidget.h"
#include "positionsnapshot.h"

class Visualization
{
public:
    Visualization(GLWidget* widget);
    void displaySnapshot(PositionSnapshot snapshot);
private:
    GLWidget *widget;
};

#endif // VISUALIZATION_H
