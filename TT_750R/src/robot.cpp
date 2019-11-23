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
    if(i==0) //LEFT
		  m.spin(vex::directionType::fwd, value, vex::percentUnits::pct);
    else if(i==1) //RIGHT
      m.spin(vex::directionType::fwd, value, vex::percentUnits::pct);
	}
}

void Robot::turnRight() {
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
    if(i%2==0) 
			m.spin(vex::directionType::fwd, 10, vex::percentUnits::pct); //LEFT SIDE
    else
      m.spin(vex::directionType::rev, 10, vex::percentUnits::pct); //RIGHT SIDE
	}
}

void Robot::turnLeft() {
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
    if(i%2==0)
			m.spin(vex::directionType::rev, 10, vex::percentUnits::pct); //LEFT SIDE
    else
      m.spin(vex::directionType::fwd, 10, vex::percentUnits::pct); //RIGHT SIDE
	}
}

void Robot::strafe(int mode)
{
		vex::motor m = vex::motor(D_MOTOR_M);
		m.spin(vex::directionType::fwd, 50*mode, vex::percentUnits::pct);
}

void Robot::lift(int mode)
{
  vex::motor m = vex::motor (RIGHT_LIFT_MOTOR,false);
  vex::motor m2 = vex::motor (LEFT_LIFT_MOTOR, true);
  m.spin(vex::directionType::fwd, 75*mode, vex::percentUnits::pct);
  m2.spin(vex::directionType::fwd, 75*mode, vex::percentUnits::pct);

  if(mode==0)
  {
    m.stop(vex::brakeType::hold);
    m2.stop(vex::brakeType::hold);
  }
}

void Robot::moveMagazine(double mode)
{
  vex::motor m = vex::motor(MAGAZINE_MOTOR);
  m.spin(vex::directionType::fwd,25*mode,vex::percentUnits::pct);
  if(mode==0)
  {
    m.stop(vex::brakeType::hold);
  }
}

void Robot::intake(int mode)
{
  vex::motor m = vex::motor(LEFT_ROLLER_MOTOR, false);
  vex::motor m2 = vex::motor(RIGHT_ROLLER_MOTOR, true);
  m.spin(vex::directionType::fwd,100*mode,vex::percentUnits::pct);
  m2.spin(vex::directionType::fwd,100*mode,vex::percentUnits::pct);

  if(mode==0)
  {
    m.stop(vex::brakeType::hold);
    m2.stop(vex::brakeType::hold);
  }
}

void Robot::driveFor(double distance, double velocity) {
  driveMotors.at(0).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
}

void Robot::turnFor(double angle) { //POSITIVE VALUE = TURN LEFT
  for(int i=0; i<driveMotors.size(); i++)
  {
    driveMotors.at(0).rotateFor(vex::directionType::fwd, -angle, vex::rotationUnits::deg, false);
    driveMotors.at(1).rotateFor(vex::directionType::fwd, angle, vex::rotationUnits::deg, true);
  }
}

void Robot::intakeFor(double distance, int mode)
{
  vex::motor m = vex::motor(LEFT_ROLLER_MOTOR, false);
  vex::motor m2 = vex::motor(RIGHT_ROLLER_MOTOR, true);
  m.rotateFor(vex::directionType::fwd, distance*mode, vex::rotationUnits::deg, false);
  m2.rotateFor(vex::directionType::fwd, distance*mode, vex::rotationUnits::deg, true);
}

void Robot::magazineFor(int mode)
{
  vex::motor m = vex::motor(MAGAZINE_MOTOR);
  m.rotateFor(vex::directionType::fwd, 1000*mode, vex::rotationUnits::deg, true);
}

void Robot::intakeAndMove(double distance, double velocity,int mode)
{
  vex::motor m = vex::motor(LEFT_ROLLER_MOTOR,false);
  vex::motor m2 = vex::motor(RIGHT_ROLLER_MOTOR, true);
  driveMotors.at(0).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  m.rotateFor(vex::directionType::fwd, distance*mode, vex::rotationUnits::deg, false);
  m2.rotateFor(vex::directionType::fwd, distance*mode, vex::rotationUnits::deg, true);
}

void Robot::liftTo(int mode)
{
  vex::motor m = vex::motor (RIGHT_LIFT_MOTOR,false);
  vex::motor m2 = vex::motor (LEFT_LIFT_MOTOR, true);
  m.rotateFor(vex::directionType::fwd, 100*mode, vex::rotationUnits::deg, 75, vex::velocityUnits::pct, false);
  m2.rotateFor(vex::directionType::fwd, 100*mode, vex::rotationUnits::deg, 75, vex::velocityUnits::pct, false);
  //m.startRotateTo(2238, vex::rotationUnits::deg, 175*mode, vex::velocityUnits::pct);
  //m2.startRotateTo(-1980, vex::rotationUnits::deg, 175*mode, vex::velocityUnits::pct);

  if(mode==0)
  {
    m.stop(vex::brakeType::hold);
    m2.stop(vex::brakeType::hold);
  }
}
