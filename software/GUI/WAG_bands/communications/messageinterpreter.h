#ifndef MESSAGEINTERPRETER_H
#define MESSAGEINTERPRETER_H

#include <QObject>
#include <QList>

#define MSG_TYPE_LEN 1
#define DATA_LEN 2

// not sure if we need to have 2 different stops, can consolidate if desired
enum MessageType {COMPUTER_INITIATE_CONNECTION, BAND_CONNECTING, COMPUTER_PING, BAND_PING, BAND_POSITION_UPDATE, POSITION_ERROR,
                  START_RECORDING, STOP_RECORDING, START_HAPTICS, STOP_HAPTICS};


class MessageInterpreter:public QObject
{
    Q_OBJECT
public:
    MessageInterpreter();
    MessageType getMessageType(QByteArray fullMsg);
    int getDataLength(QByteArray fullMsg);
    QByteArray getData(int dataLength, QByteArray fullMsg);

    QByteArray constructMessage(QByteArray data, MessageType msgType);

    // not sure if will catch multiple messages on 1 recv, if so write this
    QList<QByteArray> *separateMessages(QByteArray messageCompilation);

    // maybe add methods for parsing data from different message types into meaningful objects (@Matt)
};

#endif // MESSAGEINTERPRETER_H
