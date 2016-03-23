#pragma once
#include "overlay.h"
#include <QGraphicsDropShadowEffect>
#include "customshadoweffect.h"

class OverlayWidget : public Overlay {
    Q_OBJECT
public:
    OverlayWidget(QPointer<QWidget>parent, std::string title);
    ~OverlayWidget();

    QPointer<QVBoxLayout> getLayout();

public slots:
    void resizeWindow();
};
