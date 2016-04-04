#ifndef BANDMESSAGE_H
#define BANDMESSAGE_H

#include <QObject>
#include "incorrectdatalengthexception.h"

#define VOICE_CONTROL_START 67
#define VOICE_CONTROL_STOP 15
#define VOICE_CONTROL_DATA_LEN 1
// change these once here from Chas

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
    VOICE_CONTROL = 10,
    LOW_BATTERY_UPDATE = 11,
    BAND_CONNECTING_LOW_BATT = 12,                   //From band to PC --> shows band is starting connection    (LOW BATTERY!!!)
    BAND_PING_LOW_BATT = 13,                         //From PC to Band --> starts up TCP connection from band listener  (LOW BATTERY!!!)
    BAND_POSITION_UPDATE_LOW_BATT = 14,              //From band to Band --> contains new data for PC (12 bytes of data)    (LOW BATTERY!!!)
    VOICE_CONTROL_LOW_BATT = 15,
};

enum VoiceControlMsgType {
    VC_START = VOICE_CONTROL_START,
    VC_STOP = VOICE_CONTROL_STOP,
    OTHER = 6
};


enum ConnectionStatus {CONNECTED, DISCONNECTED};

class BandMessage : public QObject
{
    Q_OBJECT
public:
   // BandMessage();
    BandMessage(MessageType msgType, QByteArray msgData);
    BandMessage();
    ~BandMessage(){}
    void parseFromByteArray(QByteArray fullMsg);
    QByteArray handleException(IncorrectDataLengthException *e);


    int getMessageLength();
    MessageType getMessageType();
    QByteArray getMessageData();
    QByteArray getSerializedMessage();
    VoiceControlMsgType parseVoiceControlMsg();

signals:

public slots:

private:
   MessageType msgType;
   QByteArray msgData;
};

#endif // BANDMESSAGE_H
