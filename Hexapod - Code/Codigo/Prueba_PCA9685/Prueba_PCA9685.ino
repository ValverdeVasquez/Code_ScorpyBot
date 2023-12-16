#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  120
#define SERVOMAX  470
#define SERVO_FREQ 50

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

int servonum = 0; // De 0 a 15
int posizione;
int angulo;

void setup() {
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  delay(10);
}

void loop() {
  angulo = 0;
  posizione = map(angulo, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servonum, 0, posizione);
  delay(2000);

  angulo = 180;
  posizione = map(angulo, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servonum, 0, posizione);
  delay(2000);

  angulo = 0;
  posizione = map(angulo, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servonum, 0, posizione);
  delay(2000);

  angulo = 180;
  posizione = map(angulo, 0, 180, SERVOMIN, SERVOMAX);
  pwm.setPWM(servonum, 0, posizione);
  delay(2000);
}
