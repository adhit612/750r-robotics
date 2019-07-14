#include "vex.h"
#include <vector>

#ifndef ROBOT_H
#define ROBOT_H

class Robot {
	private:
    vex::brain cortex;
		std::vector<vex::motor> driveMotors;

	public:
		Robot(vex::brain cortex);
		
		void drive(int value);
    void turnRight();
    void turnLeft();
    void strafeRight(int value);
    void strafeLeft(int value);
};
#endif
