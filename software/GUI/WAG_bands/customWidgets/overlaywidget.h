#pragma once
#include "overlay.h"
#include "customshadoweffect.h"

class OverlayWidget : public Overlay {
    Q_OBJECT
public:
    OverlayWidget(QWidget* parent, std::string title);
    ~OverlayWidget();

    QVBoxLayout* getLayout();

public slots:
    void resizeWindow();
};
