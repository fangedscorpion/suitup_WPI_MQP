#include "bandmessage.h"

BandMessage::BandMessage(MessageType msgType, QByteArray msgData){
    this->msgType = msgType;
    this->msgData = msgData;
}

BandMessage::BandMessage(QByteArray fullMsg) {
    int expectedLen = (int) fullMsg[0];
    this->msgType = (MessageType) fullMsg.at(1);
    QByteArray data = fullMsg.remove(0, 2);
    // remove new line
    data.remove(data.length() -1, 1);
    if (data.length()  != expectedLen) {
        // error of some sort
    }
    this->msgData = data;
}

MessageType BandMessage::getMessageType() {
    return msgType;
}

QByteArray BandMessage::getMessageData() {
    return msgData;
}

int BandMessage::getMessageLength(){
    return msgData.length();
}

QByteArray BandMessage::getSerializedMessage() {
    char length = (char) ((this->getMessageLength()) + 2);
    QByteArray fullMsg = QByteArray(&length, 1);
    char typeStr = (char) msgType;
    fullMsg.append(&typeStr, 1);
    fullMsg.append(msgData);
    fullMsg.append('\n');
    return fullMsg;
}

VoiceControlMsgType BandMessage::parseVoiceControlMsg() {
    QByteArray data = getMessageData();
    if (getMessageLength() == VOICE_CONTROL_DATA_LEN) {
        const char *constMsgData = data.constData();
        // for now, assuming VOICE_CONTROL_DATA_LEN is 1, if not, have to change equality check
        if (constMsgData[0] == VOICE_CONTROL_START) {
            return VC_START;
        } else if (constMsgData[0] == VOICE_CONTROL_STOP){
            return VC_STOP;
        } else {
            return OTHER;
        }
    } else {
        // ERROR
        return OTHER;
    }
}
