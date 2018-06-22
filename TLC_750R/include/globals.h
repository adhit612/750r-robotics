#define D_MOTOR_1 3
#define D_MOTOR_2 8
#define D_MOTOR_3 9
#define D_MOTOR_4 2

#define INTAKE_MOTOR 5

#define LAUNCHER_MOTOR 4

#define JOYSTICK 1
#define H_AXIS 1
#define V_AXIS 2

void drive(int x, int y);
void intake(int speed);
void launch();
void auton();
