#pragma once
#include "overlay.h"
#include <QGraphicsDropShadowEffect>
#include "customshadoweffect.h"

class OverlayWidget : public Overlay {
    Q_OBJECT
public:
    OverlayWidget(QWidget *parent);
    ~OverlayWidget();

public slots:
    void resizeWindow();
};
