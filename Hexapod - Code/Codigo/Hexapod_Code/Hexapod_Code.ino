#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>
#include <Adafruit_PWMServoDriver.h>
#include <cstdlib>

#include "vectors.h"
#include "RC.h"

#include "Initializations.h"
#include "IK_Arms.h"
#include "Helpers.h"

#include "FireBase.h"
#include <WiFi.h>
#include <FirebaseESP32.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

enum State {
  Initialize,
  Stand,
  Car,
  Crab,
  Calibrate,
  SlamAttack
};

enum LegState {
  Propelling,
  Lifting,
  Standing,
  Reset
};

enum Gait {
  Tri,
  Wave,
  Ripple,
  Bi,
  Hop  
};


int WebMando;


int totalGaits = 5;
Gait gaits[5] = {Tri,Wave,Ripple,Bi,Hop};


float points = 1000;
int cycleProgress[6];
LegState legStates[6];
int standProgress = 0;

State currentState = Initialize;
Gait currentGait = Tri;
Gait previousGait = Tri;
int currentGaitID = 0;

float standingDistanceAdjustment = 0;

float distanceFromGroundBase = -60;
float distanceFromGround = 0; 
float previousDistanceFromGround = 0;

float liftHeight = 30;
float landHeight = 0;
float strideOvershoot = 10;
float distanceFromCenter = 50;

float crabTargetForwardAmount = 0;
float crabForwardAmount = 0;

Vector2 joy1TargetVector = Vector2(0,0);
float joy1TargetMagnitude = 0;

Vector2 joy1CurrentVector = Vector2(0,0);
float joy1CurrentMagnitude = 0;

Vector2 joy2TargetVector = Vector2(0,0);
float joy2TargetMagnitude = 0;

Vector2 joy2CurrentVector = Vector2(0,0);
float joy2CurrentMagnitude = 0;

unsigned long timeSinceLastInput = 0;

float landingBuffer = 15;

int attackCooldown = 0;
long elapsedTime = 0;
long loopStartTime = 0;



void setup() {
  //PCA9865 #01
  pwm.begin();  // Initialize the PWM driver
  pwm2.begin();  // Initialize the PWM driver

  pwm.setPWMFreq(60);  // Set the PWM frequency (60Hz is typical)
  pwm2.setPWMFreq(60);  // Set the PWM frequency (60Hz is typical)

  pwm.setOscillatorFrequency(27000000);
  pwm2.setOscillatorFrequency(27000000);

  // Initialize serial communication
  Serial.begin(115200);
  attachServos(); 
  RC_Setup(); 
  //FireBase_Setup();
  stateInitialize();
}

