#include "vex.h"
#include "robot.h"
#include "config.h"
#include <vector>

Robot::Robot(vex::brain cortex) : cortex(cortex), driveMotors() {
	driveMotors.push_back(vex::motor(D_MOTOR_L, false));
	driveMotors.push_back(vex::motor(D_MOTOR_R, true));
}

void Robot::drive(int value) {
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
		m.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
	}
}

void Robot::turnRight() {
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
    if(i%2==0) 
			m.spin(vex::directionType::fwd, 50, vex::percentUnits::pct); //LEFT SIDE
    else
      m.spin(vex::directionType::rev, 50, vex::percentUnits::pct); //RIGHT SIDE
	}
}

void Robot::turnLeft() {
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
    if(i%2==0)
			m.spin(vex::directionType::rev, 50, vex::percentUnits::pct); //LEFT SIDE
    else
      m.spin(vex::directionType::fwd, 50, vex::percentUnits::pct); //RIGHT SIDE
	}
}

void Robot::strafe(int mode)
{
		vex::motor m = vex::motor(D_MOTOR_M);
		m.spin(vex::directionType::fwd, 50*mode, vex::percentUnits::pct);
}

void Robot::liftUp(int value)
{
  vex::motor m = vex::motor (RIGHT_LIFT_MOTOR,false);
  vex::motor m2 = vex::motor (LEFT_LIFT_MOTOR, true);
  m.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
  m2.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
}

void Robot::liftDown(int value)
{
  vex::motor m = vex::motor (RIGHT_LIFT_MOTOR,false);
  vex::motor m2 = vex::motor (LEFT_LIFT_MOTOR, true);
  m.spin(vex::directionType::fwd, -100, vex::percentUnits::pct);
  m2.spin(vex::directionType::fwd, -100, vex::percentUnits::pct);
}

void Robot::driveFor(double distance, double velocity) {
  for(int i=0; i<driveMotors.size(); i++)
  {
    vex::motor m = driveMotors.at(i);
    driveMotors.at(i).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct);
  }
}
