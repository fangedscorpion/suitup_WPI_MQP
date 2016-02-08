#ifndef BANDMESSAGE_H
#define BANDMESSAGE_H

#include <QObject>

// not sure if we need to have 2 different stops, can consolidate if desired
enum MessageType {COMPUTER_INITIATE_CONNECTION, BAND_CONNECTING, COMPUTER_PING, BAND_PING, BAND_POSITION_UPDATE, POSITION_ERROR,
                  START_RECORDING, STOP_RECORDING, START_PLAYBACK, STOP_PLAYBACK, VOICE_CONTROL};


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