void loop() {

  int WebMando = rc_data.joy2_Button;

  int Rotate1 = rc_data.joy3_Button;
  int Rotate2 = rc_data.joy4_Button;

  int Arms_Gripper = 1;

  segmento1.write(15);
  segmento2.write(45);
  segmento3.write(50);
  segmento4.write(50);
  segmento5.write(90);

  if (Arms_Gripper == 1){

    double joy3x = map(rc_data.joy3_X,0,254,-100,100);
    double joy3y = map(rc_data.joy3_Y,0,254,-100,100);

    double joy4x = map(rc_data.joy4_X,0,254,-100,100);
    double joy4y = map(rc_data.joy4_Y,0,254,-100,100);


//Joystick N°03
    if (joy3x > 30){
      ejeY = ejeY + 1;
    }

    if (joy3x < -30){
      ejeY = ejeY - 1;
    }

    if (joy3y > 30 && ejeX <= 295){
      ejeX = ejeX + 1;
    }

    if (joy3y < -30 && ejeX >= 230){
      ejeX = ejeX - 1;
    }

//Joystick N°04
      if (joy4y > 30){
      ejeZ = ejeZ + 1;
    }

    if (joy4y < -30){
      ejeZ = ejeZ - 1;
    }

    if (joy4x > 30 && Gripper <= 155){
      Gripper = Gripper + 1;
    }

    if (joy4x < -30 && Gripper >= 80){
      Gripper = Gripper - 1;
    }


    if (Rotate1 == 1 && Rotate <= 170){
      Rotate = Rotate + 1;
    }

    if (Rotate2 == 1 && Rotate >= 0){
      Rotate = Rotate - 1;
    }



    IK_Arms();
    IkArmsBase_Final_Angle();
    LeftArm();
    RightArm();
    
    //Print_IK_Arms();
  }

  if (WebMando == 0){

    void attachServos();

    elapsedTime = millis() - loopStartTime;
    loopStartTime = millis();

    
    bool connected = RC_GetDataPackage();
    
    RC_DisplayData();

    if(connected){
      double joy1x = map(rc_data.joy1_X,0,254,-100,100);
      double joy1y = map(rc_data.joy1_Y,0,254,-100,100);

      double joy2x = map(rc_data.joy2_X,0,254,-100,100);
      double joy2y = map(rc_data.joy2_Y,0,254,-100,100);
      
      joy1TargetVector = Vector2(joy1x,joy1y);
      joy1TargetMagnitude = constrain(calculateHypotenuse(abs(joy1x),abs(joy1y)),0,100);   

      joy2TargetVector = Vector2(joy2x,joy2y);
      joy2TargetMagnitude = constrain(calculateHypotenuse(abs(joy2x),abs(joy2y)),0,100);  

      previousDistanceFromGround = distanceFromGround;
      distanceFromGround = distanceFromGroundBase + rc_data.slider1 * -1.7;
      distanceFromCenter = 60;
    }
    else{
      //calibrationState();
      //Serial.println("State: Disconnected");
      return;
    }

    joy1CurrentVector = lerp(joy1CurrentVector, joy1TargetVector, 0.08);
    joy1CurrentMagnitude = lerp(joy1CurrentMagnitude, joy1TargetMagnitude, 0.08);

    joy2CurrentVector = lerp(joy2CurrentVector, joy2TargetVector, 0.12);
    joy2CurrentMagnitude = lerp(joy2CurrentMagnitude, joy2TargetMagnitude, 0.12);  

    previousGait = currentGait;
    if(rc_data.pushButton2 == 1  && rc_data_previous.pushButton2 == 0){
      currentGaitID += 1;
      if(currentGaitID == totalGaits){
        currentGaitID = 0;
      }    
      
      currentGait = gaits[currentGaitID];
    }

    if(rc_data.joy1_Button == 1 && attackCooldown == 0){
      Serial.println("slam attack");
      resetMovementVectors();
      slamAttack();
      standingState();
      attackCooldown = 50;    
      loopStartTime = millis();
      return;
    }
    
    else{
      attackCooldown = max(attackCooldown - elapsedTime, static_cast<long>(0));
    }

    if(abs(joy1CurrentMagnitude) >= 10 || abs(joy2CurrentMagnitude) >= 10){
      carState();
      timeSinceLastInput = millis();
      return;
    }

    if(labs(timeSinceLastInput - millis()) > 5) {
      standingState();
      return;
    }
  }

  if (WebMando == 1){

    FireBase_Loop();
    FireBase_Final_Angle();
    //FireBase_Print();
    Serial.println(String(MicroSeg_LfAlpha));

    pwm.writeMicroseconds(coxa1Pin, MicroSeg_LfAlpha);
    pwm.writeMicroseconds(femur1Pin, MicroSeg_LfBeta);
    pwm.writeMicroseconds(tibia1Pin, MicroSeg_LfGamma);

    pwm.writeMicroseconds(coxa2Pin, MicroSeg_LmAlpha);
    pwm.writeMicroseconds(femur2Pin, MicroSeg_LmBeta);
    pwm.writeMicroseconds(tibia2Pin, MicroSeg_LmGamma);

    pwm.writeMicroseconds(coxa3Pin, MicroSeg_LbAlpha);
    pwm.writeMicroseconds(femur3Pin, MicroSeg_LbBeta);
    pwm.writeMicroseconds(tibia3Pin, MicroSeg_LbGamma);

    pwm2.writeMicroseconds(coxa4Pin, MicroSeg_RbAlpha);
    pwm2.writeMicroseconds(femur4Pin, MicroSeg_RbBeta);
    pwm2.writeMicroseconds(tibia4Pin, MicroSeg_RbGamma);

    pwm2.writeMicroseconds(coxa5Pin, MicroSeg_RmAlpha);
    pwm2.writeMicroseconds(femur5Pin, MicroSeg_RmBeta);
    pwm2.writeMicroseconds(tibia5Pin, MicroSeg_RmGamma);

    pwm2.writeMicroseconds(coxa6Pin, MicroSeg_RfAlpha);
    pwm2.writeMicroseconds(femur6Pin, MicroSeg_RfBeta);
    pwm2.writeMicroseconds(tibia6Pin, MicroSeg_RfGamma);

    return;
  }
}


