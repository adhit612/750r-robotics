#include "main.h"
#include "robot.h"
#include <vector>

Robot::Robot(const int portFL, const int portBL, const int portFR, const int portBR) : driveMotors(), autonIndex(0) {
	driveMotors.push_back(pros::Motor(portFL));
	driveMotors.push_back(pros::Motor(portBL));
	driveMotors.push_back(pros::Motor(portFR));
	driveMotors.push_back(pros::Motor(portBR));
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

void Robot::setAutonIndex(int index) {
	autonIndex = index;
}