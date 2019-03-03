#include "main.h"
#include "config.h"
#include "robot.h"
#include <vector>

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
				INTAKE_PORT, FLYWHEEL_PORT, INDEXER_PORT, LIFT_PORT);
	robot.setAutonIndex(autonIndex);
	
	while (true) {	
		
		robot.drive(primary.get_analog(ANALOG_RIGHT_X), primary.get_analog(ANALOG_LEFT_Y));
		
		if(primary.get_digital(DIGITAL_L1)) {
			robot.intake(INTAKE_MODE_IN);
		} else if(primary.get_digital(DIGITAL_L2)) {
			robot.intake(INTAKE_MODE_OUT);
		} else {
			robot.intake(INTAKE_MODE_STOP);
		}
		
		robot.lift(auxiliary.get_analog(ANALOG_RIGHT_Y));
		
		pros::delay(20);
	}
}
