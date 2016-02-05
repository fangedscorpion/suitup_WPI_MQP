#include "bandmessage.h"

/* BandMessage::BandMessage() : QObject() {

} */

BandMessage::BandMessage(MessageType msgType, QByteArray msgData){
    this->msgType = msgType;
    this->msgData = msgData;
}

BandMessage::BandMessage(QByteArray fullMsg) {
    int expectedLen = (int) fullMsg[0];
    this->msgType = (MessageType) fullMsg.at(1);
    QByteArray data = fullMsg.remove(0, 2);
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
    return fullMsg;
}
