#include "main.h"
#include "robot.h"
#include "config.h"
#include <vector>
#include <cmath>

Robot::Robot(const int portFL, const int portBL, const int portFR, const int portBR,
			 const int intakePort, const int flywheelPort, const int indexerPort,
			 const int liftPort, const char gyroPort) : driveMotors(), roller(intakePort),
			 flywheel(flywheelPort), indexer(indexerPort), capLift(liftPort, true), gyro(gyroPort),
			 autonIndex(0), x(0.0), y(0.0), angle(0.0), flywheelSpeed(200.0) {
	driveMotors.push_back(pros::Motor(portFL, true));
	driveMotors.push_back(pros::Motor(portFR, false));
	driveMotors.push_back(pros::Motor(portBL, true));
	driveMotors.push_back(pros::Motor(portBR, false));
	capLift.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
	gyro.reset();
}

void Robot::setAutonIndex(int index) {
	autonIndex = index;
	
	if(autonIndex == 0 || autonIndex == 2) { //FR
		x = 0.5;
		y = 3.5;
		angle = 0.0;
	} else if(autonIndex == 1 || autonIndex == 3) { //FB
		x = 5.5;
		y = 3.5;
		angle = 180.0;
	} else if(autonIndex == 4 || autonIndex == 6) { //BR
		x = 0.5;
		y = 1.5;
		angle = 0;
	} else if(autonIndex == 5 || autonIndex == 7) { // BB
		x = 5.5;
		y = 1.5;
		angle = 180.0;
	}
}

void Robot::drive(int x, int y) {
	for(int i = 0; i < driveMotors.size(); i++) {
		pros::Motor m = driveMotors.at(i);
		if(i % 2 == 0) {
			m.move_velocity(-(y + x));
		} else {
			m.move_velocity(-(y - x));
		}
	}
}

void Robot::intake(int mode) {
	roller.move_velocity(200 * mode);
}

void Robot::lift(int mode) {
	capLift.move_velocity(100 * mode);
}

void Robot::engageFlywheel(bool engage) {
	indexer.move_velocity((engage ? 200 : 0));
}

double Robot::getX() { return x; }
double Robot::getY() { return y; }
double Robot::getAngle() { return angle; }
double Robot::getFlywheelSpeed() { return flywheelSpeed; }

void Robot::setX(double newX) { x = newX; }
void Robot::setY(double newY) { y = newY; }
void Robot::setAngle(double newAngle) { angle = newAngle; }
void Robot::setFlywheelSpeed(double newSpeed) { flywheelSpeed = newSpeed; }

void Robot::moveTo(double targetX, double targetY, double velocity) {
	double dx = targetX - x;
	double dy = targetY - y;
	
	double targetAngle = atan2(dy, dx);
	turnTo(targetAngle, 100);
	
	for(int i = 0; i < driveMotors.size(); i++) {
		driveMotors.at(i).move_velocity(velocity);
	}
	
	while(fabs(targetX - getX()) > 0.01 && fabs(targetY - getY()) > 0.01) { pros::delay(20); }
	
	for(int i = 0; i < driveMotors.size(); i++) {
		driveMotors.at(i).move_velocity(0);
	}
	
}

void Robot::turnTo(double targetAngle, double velocity) {
	const double FULL_CIRCLE = 1200;
	
	for(int i = 0; i < driveMotors.size(); i++) {
		if(i % 2 == 0)
			driveMotors.at(i).move_velocity(-velocity);
		else
			driveMotors.at(i).move_velocity(velocity);
	}
	
	while(fabs(targetAngle - getAngle()) > 0.1) { pros::delay(20); }
	
	for(int i = 0; i < driveMotors.size(); i++) {
		driveMotors.at(i).move_velocity(0);
	}
	
}

void Robot::resetGyro() {
	gyro.reset();
	pros::delay(1000); 
}

double Robot::getGyro() { return gyro.get_value(); }

pros::Motor Robot::getFlywheelMotor() {
	return flywheel;
}

void Robot::startFlywheel() {
	
	pros::Task flywheelTask([](void* param){
		
		Robot* robot = (Robot*)param;
		
		const double kF = 0;
		const double kP = 0;
		const double kI = 0;
		const double kD = 0;
		
		//(*robot).getFlywheelMotor().set_vel_pid(pros::Motor::convert_pid(kF, kP, kI, kD));
		(*robot).getFlywheelMotor().move_velocity((*robot).getFlywheelSpeed());
		
		while(true) {
			if((*robot).getFlywheelMotor().get_temperature() >= 55)
				(*robot).getFlywheelMotor().move_velocity((*robot).getFlywheelSpeed() * 0.90);
			if((*robot).getFlywheelMotor().get_temperature() < 50)
				(*robot).getFlywheelMotor().move_velocity((*robot).getFlywheelSpeed());
			
			pros::delay(100);
		}
	}, this);
}

void Robot::startFieldPosition() {
	
	pros::Task fieldPositionTask([](void* robot_param){
		const double FULL_TILE = 660;
		const double FULL_CIRCLE = 1200;
		
		Robot* robot = (Robot*)robot_param;
		
		while(true) {
			
			if(driveMutex.take((uint32_t)0xffffffffUL)) {
				double fl = (*robot).getMotorAt(0).get_position();
				double fr = (*robot).getMotorAt(1).get_position();
				
				double distance = (fl + fr) / (2 * FULL_TILE);
				double turns = (fl - fr) / (2 * FULL_CIRCLE);
				
				if(!(distance == 0 && turns == 0)) {
	
					(*robot).setAngle((*robot).getAngle() + turns * 360);
					(*robot).setX((*robot).getX() + distance * cos((*robot).getAngle()));
					(*robot).setY((*robot).getY() + distance * sin((*robot).getAngle()));
				
					(*robot).tareDriveMotors();
				}
				driveMutex.give();
			}
			
			pros::delay(50);
		}
	}, this);
}

void Robot::tareDriveMotors() {
	for(int i = 0; i < driveMotors.size(); i++)
		driveMotors.at(i).tare_position();
}

pros::Motor Robot::getMotorAt(int index) {
	return driveMotors.at(index);
}

void Robot::followPath(PathGenerator generator) {
	
}
