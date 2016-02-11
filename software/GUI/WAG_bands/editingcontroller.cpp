#include "editingcontroller.h"
#include <QDebug>

EditingController::EditingController() : QObject()
{
    playing = false;
}



void EditingController::beginningSliderChanged(int sliderVal) {
    qDebug()<<"editing mode, beginning slider val: "<<sliderVal;
}

void EditingController::endSliderChanged(int sliderVal) {

    qDebug()<<"editing mode, end slider val: "<<sliderVal;
}

void EditingController::togglePlay() {
    playing = !playing;
    // play motion

    qDebug()<<"editing mode, playing: "<<playing;
    emit editingPlayStateChanged(playing);
}

void EditingController::setActiveMotion(WAGFile *editMotion) {
    activeMotion = editMotion;
    qint32 newSliderMax = activeMotion->getFrameNums();
    emit changeSliderMax(newSliderMax);
}
