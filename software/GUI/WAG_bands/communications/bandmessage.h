#ifndef BANDMESSAGE_H
#define BANDMESSAGE_H

#include <QObject>

// not sure if we need to have 2 different stops, can consolidate if desired
enum MessageType {
    COMPUTER_INITIATE_CONNECTION = 0,
    BAND_CONNECTING = 1,
    COMPUTER_PING = 2,
    BAND_PING = 3,
    BAND_POSITION_UPDATE = 4,
    POSITION_ERROR = 5,
    START_RECORDING = 6,
    STOP_RECORDING = 7,
    START_PLAYBACK = 8,
    STOP_PLAYBACK = 9,
    VOICE_CONTROL = 10
};


enum ConnectionStatus {CONNECTED, DISCONNECTED};

class BandMessage : public QObject
{
    Q_OBJECT
public:
   // BandMessage();
    BandMessage(MessageType msgType, QByteArray msgData);
    BandMessage(QByteArray fullMsg);

    int getMessageLength();
    MessageType getMessageType();
    QByteArray getMessageData();
    QByteArray getSerializedMessage();

signals:

public slots:

private:
   MessageType msgType;
   QByteArray msgData;
};

#endif // BANDMESSAGE_H
