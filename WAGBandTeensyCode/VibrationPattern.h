#ifndef VibrationPattern_h
#define VibrationPattern_h
/*

	This file contains all the things related to Matt's motor stuff. Byouful code 

*/

#include <Arduino.h>
#include "WAGBandCommon.h"

#define NUM_MOTORS 6
#define ALWAYS_MAX_OUTPUT true
#define MAX_ERR 1

class VibrationPattern{
public:

	/*
	 * variables
	 */
	// array calculations
	int pins[NUM_MOTORS] = {3,4,5,6,22,23}; // teensy PCB numbering for pins (Looking RIGHT to LEFT at the Molex connector from the Teensy)
	float motorLoc[NUM_MOTORS];
	float motorLocAdjTrans[NUM_MOTORS];
	float motorLocAdjRot[NUM_MOTORS];
	float transKernelOutput[NUM_MOTORS];
	float waveKernelOutput[NUM_MOTORS];
	int motorSpeeds[NUM_MOTORS];

	// rotation error signal control
	float waveAngle = 0;
	float stepSize = PI/80; // PI/30;
	int rotDirec = 0;

	// angle simulation variables: these values would be received over wifi. these values could probably be integers.
	float trans_angle = PI/6; // angle along which to translate to correct error
	float err_trans = 0; // translation error magnitude: from 0 to 1
	float err_rot = 1; // rotation error magnitude: from -1 to 1: sign indicates direction of rotation   (each is 4 bytes)


	/*
		
		FUNCTIONS

	*/

	VibrationPattern();
	~VibrationPattern();

	// Updates the trans_angle, err_trans and err_rot
	void updateErrors(float t_angle, float err_trns, float err_rotat);

	// sets up pins and zeros stuff out for model
	void setupMotorFunction(); 

	// Handles the logic to run the actual functions to the motors
	void performMotorCalculationsAndRunMotors();

	// returns: 0 if a<x<b or b<x<a; 1 if x>a and x>b; -1 if x<a and x<b
	int rngcmp(float x, float a, float b);

	// adjusts angles to be within pi radians of the reference angle ref
	void adjAng(float* angles, float* anglesAdj, int nAngles, float ref);

	// normal probability distribution function
	float normpdf(float x, float mu, float sd);

	// translation normal distribution kernel function
	void transKernelNormal(float* anglesAdj, int nAngles, float angle, float* output);

	// translation window kernel function
	void transKernelWindow(float* anglesAdj, int nAngles, float angle, float* output);

	// translation combined kernel function
	void transKernel(float* anglesAdj, int nAngles, float angle, float* output);

	// wave kernel function
	void waveKernel(float* anglesAdj, int nAngles, float angle, float* output);

	// assign motor speeds from kernel outputs
	void assignMotorSpeeds(float errTrans, float errRot, float* transKernelOutputs, float* rotKernelOutputs, int nAngles, int* motorSpeeds);

	// write out the motor speeds
	void writeMotorSpeeds(int* motorSpeeds, int nAngles);

	// assign zeros to array (int)
	void assignZeros(int* arr, int len);

	// assign zeros to array (float)
	void assignZeros(float* arr, int len);

	// linearly space values in given array
	// example: assignLinear(arr,5,0,1): {0,.2,.4,.6,.8}
	void assignLinear(float* arr, int len, float start, float finish);

};


#endif