#include "absband.h"
#include <cstddef>
#include <QDebug>

QDataStream & operator>>(QDataStream & str, BandType & v) {
  unsigned int type = 0;
  str >> type;
  if (type)
    v = static_cast<BandType>(type);
  return str;
}

AbsBand::AbsBand(BandType bt):QObject() {
    type = bt;
    active = true;
    commsSetUp = false;
}

void AbsBand::handleConnectionStatusChange(ConnectionStatus newStatus) {
    if (newStatus == CONNECTED) {
        // send hi there message
        BandMessage *newMsg = new BandMessage(COMPUTER_INITIATE_CONNECTION, QByteArray());
        qDebug()<<"SENDING CONNECT MSG";
        qDebug()<<"BandT type: "<<type;
        emit dataToSend(type, newMsg);
    }
    else {
        // do we try to reconnect??
        commsSetUp = false;
    }
}

void AbsBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {

    AbsState *newState;
    switch(recvdMessage->getMessageType()) {
    case BAND_CONNECTING:
        if (!commsSetUp) {
            commsSetUp = true;
        }
        qDebug("recvd band connecting");
        break;
    case BAND_PING:
        if (pendingBandPing) {
            pendingBandPing = false;
        }
        qDebug("Recvd band ping");
        break;
    case BAND_POSITION_UPDATE:
        // parse into absstate
        newState = deserialize(recvdMessage->getMessageData(), this->getPositionRepresentation());
        updateState(newState, msgTimestamp);
        // should probably handle in subclass
        break;
    case LOW_BATTERY_UPDATE:
        emit lowBattery(type);
        break;
    default:
        // should never receive a message that isn't one of these types
        break;
    }
}

void AbsBand::sendIfConnected(BandMessage *sendMsg) {
    if (commsSetUp) {
        if (sendMsg->getMessageType() == COMPUTER_PING) {
            pendingBandPing = true;
            qDebug("Sending band ping");
        }
        emit dataToSend(type, sendMsg);
    }
}

void AbsBand::updateState(AbsState* state, qint32 msgTime){
    poseRecvdTime = msgTime;
    pose->update(state);
    updatePoints();
    emit poseRecvd(pose->getState(), type, poseRecvdTime);
}

void AbsBand::updatePoints(){
    pose->updatePoints(parentBand->getState(),parentBand->getEndpoint());
    for (unsigned int i = 0; i < childBands.size(); i++){
        childBands[i]->updatePoints();
    }
}

bool AbsBand::isConnected() {
    return commsSetUp;
}

AbsState *AbsBand::deserialize(QByteArray byteRep, PositionRepresentation positionRep) {
    AbsState *state;
    switch (positionRep) {
    case QUATERNION:
        float quatFloat[4];
        quatFloat[0] = ((byteRep[0] << 8) | byteRep[1]) / 16384.0f;
        quatFloat[1] = ((byteRep[2] << 8) | byteRep[3]) / 16384.0f;
        quatFloat[2] = ((byteRep[4] << 8) | byteRep[5]) / 16384.0f;
        quatFloat[3] = ((byteRep[6] << 8) | byteRep[7]) / 16384.0f;
        for (int i = 0; i < 4; i++) {
            if (quatFloat[i] >= 2) {
                quatFloat[i] = -4 + quatFloat[i];
            }
        }
        state = new QuatState(quatFloat[0], quatFloat[1], quatFloat[2], quatFloat[3]);
        break;
    default:
        break;
    }
    return state;
}

bool AbsBand::moveTo(AbsState* x) {
    if (!isActive())
        return true;

    IError * posError = pose->error(x);
    QByteArray msgData = posError->toMessage();
    BandMessage *newMsg = new BandMessage(POSITION_ERROR, msgData);
    emit dataToSend(type, newMsg);
    if (posError->withinTolerance(tolerance)) {
        return true;
    }
    return false;
}

void AbsBand::catchTolChange(int newTol) {
    tolerance = newTol;
}
