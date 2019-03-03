#include "main.h"
#include "robot.h"
#include <vector>

Robot::Robot(const int portFL, const int portBL, const int portFR, const int portBR,
			 const int intakePort, const int flywheelPort, const int indexerPort,
			 const int liftPort) : driveMotors(), roller(intakePort),
			 flywheel(flywheelPort), indexer(indexerPort), capLift(liftPort), autonIndex(0) {
	driveMotors.push_back(pros::Motor(portFL));
	driveMotors.push_back(pros::Motor(portBL));
	driveMotors.push_back(pros::Motor(portFR));
	driveMotors.push_back(pros::Motor(portBR));
}

void Robot::setAutonIndex(int index) {
	autonIndex = index;
}

void Robot::drive(int x, int y) {
	for(int i = 0; i < driveMotors.size(); i++) {
		pros::Motor m = driveMotors.at(i);
		if(i % 2 == 0) {
			m.move_velocity(y + x);
		} else {
			m.move_velocity(y - x);
		}
	}
}

void Robot::intake(int mode) {
	roller.move_velocity(200 * mode);
}

void Robot::lift(int power) {
	capLift.move_velocity(power);
}