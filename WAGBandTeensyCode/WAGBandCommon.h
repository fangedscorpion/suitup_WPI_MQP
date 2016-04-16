#ifndef WAGBandCommon_h
#define WAGBandCommon_h

/*

	Anything that should be shared across header files!

*/


#define DEBUG_SERIAL Serial
#define ESP8266_SERIAL Serial3


//This defines the band numbers based on the BAND IP numbers they receive from the router
// but also identifies the bands for the calibration of the accel/gyro
#define RIGHT_SHOULDER_BAND_NUM 205
#define RIGHT_UPPER_ARM_BAND_NUM 207 //Bicep
#define RIGHT_WRIST_BAND_NUM 209

#define LEFT_SHOULDER_BAND_NUM 222
#define LEFT_UPPER_ARM_BAND_NUM 224 //Bicep
#define LEFT_WRIST_BAND_NUM 226

#define CHEST_PIECE_BAND_NUM 200

#endif
