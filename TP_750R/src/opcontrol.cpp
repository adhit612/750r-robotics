#include "main.h"
#include "config.h"
#include "robot.h"
#include <vector>

#define PI 3.14159

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
 
void opcontrol() {
	pros::Controller primary = pros::Controller(pros::E_CONTROLLER_MASTER);
	pros::Controller auxiliary = pros::Controller(pros::E_CONTROLLER_PARTNER);
	
	Robot robot(D_MOTOR_FL, D_MOTOR_BL, D_MOTOR_FR, D_MOTOR_BR,
				INTAKE_PORT, FLYWHEEL_PORT, INDEXER_PORT, LIFT_PORT, GYRO_PORT);
	robot.setAutonIndex(autonIndex);
	
	robot.startFlywheel();
	robot.startFieldPosition();
	
	int flywheelCount = 0;
	
	while (true) {
		
		if(driveMutex.take((uint32_t)0xffffffffUL)) {
			robot.drive(primary.get_analog(ANALOG_RIGHT_X) * 200.0 / 127.0, primary.get_analog(ANALOG_LEFT_Y) * 200.0 / 127.0);
			driveMutex.give();
		}
		
		if(primary.get_digital(DIGITAL_L1)) {
			robot.intake(INTAKE_MODE_IN);
		} else if(primary.get_digital(DIGITAL_L2)) {
			robot.intake(INTAKE_MODE_OUT);
		} else {
			robot.intake(INTAKE_MODE_STOP);
		}
		
		if(primary.get_digital(DIGITAL_R1)) {
			robot.lift(LIFT_MODE_UP);
		} else if(primary.get_digital(DIGITAL_R2)) {
			robot.lift(LIFT_MODE_DOWN);
		} else {
			robot.lift(LIFT_MODE_HOLD);
		}
		
		if(primary.get_digital(DIGITAL_Y)) {
			flywheelCount++;
			if(flywheelCount == 10) {
				robot.startFlywheel();
				flywheelCount = 0;
				primary.print(0, 0, "START");
				pros::delay(50);
			}
		}
		
		robot.engageFlywheel(primary.get_digital(DIGITAL_A));
		
		if(primary.get_digital(DIGITAL_B)) {
			primary.print(0, 0, "X: %d", robot.getX());
			pros::delay(50);
			primary.print(1, 0, "Y: %d", robot.getY());
			pros::delay(50);
			primary.print(2, 0, "Angle: %d", robot.getAngle());
			pros::delay(50);
		}
		
		pros::delay(20);
	}
}
