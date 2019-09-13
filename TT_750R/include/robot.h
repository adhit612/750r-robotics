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
    void strafe(int value);
    void liftUp(int value);
    void liftDown(int value);
    void clawOpen();
    void clawClose();
    void driveFor();
};
#endif
