#include "vex.h"
#include "robot.h"
#include "config.h"
#include <vector>

Robot::Robot(vex::brain cortex) : cortex(cortex), driveMotors() {
	driveMotors.push_back(vex::motor(D_MOTOR_FL, false));
	driveMotors.push_back(vex::motor(D_MOTOR_FR, true));
	driveMotors.push_back(vex::motor(D_MOTOR_BL, false));
	driveMotors.push_back(vex::motor(D_MOTOR_BR, true));
}

void Robot::drive(int value) {
  
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
		m.spin(vex::directionType::fwd, value, vex::percentUnits::pct);
	}
}

void Robot::turnRight() {
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
    if(i%2==0)
			m.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
    else
      m.spin(vex::directionType::fwd, -100, vex::percentUnits::pct);
	}
}

void Robot::turnLeft() {
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
    if(i%2==0)
			m.spin(vex::directionType::fwd, -100, vex::percentUnits::pct);
    else
      m.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
	}
}

void Robot::strafeRight(int value)
{
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
		if(i==0) {
			m.spin(vex::directionType::fwd, value, vex::percentUnits::pct);
		} 
    if(i==1) {
			m.spin(vex::directionType::rev, value, vex::percentUnits::pct);
		} 
    if(i==2) {
			m.spin(vex::directionType::rev, value, vex::percentUnits::pct);
		} 
    if(i==3) {
			m.spin(vex::directionType::fwd, value, vex::percentUnits::pct);
		} 
	} 
}

void Robot::strafeLeft(int value)
{
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
		if(i==0) {
			m.spin(vex::directionType::rev, value, vex::percentUnits::pct);
		} 
    if(i==1) {
			m.spin(vex::directionType::fwd, value, vex::percentUnits::pct);
		} 
    if(i==2) {
			m.spin(vex::directionType::fwd, value, vex::percentUnits::pct);
		} 
    if(i==3) {
			m.spin(vex::directionType::rev, value, vex::percentUnits::pct);
		} 
	} 
}