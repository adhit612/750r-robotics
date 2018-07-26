#include "main.h"
#include "globals.h"

void drive(int driving) {
	motorSet(DRIVE_MOTOR, driving);
}

void steer(int steering) {
	motorSet(STEERING_MOTOR, steering);	
}