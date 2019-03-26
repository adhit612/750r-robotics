#include "main.h"
#include "path-generator.h"
#include <vector>

class Robot {
	private:
		std::vector<pros::Motor> driveMotors;
		pros::Motor roller;
		pros::Motor flywheel;
		pros::Motor indexer;
		pros::Motor capLift;
		pros::ADIGyro gyro;
		
		int autonIndex;
		
		double x, y, angle;
		double flywheelSpeed;
	
	public:
		Robot(const int portFL, const int portBL, const int portFR, const int portBR,
			  const int intakePort, const int flywheelPort, const int indexerPort,
			  const int liftPort, const char gyroPort);
		void setAutonIndex(int index);
		
		void drive(int x, int y);
		void intake(int mode);
		void lift(int power);
		void engageFlywheel(bool engage);
		
		double getX();
		double getY();
		double getAngle();
		double getFlywheelSpeed();
		pros::Motor getMotorAt(int index);
		
		void setX(double newX);
		void setY(double newY);
		void setAngle(double newAngle);
		void setFlywheelSpeed(double newSpeed);
		
		void moveTo(double targetX, double targetY, double velocity);
		void turnTo(double angle, double velocity);
		
		void resetGyro();
		double getGyro();
		pros::Motor getFlywheelMotor();
		
		void startFlywheel();
		void startFieldPosition();
		
		void tareDriveMotors();
		
		void followPath(PathGenerator generator);
		
};