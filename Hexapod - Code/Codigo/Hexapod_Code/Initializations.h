#include <Adafruit_PWMServoDriver.h>

// Create an instance of the Adafruit_PWMServoDriver class
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x41);

const int coxa1Pin = 5;
const int femur1Pin = 6;
const int tibia1Pin = 7;

const int coxa2Pin = 8;
const int femur2Pin = 9;
const int tibia2Pin = 10;

const int coxa3Pin = 11;
const int femur3Pin = 12;
const int tibia3Pin = 13;

const int coxa4Pin = 0;
const int femur4Pin = 1;
const int tibia4Pin = 2;

const int coxa5Pin = 3;
const int femur5Pin = 4;
const int tibia5Pin = 5;

const int coxa6Pin = 6;
const int femur6Pin = 7;
const int tibia6Pin = 8;

const int pinzacoxa1Pin = 4;
const int pinzafemur1Pin = 3;
const int pinzatibia1Pin = 2;
const int pinzarotate1Pin = 1;
const int pinzagripper1Pin = 0;

const int pinzacoxa2Pin = 11;
const int pinzafemur2Pin = 12;
const int pinzatibia2Pin = 13;
const int pinzarotate2Pin = 14;
const int pinzagripper2Pin = 15;

//

Servo coxacola;
Servo segmento1;
Servo segmento2;
Servo segmento3;
Servo segmento4;
Servo segmento5;

//

const int coxacolaPin = 1;
const int segmento1Pin = 13;
const int segmento2Pin = 12;
const int segmento3Pin = 14;
const int segmento4Pin = 27;
const int segmento5Pin = 26;

const Vector3 offsets1 = {100,185,-20};
const Vector3 offsets2 = {100,185,-20};
const Vector3 offsets3 = {100,185,-20};
const Vector3 offsets4 = {100,185,-20};
const Vector3 offsets5 = {100,185,-20};
const Vector3 offsets6 = {100,185,-20};
const Vector3 offsets[6] = {offsets1, offsets2, offsets3, offsets4, offsets5, offsets6};


const float a1 = 53;  //Coxa Length
const float a2 = 80; //Femur Length
const float a3 = 144; //Tibia Length   
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

  pwm.setPWM(coxa1Pin, 500, 2500); 
  pwm.setPWM(femur1Pin, 500, 2500); 
  pwm.setPWM(tibia1Pin, 500, 2500);

  pwm.setPWM(coxa2Pin, 500, 2500);  
  pwm.setPWM(femur2Pin, 500, 2500); 
  pwm.setPWM(tibia2Pin, 500, 2500); 

  pwm.setPWM(coxa3Pin, 500, 2500); 
  pwm.setPWM(femur3Pin, 500, 2500); 
  pwm.setPWM(tibia3Pin, 500, 2500);

  pwm2.setPWM(coxa4Pin, 500, 2500);  
  pwm2.setPWM(femur4Pin, 500, 2500); 
  pwm2.setPWM(tibia4Pin, 500, 2500); 

  pwm2.setPWM(coxa5Pin, 500, 2500); 
  pwm2.setPWM(femur5Pin, 500, 2500); 
  pwm2.setPWM(tibia5Pin, 500, 2500);

  pwm2.setPWM(coxa6Pin, 500, 2500);  
  pwm2.setPWM(femur6Pin, 500, 2500); 
  pwm2.setPWM(tibia6Pin, 500, 2500);

  coxacola.attach(coxacolaPin, 500, 2500);
  segmento1.attach(segmento1Pin, 500, 2500);
  segmento2.attach(segmento2Pin, 500, 2500);
  segmento3.attach(segmento3Pin, 500, 2500);
  segmento4.attach(segmento4Pin, 500, 2500);
  segmento5.attach(segmento5Pin, 500, 2500);
  

}