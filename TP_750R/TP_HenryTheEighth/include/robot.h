#include "vex.h"
#include "path-generator.h"
#include <vector>

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
	private:
    vex::brain cortex;
		std::vector<vex::motor> driveMotors;
		vex::motor roller;
		vex::motor flywheel;
		vex::motor indexer;
		vex::motor capLift;
    vex::line lineSensor;
		vex::gyro g;
    
    double startingLineValue;
		
		int autonIndex;
		
		double x, y, angle, driveApprox;
		double flywheelSpeed;

    bool dsFlag;
	
	public:
		Robot(vex::brain cortex);
		
		void drive(int x, int y);
		void intake(int mode);
		void lift(int mode);
		void engageFlywheel(bool engage);
		
		double getX();
		double getY();
		double getAngle();
		double getFlywheelSpeed();
    double getDriveApprox();
		vex::motor getMotorAt(int index);
		
		void setX(double newX);
		void setY(double newY);
		void setAngle(double newAngle);
		void setFlywheelSpeed(double newSpeed);
    void setDriveApprox(double newApprox);

		void selectAuton();
    void auton();
		
		void pidDriveFor(double distance, double limitSpeed);
    void gyroTurn(double angle);
    void driveFor(double distance, double velocity);
    void cwTurn(double angle, double velocity);
    void ccwTurn(double angle, double velocity);
    void autoIntake();
    void autoFlip(double distance, double velocity);
    void autoShoot(double time);
    void wheelie(double distance);
    void doubleShot(double delay);
    void modifiedDoubleShot(double delay);
    void liftTo(double angle);
		
		void resetGyro();
		double getGyro();
    bool getDSFlag();
    double getLineSensor();
		vex::motor getFlywheelMotor();
		
		void startFlywheel();
		void startFieldPosition();
		
		void tareDriveMotors();
		
		void followPath(PathGenerator generator);
		
};

#endif