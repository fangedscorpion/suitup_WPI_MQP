#include "absband.h"
#include <cstddef>
#include <QDebug>
#include <stdexcept>

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
    pingProblems = 0;
    hasLowBattery = false;
}

AbsBand::~AbsBand(){
    delete pose;
}

void AbsBand::handleConnectionStatusChange(ConnectionStatus newStatus) {
    if (newStatus == CONNECTED) {
        // send hi there message
        BandMessage *newMsg = new BandMessage(COMPUTER_INITIATE_CONNECTION, QByteArray());
        qDebug()<<"AbsBand: SENDING CONNECT MSG";
        qDebug()<<"AbsBand: Band type: "<<type;
        emit dataToSend(type, newMsg);
    }
    else {
        // do we try to reconnect??
        commsSetUp = false;
    }
}

void AbsBand::handleMessage(qint32 msgTimestamp, BandMessage *recvdMessage) {
    bool tmpLowBattery;
    qDebug()<<"AbsBand: message type:"<<recvdMessage->getMessageType();
    switch (recvdMessage->getMessageType()) {
    case VOICE_CONTROL_LOW_BATT:
    case BAND_CONNECTING_LOW_BATT:
    case BAND_PING_LOW_BATT:
    case BAND_POSITION_UPDATE_LOW_BATT:
    case LOW_BATTERY_UPDATE:
        //qDebug()<<"AbsBand:: has low battery";
        tmpLowBattery = true;
        break;
    default:
        tmpLowBattery = false;
    }

    if (tmpLowBattery != hasLowBattery) {
        hasLowBattery = tmpLowBattery;
        emit lowBattery(type, hasLowBattery);
    }

    AbsState *newState;
    switch(recvdMessage->getMessageType()) {
    case BAND_CONNECTING_LOW_BATT:
    case BAND_CONNECTING:
        if (!commsSetUp) {
            commsSetUp = true;
        }
        qDebug("AbsBand: recvd band connecting");
        break;
    case BAND_PING_LOW_BATT:
    case BAND_PING:
        if (pendingBandPing) {
            pendingBandPing = false;
        }
        break;
    case BAND_POSITION_UPDATE_LOW_BATT:
    case BAND_POSITION_UPDATE:
        // parse into absstate

        // this print line is necessary or the program will crash. cannot figure out why
        //qDebug()<<"AbsBand: BAND POSITION RECEIVED FROM"<<type<<" at "<<msgTimestamp;
        //qDebug()<<"ABsBand: position data "<<recvdMessage->getMessageData();
        newState = deserialize(recvdMessage->getMessageData(), this->getPositionRepresentation());
        updateState(newState, msgTimestamp);
        // should probably handle in subclass
        break;
    default:
        // should never receive a message that isn't one of these types
        break;
    }
}

void AbsBand::sendIfConnected(BandMessage *sendMsg) {
    if (commsSetUp) {
        if (sendMsg->getMessageType() == COMPUTER_PING) {
            if (pendingBandPing) {
                pingProblems++;
                if (pingProblems >= PING_PROBLEMS_THRESHOLD) {
                    // should've already sent back a band ping
                    qDebug()<<"AbsBand:: Connection problem for band "<<type;
                    emit connectionProblem(type);
                    pendingBandPing = false;
                    pingProblems = 0;
                    return;
                }
            }
            pendingBandPing = true;
            pingProblems = 0;
            qDebug("AbsBand: Sending band ping");
        }
        qDebug()<<"AbsBand: sending message type "<<sendMsg->getMessageType();
        emit dataToSend(type, sendMsg);
    }
//    delete sendMsg;
}

void AbsBand::updateState(AbsState* state, qint32 msgTime){
    poseRecvdTime = msgTime;
    pose->update(state);

    emit poseRecvd(pose->getState(), type, poseRecvdTime);
}

bool AbsBand::isConnected() {
    return commsSetUp;
}

//void AbsBand::assignNode(QSharedPointer<Node> node){
//    pose->assignNode(node);
//}

AbsState *AbsBand::deserialize(QByteArray byteRep, PositionRepresentation positionRep) {
    AbsState *state;
    switch (positionRep) {
    case QUATERNION:
        float quatFloat[4];
        quatFloat[0] = (((quint8)byteRep[0] << 8) | (quint8)byteRep[1]) / 16384.0f;
        quatFloat[1] = (((quint8)byteRep[2] << 8) | (quint8)byteRep[3]) / 16384.0f;
        quatFloat[2] = (((quint8)byteRep[4] << 8) | (quint8)byteRep[5]) / 16384.0f;
        quatFloat[3] = (((quint8)byteRep[6] << 8) | (quint8)byteRep[7]) / 16384.0f;
        for (int i = 0; i < 4; i++) {
            if (quatFloat[i] >= 2) {
                quatFloat[i] = -4 + quatFloat[i];
            }
        }
        state = new QuatState(quatFloat[0], -quatFloat[1], quatFloat[3], quatFloat[2]);
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
    //qDebug()<<"AbsBand: sending error message";
    emit dataToSend(type, newMsg);
    if (posError->withinTolerance(tolerance)) {
        return true;
    }
    return false;
}

void AbsBand::catchTolChange(int newTol) {
    tolerance = newTol;
}

QString AbsBand::bandTypeToString(BandType stringifyThis) {
    switch(stringifyThis) {
    case LEFT_HAND:
        return QString("left hand band");
        break;
    case RIGHT_HAND:
        return QString("right hand band");
        break;
    case LEFT_LOWER_ARM:
        return QString("left lower arm band");
        break;
    case RIGHT_LOWER_ARM:
        return QString("right lower arm band");
        break;
    case LEFT_UPPER_ARM:
        return QString("left upper arm band");
        break;
    case RIGHT_UPPER_ARM:
        return QString("right upper arm band");
        break;
    case LEFT_SHOULDER:
        return QString("left shoulder band");
        break;
    case RIGHT_SHOULDER:
        return QString("right shoulder band");
        break;
    case CHEST:
        return QString ("chest band");
        break;
    default:
        return QString("unknown band");
        break;
    }
}

QString AbsBand::bandTypeToModelName(BandType b){
    switch(b){
    case LEFT_LOWER_ARM:
        return QString("Forearm.L");
    case LEFT_UPPER_ARM:
        return QString("UpperArm.L");
    case LEFT_SHOULDER:
        return QString("Shoulder.L");
    case RIGHT_SHOULDER:
        return QString("Shoulder.R");
    case RIGHT_UPPER_ARM:
        return QString("UpperArm.R");
    case RIGHT_LOWER_ARM:
        return QString("Forearm.R");
    case CHEST:
        return QString("Chest");
    default:
        throw std::invalid_argument("unknown bandtype");
    }
}
