#include "bandmessage.h"
#include "qdebug.h"

BandMessage::BandMessage(MessageType msgType, QByteArray msgData){
    this->msgType = msgType;
    this->msgData = msgData;
}

BandMessage::BandMessage() {}

void BandMessage::parseFromByteArray(QByteArray fullMsg) {
    int expectedLen = (int) fullMsg[0];
    this->msgType = (MessageType) fullMsg.at(1);
    QByteArray data = fullMsg.remove(0, 2);

    this->msgData = data;
    // data.length -1 (full packet (not including first two bytes) minus new line)
    // should equal the expected length embedded in the packet (which is the data length + 2)
    if ((data.length() - 1) != (expectedLen -2)) {
        // error of some sort
        qDebug()<<"BandMessage: Expected length: "<<expectedLen -2;
        qDebug()<<"BandMessage: Actual length: "<<data.length();
        // theintended length for the dat of this message is the expected length field in the message (first byte)
        // minus 2 (message type field and length field are both 1 byte)
        IncorrectDataLengthException *newException = new IncorrectDataLengthException(expectedLen - 2);
        throw newException;
    } else {
        // this message is it's own message, so we should remove the new line
        data.remove(data.length() -1, 1);
    }
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

/*
 * Call this if an IncorrectDataLengthException is thrown trying to parse a MessageType
 * this repairs the initial message and pulls out the extra bytes so that they can be parsed into another Message
 */
QByteArray BandMessage::handleException(IncorrectDataLengthException *e) {
    int intendedLength = e->getIntendedLength();
    if (intendedLength > this->getMessageLength()) {
        return QByteArray();
    }
    // get the next packet that was lumped in with this message (intended length + 1 so we remove the new line that went with the first message)
    QByteArray extraPacketData = this->getMessageData().right(this->getMessageLength() - (intendedLength + 1));
    // shorten the data to what was actually supposed to be int heis message
    this->msgData = this->getMessageData().left(intendedLength);

    return extraPacketData;
}
