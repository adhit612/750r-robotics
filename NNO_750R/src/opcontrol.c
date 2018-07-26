/** @file opcontrol.c
 * @brief File for operator control code
 *
 * This file should contain the user operatorControl() function and any functions related to it.
 *
 * Any copyright is dedicated to the Public Domain.
 * http://creativecommons.org/publicdomain/zero/1.0/
 *
 * PROS contains FreeRTOS (http://www.freertos.org) whose source code may be
 * obtained from http://sourceforge.net/projects/freertos/files/ or on request.
 */

#include "main.h"
#include "globals.h"

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
void operatorControl() {
	int driving, steering;
	int LIMITER = 20;
	
	while (true) {
		
		//Test axis 3 (left stick, up-down) for forward/backward input
		if(abs(joystickGetAnalog(JOYSTICK, DRIVE_AXIS)) >= LIMITER) {
			driving = joystickGetAnalog(JOYSTICK, DRIVE_AXIS);
		} else {
			driving = 0;
		}
		
		//Test axis 1 (right stick, left-right) for left/right steering input
		if(abs(joystickGetAnalog(JOYSTICK, STEER_AXIS)) >= LIMITER) {
			steering = joystickGetAnalog(JOYSTICK, STEER_AXIS);
		} else {
			steering = 0;
		}
		
		//Set motors with inputs
		drive(driving);
		steer(steering);
		
		//Idling auton
		if(joystickGetDigital(JOYSTICK, 7, JOY_UP)) {
			drive(0);
			steer(0);
			delay(1000);
			
			//Drives forwards and backwards until auto button pressed again
			while(!joystickGetDigital(JOYSTICK, 7, JOY_UP)) {
				drive(127);
				delay(1000);
				drive(0);
				delay(200);
				drive(-127);
				delay(1000);
				drive(0);
				delay(200);
			}
		}
	}
}
