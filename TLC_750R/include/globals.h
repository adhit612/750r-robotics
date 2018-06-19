#define D_MOTOR_1 1
#define D_MOTOR_2 2
#define D_MOTOR_3 9
#define D_MOTOR_4 10

#define INTAKE_MOTOR 3

#define LAUNCHER_MOTOR 4

#define JOYSTICK 1
#define H_AXIS 1
#define V_AXIS 2

void drive(int x, int y);
void intake(int speed);
void launch();
void auton();
