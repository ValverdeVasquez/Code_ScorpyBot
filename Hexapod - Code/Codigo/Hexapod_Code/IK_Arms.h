double ejeX = 240;
double ejeY = 0;
double ejeZ = 100;

double Rotate = 90;
double Gripper = 90;

double desfaceZ = 70;

double gamna, alfa, Theta2, hipoXY, lineaAlfa,Phi1,Phi2,Phi3,Phi4;

const float COXA = 53;  //Coxa Length
const float FEMUR = 80; //Femur Length
const float TIBIA = 185; //Tibia Length

int MicroSeg_gamna;
int MicroSeg_Phi1;
int MicroSeg_Phi3;
int MicroSeg_Rotate;
int MicroSeg_Gripper;

int OffsetCoxi = 50;

void Print_IK_Arms(){
  Serial.print(ejeX);
  Serial.print("     ");
  Serial.print(ejeY);
  Serial.print("     ");
  Serial.print(ejeZ);
  Serial.print("     ");
  Serial.print(Gripper);
  Serial.print("     ");
  Serial.print(Rotate);
  Serial.print("     ");
  Serial.println(Phi3);
  Serial.print("     ");
  
}

void IK_Arms() {

  hipoXY = sqrt(pow(ejeX,2) + pow(ejeY,2));

  lineaAlfa = sqrt(  pow(desfaceZ-ejeZ,2) + pow(hipoXY - COXA,2) );

  gamna = atan(ejeY/ejeX) * RAD_TO_DEG;

  Phi2 = acos((desfaceZ-ejeZ)/lineaAlfa) * RAD_TO_DEG;
  Phi1 = acos(  (pow(TIBIA,2)-pow(FEMUR,2)-pow(lineaAlfa,2))   /   (-2*FEMUR*lineaAlfa)  ) * RAD_TO_DEG;
  Theta2 = Phi1 + Phi2;

  Phi3 = acos(  (pow(lineaAlfa,2)-pow(TIBIA,2)-pow(FEMUR,2)  ) / (-2*TIBIA*FEMUR)   ) * RAD_TO_DEG;

}

int ArmsAngleToMicroseconds(double angle) {
    double val = 500.0 + (((2500.0 - 500.0) / 180.0) * angle);
    return (int)val;
  }

void IkArmsBase_Final_Angle() {
  MicroSeg_gamna = ArmsAngleToMicroseconds(gamna + OffsetCoxi);
  MicroSeg_Phi1 = ArmsAngleToMicroseconds(Theta2);
  MicroSeg_Phi3 = ArmsAngleToMicroseconds(Phi3);
  MicroSeg_Rotate = ArmsAngleToMicroseconds(Rotate);
  MicroSeg_Gripper = ArmsAngleToMicroseconds(Gripper);
}

void RightArm(){
  pwm.writeMicroseconds(pinzacoxa1Pin, MicroSeg_gamna);
  pwm.writeMicroseconds(pinzafemur1Pin, MicroSeg_Phi1);
  pwm.writeMicroseconds(pinzatibia1Pin, MicroSeg_Phi3);
  pwm.writeMicroseconds(pinzarotate1Pin, MicroSeg_Rotate);
  pwm.writeMicroseconds(pinzagripper1Pin, MicroSeg_Gripper);
}

void LeftArm(){
  pwm2.writeMicroseconds(pinzacoxa2Pin, MicroSeg_gamna);
  pwm2.writeMicroseconds(pinzafemur2Pin, MicroSeg_Phi1);
  pwm2.writeMicroseconds(pinzatibia2Pin, MicroSeg_Phi3);
  pwm2.writeMicroseconds(pinzarotate2Pin, MicroSeg_Rotate);
  pwm2.writeMicroseconds(pinzagripper2Pin, MicroSeg_Gripper);
}

void ResetArms(){
  ejeX = 240;
  ejeY = 0;
  ejeZ = 100;
  Rotate = 90;
  Gripper = 90;
}