void resetMovementVectors(){
  joy1CurrentVector = Vector2(0,0);
  joy1CurrentMagnitude = 0;

  joy2CurrentVector = Vector2(0,0);
  joy2CurrentMagnitude = 0;
}

void setCycleStartPoints(int leg){
  cycleStartPoints[leg] = currentPoints[leg];    
}

void setCycleStartPoints(){
  for(int i = 0; i < 6; i++){
    cycleStartPoints[i] = currentPoints[i]; 
  }     
}

int angleToMicroseconds(double angle) {
  double val = 500.0 + (((2500.0 - 500.0) / 180.0) * angle);
  return (int)val;
}

void moveToPos(int leg, Vector3 pos){
  currentPoints[leg] = pos;
  
  float dis = Vector3(0,0,0).distanceTo(pos);
  if(dis > legLength){
    print_value("Point impossible to reach", pos, false);
    print_value("Distance",dis, true);
    return;
  }

  float x = pos.x;
  float y = pos.y;
  float z = pos.z;

  float o1 = offsets[leg].x;
  float o2 = offsets[leg].y;
  float o3 = offsets[leg].z;

  float theta1 = atan2(y,x) * (180 / PI) + o1; // base angle
  float l = sqrt(x*x + y*y); // x and y extension 
  float l1 = l - a1;
  float h = sqrt(l1*l1 + z*z);

  float phi1 = acos(constrain((pow(h,2) + pow(a2,2) - pow(a3,2)) / (2*h*a2),-1,1));
  float phi2 = atan2(z, l1);
  float theta2 = (phi1 + phi2) * 180 / PI + o2;
  float phi3 = acos(constrain((pow(a2,2) + pow(a3,2) - pow(h,2)) / (2*a2*a3),-1,1));
  float theta3 = 180 - (phi3 * 180 / PI) + o3;

  targetRot = Vector3(theta1,theta2,theta3);
  
  int coxaMicroseconds = angleToMicroseconds(targetRot.x);
  int femurMicroseconds = angleToMicroseconds(targetRot.y);
  int tibiaMicroseconds = angleToMicroseconds(targetRot.z);

  if (WebMando == 0){
    switch(leg){
      case 0:
        pwm.writeMicroseconds(coxa1Pin, coxaMicroseconds);
        pwm.writeMicroseconds(femur1Pin, femurMicroseconds);
        pwm.writeMicroseconds(tibia1Pin, tibiaMicroseconds);
        break;

      case 1:
        pwm.writeMicroseconds(coxa2Pin, coxaMicroseconds);
        pwm.writeMicroseconds(femur2Pin, femurMicroseconds);
        pwm.writeMicroseconds(tibia2Pin, tibiaMicroseconds);
        break;

      case 2:
        pwm.writeMicroseconds(coxa3Pin, coxaMicroseconds);
        pwm.writeMicroseconds(femur3Pin, femurMicroseconds);
        pwm.writeMicroseconds(tibia3Pin, tibiaMicroseconds);
        break;

      case 3:
        pwm2.writeMicroseconds(coxa4Pin, coxaMicroseconds);
        pwm2.writeMicroseconds(femur4Pin, femurMicroseconds);
        pwm2.writeMicroseconds(tibia4Pin, tibiaMicroseconds);
        break;

      case 4:
        pwm2.writeMicroseconds(coxa5Pin, coxaMicroseconds);
        pwm2.writeMicroseconds(femur5Pin, femurMicroseconds);
        pwm2.writeMicroseconds(tibia5Pin, tibiaMicroseconds);
        break;

      case 5:
        pwm2.writeMicroseconds(coxa6Pin, coxaMicroseconds);
        pwm2.writeMicroseconds(femur6Pin, femurMicroseconds);
        pwm2.writeMicroseconds(tibia6Pin, tibiaMicroseconds);
        break;

      default:
        break;
    }
    return;
  }
}






