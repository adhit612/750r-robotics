#include "main.h"

//MOTOR PORTS BROKEN = 10 12 11
int8_t DRIVE_MOTOR_FL = 13;
int8_t DRIVE_MOTOR_FR = -19;
int8_t DRIVE_MOTOR_BL = 12;
int8_t DRIVE_MOTOR_BR = -20;
int TILTER = -1;
int LIFT = 16;
int LEFT_ROLLER = 6;
int RIGHT_ROLLER = -8;


//TARGETS FOR MACROS
int midTowerTarget=2223;
int trayTarget=2093;
//2006 before
int lowTowerTarget = 1750;
int downTarget=3494;

bool macroHappening = false;
bool temperatureShow = true;

//MOTOR DECLARATION
Motor tilter(TILTER);
Motor lift(LIFT);
Motor rollerL(LEFT_ROLLER);
Motor rollerR(RIGHT_ROLLER);
Motor driveFL(DRIVE_MOTOR_FL);
Motor driveFR(DRIVE_MOTOR_FR);
Motor driveBL(DRIVE_MOTOR_BL);
Motor driveBR(DRIVE_MOTOR_BR);

//POT DECLARATION
Potentiometer liftPot('B');
Potentiometer trayPot('C');

//CONTROLLER
Controller controller;

//BUTTONS
ControllerButton intakeIn(ControllerDigital::R1);
ControllerButton intakeOut(ControllerDigital::R2);

ControllerButton magazineForward(ControllerDigital::X);
ControllerButton magazineBackward(ControllerDigital::B);

ControllerButton liftUp(ControllerDigital::L1);
ControllerButton liftDown(ControllerDigital::L2);

ControllerButton driveToggle(ControllerDigital::L1);

ControllerButton lowTowerButton(ControllerDigital::right);
ControllerButton stackButton(ControllerDigital::left);
ControllerButton midTowerButton(ControllerDigital::down);

ControllerButton override(ControllerDigital::up);

ControllerButton descoreButton(ControllerDigital::Y);

//DRIVE
auto drive = ChassisControllerBuilder()
.withMotors({DRIVE_MOTOR_FL, DRIVE_MOTOR_BL}, {DRIVE_MOTOR_FR, DRIVE_MOTOR_BR})
.withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
.withOdometry()
.buildOdometry();

auto liftController = AsyncPosControllerBuilder()
											.withMotor(LIFT)
											.build();


//TASKS
void driveTask(void* param){
		drive->getModel()->arcade((controller.getAnalog(ControllerAnalog::leftY)), (controller.getAnalog(ControllerAnalog::rightX)/2));
}
void stackTask(void* param){
	if(stackButton.isPressed()){
		rollerL.setBrakeMode(AbstractMotor::brakeMode::brake);
		rollerR.setBrakeMode(AbstractMotor::brakeMode::brake);
		int error=trayTarget-trayPot.get();
		while(fabs(error)>10)
		{
			if(override.isPressed())
			{
				break;
			}
			error=trayTarget-trayPot.get();
			if(fabs(error)>500)
			{
				tilter.moveVelocity(65);
				lift.moveVelocity(-100);
			}
			else
				tilter.moveVelocity(30);
		}
			pros::delay(20);
	}
}


void pidTurn(int value){
	double kP;
	double kI;
	double kD;
}

//USER CONTROL FUNCTIONS
void rollers(int speed){
	rollerL.moveVelocity(speed);
	rollerR.moveVelocity(speed);
}

void intakeControl(){
	if(intakeIn.isPressed()){
		rollers(200);
	}
	else if(intakeOut.isPressed()){
		if(liftPot.get()>1500)
			rollers(-100);
		else
			rollers(-200);
	}
	else{
		rollers(0);
	}
}

void descore(){
	if(descoreButton.isPressed())
		rollers(-200);
}

void magazineControl(){
	if(magazineForward.isPressed())
	{
		tilter.moveVelocity(50);
  }
	else if(magazineBackward.isPressed())
	{
		tilter.moveVelocity(-100);
  }
	else
	{
		tilter.moveVelocity(0);
	}
}

void liftControl(){
	if(liftUp.isPressed())
	{
			lift.moveVelocity(80);
	}
	else if(liftDown.isPressed())
	{
		lift.moveVelocity(-100);
	}
	else if(!macroHappening)
	{
		lift.moveVelocity(0);
	}
}

void driveControl(){
		drive->getModel()->arcade((controller.getAnalog(ControllerAnalog::leftY)), (controller.getAnalog(ControllerAnalog::rightX)*.75));
}

//MACROS
void stack()
{
	if(stackButton.isPressed()){
		rollerL.setBrakeMode(AbstractMotor::brakeMode::brake);
		rollerR.setBrakeMode(AbstractMotor::brakeMode::brake);
		int error=trayTarget-trayPot.get();
		while(fabs(error)>10)
		{
			if(override.isPressed())
			{
				break;
			}
			error=trayTarget-trayPot.get();
			if(fabs(error)>500)
			{
				tilter.moveVelocity(65);
				lift.moveVelocity(-100);
			}
			else
				tilter.moveVelocity(30);
		}
			pros::delay(20);
	}
}

void comeback(){
	int error=downTarget-trayPot.get();
	while(fabs(error)>10)
	{
		error=downTarget-trayPot.get();
		if(override.isPressed())
		{
			break;
		}
		tilter.moveVelocity(-100);

	}
	pros::delay(20);
}

