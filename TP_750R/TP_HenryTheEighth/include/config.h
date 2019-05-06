#include "vex.h"
#include "robot.h"
#include <vector>

#define D_MOTOR_FL vex::PORT7
#define D_MOTOR_BL vex::PORT4
#define D_MOTOR_FR vex::PORT6
#define D_MOTOR_BR vex::PORT5

#define INTAKE_PORT vex::PORT11
#define FLYWHEEL_PORT vex::PORT15
#define INDEXER_PORT vex::PORT13
#define LIFT_PORT vex::PORT18

#define INTAKE_MODE_IN 1
#define INTAKE_MODE_OUT -1
#define INTAKE_MODE_STOP 0

#define LIFT_MODE_UP 1
#define LIFT_MODE_DOWN -1
#define LIFT_MODE_HOLD 0

extern Robot robot;
extern vex::mutex driveMutex;