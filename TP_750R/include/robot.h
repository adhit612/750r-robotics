#include "main.h"
#include <vector>

class Robot {
	private:
		std::vector<pros::Motor> driveMotors;
	
	public:
		Robot(const int portFL, const int portBL, const int portFR, const int portBR);
		void drive(int x, int y);
};