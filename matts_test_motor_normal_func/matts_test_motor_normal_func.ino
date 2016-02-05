#define NUM_MOTORS 6
#define ALWAYS_MAX_OUTPUT true
#define MAX_ERR 1

/*
 * variables
 */
// array calculations
//int pins[NUM_MOTORS] = {4,5,6,9,10,11}; // ard
int pins[NUM_MOTORS] = {3,4,5,6,22,23};//23,6,5,4,3}; // teensy
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
float err_rot = 1; // rotation error magnitude: from -1 to 1: sign indicates direction of rotation

/* 
 * setup
 */
void setup() {
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

/*
 * loop
 */
void loop() {
  // potential program layout for bands? might take a bit more thought to get working right though
  /*
   * Run MPU stuff here, calculate new quaternion here
   */

  /*
   * Send new quaternion data over wifi here
   */

  // wait. but this needs to be replaced with an actual timer lol
  delay(9);
  
  /*
   * Receive new values of trans_angle, err_trans, and err_rot from wifi here and process
   */
  // end potential program layout for bands
  
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

/*
 * function definitions
 */
// returns: 0 if a<x<b or b<x<a; 1 if x>a and x>b; -1 if x<a and x<b
int rngcmp(float x, float a, float b){
  return (x>a) + (x>b) - 1;
}

// adjusts angles to be within pi radians of the reference angle ref
void adjAng(float* angles, float* anglesAdj, int nAngles, float ref){
  for (int i=0; i<nAngles; i++){
    anglesAdj[i] = angles[i] - rngcmp(angles[i]-ref,-PI,PI)*TWO_PI;
  }
}

// normal probability distribution function
float normpdf(float x, float mu, float sd){
  return exp(-((x-mu)*(x-mu))/(sd*sd*2))/(sd*sqrt(TWO_PI));
}

// translation normal distribution kernel function
void transKernelNormal(float* anglesAdj, int nAngles, float angle, float* output){  
  // normal kernel parameters
  const float STDEV = 0.5;
  const float SCALE = 1/normpdf(0,0,STDEV);
  
  // calculate the kernel function over each angle
  for (int i=0; i<nAngles; i++){
    output[i] = SCALE * normpdf(anglesAdj[i], angle, STDEV);
  }
}

// translation window kernel function
void transKernelWindow(float* anglesAdj, int nAngles, float angle, float* output){
  // window kernel parameters
  const float SCALE = 1;
  const float WIDTH = PI/3;

  // calculate kernel function over each angle
  for (int i=0; i<nAngles; i++){
    output[i] = SCALE * (abs(anglesAdj[i] - angle) < WIDTH);
  }
}

// translation combined kernel function
void transKernel(float* anglesAdj, int nAngles, float angle, float* output){
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
void waveKernel(float* anglesAdj, int nAngles, float angle, float* output){
  // normal kernel parameters
  const float STDEV = 0.4;
  const float SCALE = 1/normpdf(0,0,STDEV);
  
  // calculate the kernel function over each angle
  for (int i=0; i<nAngles; i++){
    output[i] = SCALE * normpdf(anglesAdj[i], angle, STDEV);
  }
}

// assign motor speeds from kernel outputs
void assignMotorSpeeds(float errTrans, float errRot, float* transKernelOutputs, float* rotKernelOutputs, int nAngles, int* motorSpeeds){
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
void writeMotorSpeeds(int* motorSpeeds, int nAngles){
  for (int i=0; i<nAngles; i++){
    analogWrite(pins[i], motorSpeeds[i]);
  }
}

// assign zeros to array (int)
void assignZeros(int* arr, int len){
  for (int i=0; i<len; i++){
    arr[i] = 0;
  }
}

// assign zeros to array (float)
void assignZeros(float* arr, int len){
  for (int i=0; i<len; i++){
    arr[i] = 0;
  }
}

// linearly space values in given array
// example: assignLinear(arr,5,0,1): {0,.2,.4,.6,.8}
void assignLinear(float* arr, int len, float start, float finish){
  float stp = (finish - start)/len;
  float val = start;
  for (int i=0; i<len; i++){
    arr[i] = val;
    val = val + stp;
  }
}