void midTowerMacro()
{
	if(midTowerButton.changedToPressed())
	{
		macroHappening = true;
		liftController->setTarget(midTowerTarget);
	}
	// if(liftController.isSettled()){
	// 	macroHappening = false;
	// }
}


void lowTowerMacro()
{
	if(lowTowerButton.changedToPressed()){
		macroHappening = true;
		liftController->setTarget(lowTowerTarget);
	}
	if(liftController.isSettled()){
		macroHappening = false;
	}
}

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	tilter.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerL.setBrakeMode(AbstractMotor::brakeMode::hold);
	rollerR.setBrakeMode(AbstractMotor::brakeMode::hold);

	//BACK BLUE AUTON
	lift.moveVelocity(-100);
	rollers(-200);
	pros::delay(1200);
	rollers(200);
	drive->setMaxVelocity(75);
	drive->moveDistance(45.5_in);
	drive->waitUntilSettled();
	pros::delay(500);
	rollers(0);
	drive->moveDistance(-27.5_in);
	drive->waitUntilSettled();
	drive->setMaxVelocity(50);
	drive->turnAngle(-100_deg);
	drive->waitUntilSettled();
	drive->setMaxVelocity(75);
	drive->moveDistance(14.5_in);
	drive->waitUntilSettled();
	rollers(-70);
	pros::delay(720);
	rollers(0);
	tilter.moveRelative(2550,100);
	pros::delay(2000);
	rollers(-125);
	drive->moveDistance(-15_in);
	drive->waitUntilSettled();
	rollers(0);

	//BACK RED AUTON
	/*lift.moveVelocity(-100);
	rollers(-200);
	pros::delay(1200);
	rollers(200);
	drive->setMaxVelocity(75);
	drive->moveDistance(45.5_in);
	drive->waitUntilSettled();
	pros::delay(500);
	rollers(0);
	drive->moveDistance(-27.5_in);
	drive->waitUntilSettled();
	drive->setMaxVelocity(50);
	drive->turnAngle(105_deg);
	drive->moveDistance(13_in);
	drive->waitUntilSettled();
	rollers(-70);
	pros::delay(740);
	rollers(0);
	tilter.moveRelative(2550,100);
	pros::delay(2500);
	rollers(-125);
	drive->moveDistance(-15_in);
	tilter.moveVelocity(-100);
	drive->waitUntilSettled();
	rollers(0);*/

	//ONE POINT
	/*drive->setMaxVelocity(50);
	rollers(-200);
	pros::delay(1200);
	rollers(0);
	lift.moveVelocity(-100);
	drive->moveDistance(20_in);
	drive->waitUntilSettled();
	drive->setMaxVelocity(75);
	rollers(-100);
	drive->moveDistance(-1_ft);
	drive->waitUntilSettled();
	rollers(0);*/

	//TWO POINT RED
	/*drive->setMaxVelocity(75);
	rollers(-200);
	pros::delay(1200);
	rollers(0);
	rollers(200);
	drive->moveDistance(25_in);
	drive->moveDistance(-25.5_in);
	rollers(0);
	drive->turnAngle(-90_deg);
	drive->moveDistance(27_in);
	rollers(-70);
	pros::delay(740);
	rollers(0);
	tilter.moveRelative(2600,100);
	pros::delay(2000);
	rollers(-125);
	drive->moveDistance(-15_in);
	drive->waitUntilSettled();
	rollers(0);*/

	//TWO POINT BLUE
	/*drive->setMaxVelocity(75);
	rollers(-200);
	pros::delay(1200);
	rollers(0);
	rollers(200);
	drive->moveDistance(25_in);
	drive->waitUntilSettled();
	drive->moveDistance(-23_in);
	drive->waitUntilSettled();
	rollers(0);
	drive->turnAngle(90_deg);
	drive->waitUntilSettled();
	drive->moveDistance(27_in);
	drive->waitUntilSettled();
	rollers(-70);
	pros::delay(740);
	rollers(0);
	tilter.moveRelative(2600,100);
	pros::delay(2000);
	rollers(-125);
	drive->moveDistance(-15_in);
	drive->waitUntilSettled();
	rollers(0);*/
	}


/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	while (true) {
		tilter.setBrakeMode(AbstractMotor::brakeMode::hold);
		rollerL.setBrakeMode(AbstractMotor::brakeMode::hold);
		rollerR.setBrakeMode(AbstractMotor::brakeMode::hold);
		lift.setBrakeMode(AbstractMotor::brakeMode::hold);

		while(temperatureShow)
		{
		controller.setText(1, 7, std::to_string(lift.getTemperature()));
	}

		pros::Task drive (driveTask, (void*)"PROS", TASK_PRIORITY_MAX);
		//pros::Task stack (stackTask, (void*)"PROS", TASK_PRIORITY_DEFAULT);
		//pros::Task lowTowerMacro (lowTowerMacroTask, (void*)"PROS", TASK_PRIORITY_DEFAULT);
		driveControl();
		intakeControl();
		magazineControl();
		liftControl();
		descore();
		stack();


		// if(midTowerButton.isPressed())
		// {
		 	midTowerMacro();
		// }
		lowTowerMacro();
		pros::delay(20);
	}
}
