#include "main.h"
#include <vector>

class Robot {
	private:
		std::vector<pros::Motor> driveMotors;
		pros::Motor roller;
		pros::Motor flywheel;
		pros::Motor indexer;
		pros::Motor capLift;
		int autonIndex;
	
	public:
		Robot(const int portFL, const int portBL, const int portFR, const int portBR,
			  const int intakePort, const int flywheelPort, const int indexerPort,
			  const int liftPort);
		void setAutonIndex(int index);
		void drive(int x, int y);
		void intake(int mode);
		void lift(int power);
};