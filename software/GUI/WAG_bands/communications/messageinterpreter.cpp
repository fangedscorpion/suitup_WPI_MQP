#include "messageinterpreter.h"

MessageInterpreter::MessageInterpreter():QObject()
{

}

MessageType MessageInterpreter::getMessageType(QByteArray fullMsg) {
    // TODO
    return COMPUTER_PING;
}

int MessageInterpreter::getDataLength(QByteArray fullMsg) {
    // TODO
    return 1;

}

QByteArray MessageInterpreter::getData(int dataLength, QByteArray fullMsg) {
    // TODO
    return QByteArray();

}

QByteArray MessageInterpreter::constructMessage(QByteArray data, MessageType msgType) {
    // TODO
    return QByteArray();
}

// not sure if will catch multiple messages on 1 recv, if so write this
QList<QByteArray> * MessageInterpreter::separateMessages(QByteArray messageCompilation) {
    // TODO
    return new QList<QByteArray>();
}
