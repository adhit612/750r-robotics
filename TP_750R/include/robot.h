#include "main.h"
#include <vector>

class Robot {
	private:
		std::vector<pros::Motor> driveMotors;
		int autonIndex;
	
	public:
		Robot(const int portFL, const int portBL, const int portFR, const int portBR);
		void drive(int x, int y);
		void setAutonIndex(int index);
};