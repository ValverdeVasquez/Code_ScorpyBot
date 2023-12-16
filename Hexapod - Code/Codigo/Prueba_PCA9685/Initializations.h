#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm0;

const int coxa1Pin = 0;
const int femur1Pin = 1;
const int tibia1Pin = 2;

const int coxa2Pin = 3;
const int femur2Pin = 4;
const int tibia2Pin = 5;

const int coxa3Pin = 6;
const int femur3Pin = 7;
const int tibia3Pin = 8;

const int coxa4Pin = 9;
const int femur4Pin = 10;
const int tibia4Pin = 11;

const int coxa5Pin = 12;
const int femur5Pin = 13;
const int tibia5Pin = 14;

const int coxa6Pin = 15;
const int femur6Pin = 16;
const int tibia6Pin = 17;

const Vector3 offsets1 = {90,75,-18};
const Vector3 offsets2 = {93,75,-15};
const Vector3 offsets3 = {93,75,-18}; 
const Vector3 offsets4 = {87,80,-26};
const Vector3 offsets5 = {85,89,-16};
const Vector3 offsets6 = {93,85,-24};
const Vector3 offsets[6] = {offsets1, offsets2, offsets3, offsets4, offsets5, offsets6};


const float a1 = 41;  //Coxa Length
const float a2 = 116; //Femur Length
const float a3 = 183; //Tibia Length   
float legLength = a1+a2+a3;

Vector3 currentPoints[6];
Vector3 cycleStartPoints[6];

Vector3 currentRot(180, 0, 180);
Vector3 targetRot(180, 0, 180);

float strideMultiplier[6] = {1, 1, 1, -1, -1, -1};
float rotationMultiplier[6] = {-1, 0, 1, -1, 0 , 1};

Vector3 ControlPoints[10];
Vector3 RotateControlPoints[10];

Vector3 AttackControlPoints[10];


void attachServos(){
  pwm0.setPWM(coxa1Pin, 500, 2500);  // Initialize servos to their neutral positions
  pwm0.setPWM(femur1Pin, 500, 2500);
  pwm0.setPWM(tibia1Pin, 500, 2500);

  pwm0.setPWM(coxa2Pin, 500, 2500);
  pwm0.setPWM(femur2Pin, 500, 2500);
  pwm0.setPWM(tibia2Pin, 500, 2500);

  pwm0.setPWM(coxa3Pin, 500, 2500);
  pwm0.setPWM(femur3Pin, 500, 2500);
  pwm0.setPWM(tibia3Pin, 500, 2500);

  pwm0.setPWM(coxa4Pin, 500, 2500);
  pwm0.setPWM(femur4Pin, 500, 2500);
  pwm0.setPWM(tibia4Pin, 500, 2500);

  pwm0.setPWM(coxa5Pin, 500, 2500);
  pwm0.setPWM(femur5Pin, 500, 2500);
  pwm0.setPWM(tibia5Pin, 500, 2500);

  pwm0.setPWM(coxa6Pin, 500, 2500);
  pwm0.setPWM(femur6Pin, 500, 2500);
  pwm0.setPWM(tibia6Pin, 500, 2500);
}