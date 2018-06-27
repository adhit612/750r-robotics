#define D_MOTOR_1 3
#define D_MOTOR_2 8
#define D_MOTOR_3 9
#define D_MOTOR_4 2

#define INTAKE_MOTOR 4
#define LAUNCHER_MOTOR 5
#define LAUNCHER_MOTOR2 6
#define CARRIER_MOTOR 7

#define JOYSTICK 1
#define V_AXIS 1
#define H_AXIS 2
#define VLEFT_AXIS 3

void drive(int x, int y);
void intake(int in);
void launch();
void auton();
void carry(int in);