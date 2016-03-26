#ifndef ESP8266Comms_h
#define ESP8266Comms_h

/*

	This file contains all the code related to listening to the ESP8266 module

*/
#include <Arduino.h>
#include "WAGBandCommon.h"

/* 		
	MESSAGE STRUCTURE FROM ESP8266

	Total of (ESP8266_ALIGN_BYTES + ESP8266_MSG_SIZE) bytes long

	Packet content:
	{ESP8266_START_BYTE, ESP8266_START_BYTE, ESP8266_START_BYTE, <(1 byte > ESP8266_MIN_CMD_BYTE)>, <(4 bytes = 1 float)>, <(4 bytes = 1 float)>, <(4 bytes = 1 float)>}

	After the three start bytes there is a command which is can be any of the ESP8266_CMD_... defines
	Some functionality makes the Teensy read the rest of the Serial data, others just quit and go back to the main loop
		(See the readFromESP8266() function for more details)

	After the command are an optional 12 bytes of data for 3 floats indicating the trans_angle, err_trans and err_rot from the VibrationPattern.h file

	Note that this function is BLOCKING as it has to wait for enough Serial data to come in before it can finish a read from a float
	This is the shortest it can be with only microseconds of time between each check of the ESP8266_SERIAL
*/
#define ESP8266_ALIGN_BYTES 4
#define ESP8266_MSG_SIZE 12

#define MSG_TO_ESP8266_ALIGN_BYTES 4
#define MSG_TO_ESP8266_MSG_SIZE 8 //For sending back to the ESP8266
#define MSG_TO_ESP8266_TOTAL_SIZE (MSG_TO_ESP8266_ALIGN_BYTES + MSG_TO_ESP8266_MSG_SIZE)

#define ESP8266_START_BYTE 254

#define ESP8266_MIN_CMD_BYTE 192 //Minimum number to be sent to complete the packet
//Commands are defined as follows (anything above the minimum value up to 253)
#define ESP8266_CMD_NO_AXN ESP8266_MIN_CMD_BYTE
#define ESP8266_CMD_START_RECORDING 193
#define ESP8266_CMD_CONTINUE_RECORDING 194
#define ESP8266_CMD_STOP_RECORDING 195
#define ESP8266_CMD_START_PLAYBACK 196
#define ESP8266_CMD_CONTINUE_PLAYBACK 197
#define ESP8266_CMD_STOP_PLAYBACK 198

#define ESP8266_CMD_MPU6050_NO_DATA 205 //Sent to the ESP8266 from the teensy if there is no data
#define ESP8266_CMD_MPU6050_DATA 206 //Sent if there is data following
#define ESP8266_CMD_MPU6050_DATA_LOW_BATT	207 //Send if low battery
#define ESP8266_CMD_MPU6050_NO_DATA_LOW_BATT	208 //Send if low battery

#define ESP8266_CMD_VOICE_START 210
#define ESP8266_CMD_VOICE_START_LOW_BATT 211
#define ESP8266_CMD_VOICE_STOP 212
#define ESP8266_CMD_VOICE_STOP_LOW_BATT 213

class ESP8266Comms{
	public:

		ESP8266Comms();
		~ESP8266Comms();

		//The three error floats that could come from the ESP8266
		//These are used to convert an unsigned char to a float
		union {
		  char bytes[4];
		  float f;
		} TransAngleUnion;
		float RX_trans_angle;

		union {
		  char bytes[4];
		  float f;
		} TransErrorUnion;
		float RX_err_trans;

		union {
		  char bytes[4];
		  float f;
		} RotatErrorUnion;
		float RX_err_rot;

		//	Data to be sent to the ESP8266
		uint8_t msgToESP8266[MSG_TO_ESP8266_TOTAL_SIZE] = {ESP8266_START_BYTE, ESP8266_START_BYTE, ESP8266_START_BYTE, ESP8266_CMD_NO_AXN, 0,0,0,0, 0,0,0,0};

		/*

			FUNCTIONS

		*/

		//Sets all errors to zero
		void zeroErrorCalculations(); 


		#define COULD_NOT_ALIGN_START_BYTE 0x00
		// Returns a COULD_NOT_ALIGN_START_BTYE character to indicate it couldn't match the beginning
		// Or returns the character of the command the beginningChars with numCharsToMatch
		// Used to sync up the beginning 4 chars of the msg from the ESP8266
		// The float value is anything larger than 254 254 254 192 which is a 0xFEFEFEFEC0
		char synchronizeSerialBeginning();

		//Returns a true or false to indicate it received data properly or not
		int readFromESP8266();

		void setCommand(char cmd); //Sets the command within the pkt

		void sendMsgToESP8266(char cmd, uint8_t* teaPkt); //Sends the msgToESP8266 bytes to the ESP with or without data

		void sendMsgToESP8266(char cmd); //Sends the msgToESP8266 bytes to the ESP with or without data

		void copyMPU6050DataIntoMsg(uint8_t* teapotPkt); //Copies in the array of data to the teapotPkt
};



#endif
