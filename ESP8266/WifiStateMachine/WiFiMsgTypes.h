#ifndef WiFiMsgTypes_h
#define WiFiMsgTypes_h

//#include "WAGBandCommon.h"

// #define COMPUTER_INITIATE_CONNECTION 0
// #define BAND_CONNECTING 1
// #define COMPUTER_PING 2
// #define BAND_PING 3
// #define BAND_POSITION_UPDATE 4
// #define POSITION_ERROR 5
// #define START_RECORDING 6
// #define STOP_RECORDING 7
// #define START_PLAYBACK 8
// #define STOP_PLAYBACK 9
// #define VOICE_CONTROL 10

typedef enum MessageType {
    COMPUTER_INITIATE_CONNECTION = 0, 	//From PC to Band --> starts up TCP connection from band listener
    BAND_CONNECTING = 1, 				//From band to PC --> shows band is starting connection
    COMPUTER_PING = 2, 					//From PC to Band --> checks band is connected
    BAND_PING = 3,						//From PC to Band --> starts up TCP connection from band listener
    BAND_POSITION_UPDATE = 4,			//From band to Band --> contains new data for PC (12 bytes of data)
    POSITION_ERROR = 5,					//From PC to Band --> Tells the band what the error in position is (12 bytes of data)
    START_RECORDING = 6,				//From PC to Band --> Tells band to stream accel/gyro data
    STOP_RECORDING = 7,					//From PC to Band --> Tells band to stop sending data
    START_PLAYBACK = 8,					//From PC to Band --> Tells band to start motors
    STOP_PLAYBACK = 9,					//From PC to Band --> Tells band to stop any motor feedback/data
    VOICE_CONTROL = 10,					//From Band to PC --> Gives PC voice control data from chestpiece
    LOW_BATTERY_UPDATE = 11,			//From Band to PC --> Tells PC that band voltage is way low --> charge it
} MsgTypes;

//Extra things to hold spots for enums when not being used for checking
#define BROKEN_PACKET 255
#define NOTHING_NEW 254

#define POSITION_DATA_BYTES 12
#define NON_POSITION_DATA_BYTES_MAX 4
#define RECORDING_MSG_SIZE 11

#endif
