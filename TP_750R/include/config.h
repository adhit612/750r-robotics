#include "main.h"
#include <vector>

#define D_MOTOR_FL 3
#define D_MOTOR_BL 19
#define D_MOTOR_FR 14
#define D_MOTOR_BR 8

#define INTAKE_PORT 11
#define FLYWHEEL_PORT 15
#define INDEXER_PORT 13
#define LIFT_PORT 18

#define GYRO_PORT 'C'

#define INTAKE_MODE_IN 1
#define INTAKE_MODE_OUT -1
#define INTAKE_MODE_STOP 0

#define LIFT_MODE_UP 1
#define LIFT_MODE_DOWN -1
#define LIFT_MODE_HOLD 0

extern int autonIndex;
extern pros::Mutex driveMutex;

void autonomous();