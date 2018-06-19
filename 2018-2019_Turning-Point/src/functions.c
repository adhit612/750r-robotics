#include "main.h"
#include "globals.h"

void drive(int x, int y) {
  motorSet(D_MOTOR_1, y-x);
  motorSet(D_MOTOR_2, -(y+x));
  motorSet(D_MOTOR_3, -(y+x));
  motorSet(D_MOTOR_4, y-x);
}

void intake(int speed) {
    motorSet(INTAKE_MOTOR, speed);
}

void launch() {
  motorSet(LAUNCHER_MOTOR, 127);
  delay(2000);
  motorStop(LAUNCHER_MOTOR);
}

void auton() {
  //todo 
}
