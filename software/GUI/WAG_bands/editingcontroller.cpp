#include "editingcontroller.h"
#include <QDebug>

EditingController::EditingController() : QObject()
{
    playing = false;
}



void EditingController::beginningSliderChanged(int sliderVal) {
    qDebug()<<"EditingController: beginning slider val: "<<sliderVal;
}

void EditingController::endSliderChanged(int sliderVal) {
    qDebug()<<"EditingController: end slider val: "<<sliderVal;
}

void EditingController::togglePlay() {
    playing = !playing;
    // play motion

    qDebug()<<"EditingController: playing: "<<playing;
    emit editingPlayStateChanged(playing);
}

void EditingController::setActiveMotion(WAGFile *editMotion) {
    activeMotion = editMotion;
    qint32 newSliderMax = activeMotion->getFrameNums();
    emit changeSliderMax(newSliderMax);
}
