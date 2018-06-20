#include "main.h"
#include "globals.h"

//First param is rotation
//Second param is forwards/backwards
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
  intake(127);
  drive(0, 127);
  delay(1000);
  drive(0, 0);
  delay(250);
  intake(0);
  drive(0, -127);
  delay(1000);
  drive(0, 0);
  delay(250);
}
