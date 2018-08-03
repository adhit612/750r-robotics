#include "main.h"
#include "globals.h"

void drive(int driving) {
	motorSet(DRIVE_MOTOR, driving);
}

void steer(double steering) {
	motorSet(STEERING_MOTOR, steering);
}