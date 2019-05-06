/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"

/*
 * Runs the user operator control code. This function will be started in its own task with the
 * default priority and stack size whenever the robot is enabled via the Field Management System
 * or the VEX Competition Switch in the operator control mode. If the robot is disabled or
 * communications is lost, the operator control task will be stopped by the kernel. Re-enabling
 * the robot will restart the task, not resume it from where it left off.
 *
 * If no VEX Competition Switch or Field Management system is plugged in, the VEX Cortex will
 * run the operator control task. Be warned that this will also occur if the VEX Cortex is
 * tethered directly to a computer via the USB A to A cable without any VEX Joystick attached.
 *
 * Code running in this task can take almost any action, as the VEX Joystick is available and
 * the scheduler is operational. However, proper use of delay() or taskDelayUntil() is highly
 * recommended to give other tasks (including system tasks such as updating LCDs) time to run.
 *
 * This task should never exit; it should end with some kind of infinite loop, even if empty.
 */
 
#define DRIVE_FL 2
#define DRIVE_FR 8
#define DRIVE_BL 3
#define DRIVE_BR 9

#define SHOULDER 1
#define ELBOW_1 7
#define ELBOW_2 5
#define WRIST 6
 
void operatorControl() {
	int x, y;
	int THRESHOLD = 20;
	bool isDriving = true;
	
	while (1) {
		if(isDriving) {
			if(abs(joystickGetAnalog(1, 1)) > THRESHOLD)
				y = joystickGetAnalog(1, 1) * 70 / 127;
			else
				y = 0;
			
			if(abs(joystickGetAnalog(1, 3)) > THRESHOLD)
				x = joystickGetAnalog(1, 3) * 70 / 127;
			else 
				x = 0;
		
			motorSet(DRIVE_FL, y + x);
			motorSet(DRIVE_FR, y - x);
			motorSet(DRIVE_BL, y + x);
			motorSet(DRIVE_BR, y - x);
		} else {
			if(abs(joystickGetAnalog(1, 3)) > THRESHOLD)
				motorSet(SHOULDER, joystickGetAnalog(1, 3) * 50 / 127);
			else
				motorStop(SHOULDER);
			
			if(abs(joystickGetAnalog(1, 2)) > THRESHOLD) {
				motorSet(ELBOW_1, joystickGetAnalog(1, 2) * 50 / 127);
				motorSet(ELBOW_2, -joystickGetAnalog(1, 2) * 50 / 127);
			} else {
				motorStop(ELBOW_1);
				motorStop(ELBOW_2);
			}
			
		}
		
		if(joystickGetDigital(1, 8, JOY_UP)) {
			isDriving = !isDriving;
			delay(500);
			motorStop(DRIVE_FL);
			motorStop(DRIVE_FR);
			motorStop(DRIVE_BL);
			motorStop(DRIVE_BR);
		}
		
		if(joystickGetDigital(1, 6, JOY_UP)) {
			motorSet(WRIST, 127);
		} else if(joystickGetDigital(1, 6, JOY_DOWN)) {
			motorSet(WRIST, -127);
		} else {
			motorSet(WRIST, 0);
		}
		
		delay(20);
	}
}
