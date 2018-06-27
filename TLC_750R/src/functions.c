#include "main.h"
#include "globals.h"

void drive(int x, int y) {
  motorSet(D_MOTOR_1, x-y);
  motorSet(D_MOTOR_2, -(x+y));
  motorSet(D_MOTOR_3, -(x+y));
  motorSet(D_MOTOR_4, x-y);
}

void launch(int in) {
  motorSet(LAUNCHER_MOTOR, in);
  motorSet(LAUNCHER_MOTOR2, in);
}

void intake(int in) {
  motorSet(INTAKE_MOTOR, in);
}

void carry(int in) {
  motorSet(CARRIER_MOTOR, in);
}

void auton() {
  intake(-127);
  drive(127, 0);
  delay(2000);
  drive(0, 0);
  intake(0);
  delay(500);
  carry(25);
  delay(3000);
  carry(-25);
  delay(3000);
  drive(-127, 0);
  delay(2000);
  drive(0, 0);
  delay(250);
  launch(127);
  delay(1000);
  launch(0);
}
