#include "VibrationPattern.h"


VibrationPattern::VibrationPattern(){
    assignZeros(transKernelOutput,NUM_MOTORS);
  assignZeros(waveKernelOutput,NUM_MOTORS);
  assignZeros(motorSpeeds,NUM_MOTORS);
  assignLinear(motorLoc,NUM_MOTORS,-PI,PI);
  assignLinear(motorLocAdjTrans,NUM_MOTORS,-PI,PI);
  assignLinear(motorLocAdjRot,NUM_MOTORS,-PI,PI);
  
  for(int i=0; i<NUM_MOTORS; i++){
    pinMode(pins[i], OUTPUT);  
  }
  
  // sum of err and rot_err must be between 0 and 1
  float err_sum = err_trans + err_rot;
  if (err_sum > 1){
    err_trans /= err_sum;
    err_rot /= err_sum;
  }
}

VibrationPattern::~VibrationPattern(){
    //Blank
}



/*

    FUNCTIONS

*/

//sets up pins and zeros stuff out for model    
void VibrationPattern::setupMotorFunction(){
  assignZeros(transKernelOutput,NUM_MOTORS);
  assignZeros(waveKernelOutput,NUM_MOTORS);
  assignZeros(motorSpeeds,NUM_MOTORS);
  assignLinear(motorLoc,NUM_MOTORS,-PI,PI);
  assignLinear(motorLocAdjTrans,NUM_MOTORS,-PI,PI);
  assignLinear(motorLocAdjRot,NUM_MOTORS,-PI,PI);
  
  for(int i=0; i<NUM_MOTORS; i++){
    pinMode(pins[i], OUTPUT);  
  }
  
  // sum of err and rot_err must be between 0 and 1
  float err_sum = err_trans + err_rot;
  if (err_sum > 1){
    err_trans /= err_sum;
    err_rot /= err_sum;
  }
}

// Handles the logic to run the actual functions to the motors
void VibrationPattern::performMotorCalculationsAndRunMotors(){
  // set the rotation direction
  rotDirec = (err_rot > 0) - (err_rot < 0);
  if (err_rot < 0) err_rot = -err_rot;
  
  // adjust the motor angles to be within pi of the reference angles
  adjAng(motorLoc, motorLocAdjTrans, NUM_MOTORS, trans_angle);
  adjAng(motorLoc, motorLocAdjRot, NUM_MOTORS, waveAngle);
  
  // calculate the translation kernel function over the adjusted angles
  transKernel(motorLocAdjTrans, NUM_MOTORS, trans_angle, transKernelOutput);

  // calculate the wave kernel function (for rotation) over the adjusted angles
  waveKernel(motorLocAdjRot, NUM_MOTORS, waveAngle, waveKernelOutput);
  
  // calculate motor speeds from the kernel outputs
  assignMotorSpeeds(err_trans, err_rot, transKernelOutput, waveKernelOutput, NUM_MOTORS, motorSpeeds);
  
  // write the motor speeds to the motors
  writeMotorSpeeds(motorSpeeds, NUM_MOTORS);

  // update wave angle
  waveAngle += rotDirec*stepSize;
  adjAng(&waveAngle, &waveAngle, 1, 0);
}

// returns: 0 if a<x<b or b<x<a; 1 if x>a and x>b; -1 if x<a and x<b
int VibrationPattern::rngcmp(float x, float a, float b){
  return (x>a) + (x>b) - 1;
}

// adjusts angles to be within pi radians of the reference angle ref
void VibrationPattern::adjAng(float* angles, float* anglesAdj, int nAngles, float ref){
  for (int i=0; i<nAngles; i++){
    anglesAdj[i] = angles[i] - rngcmp(angles[i]-ref,-PI,PI)*TWO_PI;
  }
}

// normal probability distribution function
float VibrationPattern::normpdf(float x, float mu, float sd){
  return exp(-((x-mu)*(x-mu))/(sd*sd*2))/(sd*sqrt(TWO_PI));
}

// translation normal distribution kernel function
void VibrationPattern::transKernelNormal(float* anglesAdj, int nAngles, float angle, float* output){  
  // normal kernel parameters
  const float STDEV = 0.5;
  const float SCALE = 1/normpdf(0,0,STDEV);
  
  // calculate the kernel function over each angle
  for (int i=0; i<nAngles; i++){
    output[i] = SCALE * normpdf(anglesAdj[i], angle, STDEV);
  }
}

// translation window kernel function
void VibrationPattern::transKernelWindow(float* anglesAdj, int nAngles, float angle, float* output){
  // window kernel parameters
  const float SCALE = 1;
  const float WIDTH = PI/3;

  // calculate kernel function over each angle
  for (int i=0; i<nAngles; i++){
    output[i] = SCALE * (abs(anglesAdj[i] - angle) < WIDTH);
  }
}

// translation combined kernel function
void VibrationPattern::transKernel(float* anglesAdj, int nAngles, float angle, float* output){
  // allocate normal and window outputs
  float outWindow[nAngles];
  float outNormal[nAngles];
  
  // calculate normal and window outputs
  transKernelNormal(anglesAdj, nAngles, angle, outNormal);
  transKernelWindow(anglesAdj, nAngles, angle, outWindow);
  
  // calculate combined outputs
  for (int i=0; i<nAngles; i++){
    output[i] = outWindow[i] * outNormal[i];
  }
}

// wave kernel function
void VibrationPattern::waveKernel(float* anglesAdj, int nAngles, float angle, float* output){
  // normal kernel parameters
  const float STDEV = 0.4;
  const float SCALE = 1/normpdf(0,0,STDEV);
  
  // calculate the kernel function over each angle
  for (int i=0; i<nAngles; i++){
    output[i] = SCALE * normpdf(anglesAdj[i], angle, STDEV);
  }
}

// assign motor speeds from kernel outputs
void VibrationPattern::assignMotorSpeeds(float errTrans, float errRot, float* transKernelOutputs, float* rotKernelOutputs, int nAngles, int* motorSpeeds){
  float m_trans = 0;
  float m_wave = 0;
  if (ALWAYS_MAX_OUTPUT){
    for (int i=0; i<nAngles; i++){
      m_trans = max(m_trans,transKernelOutputs[i]);
      m_wave = max(m_wave,rotKernelOutputs[i]);
    }
  } else {
    m_trans = 1;
    m_wave = 1;
  }

  for (int i=0; i<nAngles; i++){
    float speedTrans = transKernelOutputs[i] * min(err_trans,MAX_ERR) / (m_trans*MAX_ERR);
    float speedRot = rotKernelOutputs[i] * min(err_rot,MAX_ERR) / (m_wave*MAX_ERR);
    motorSpeeds[i] = int(255*(speedTrans + speedRot));
  }
}

// write out the motor speeds
void VibrationPattern::writeMotorSpeeds(int* motorSpeeds, int nAngles){
  for (int i=0; i<nAngles; i++){
    analogWrite(pins[i], motorSpeeds[i]);
  }
}

// assign zeros to array (int)
void VibrationPattern::assignZeros(int* arr, int len){
  for (int i=0; i<len; i++){
    arr[i] = 0;
  }
}

// assign zeros to array (float)
void VibrationPattern::assignZeros(float* arr, int len){
  for (int i=0; i<len; i++){
    arr[i] = 0;
  }
}

// linearly space values in given array
// example: assignLinear(arr,5,0,1): {0,.2,.4,.6,.8}
void VibrationPattern::assignLinear(float* arr, int len, float start, float finish){
  float stp = (finish - start)/len;
  float val = start;
  for (int i=0; i<len; i++){
    arr[i] = val;
    val = val + stp;
  }
}