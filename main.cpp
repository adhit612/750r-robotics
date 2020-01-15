#include "main.h"

//MOTOR PORTS 4
int DRIVE_MOTOR_FL = 12;
int DRIVE_MOTOR_FR = -19;
int DRIVE_MOTOR_BL = 11;
int DRIVE_MOTOR_BR = -20;
int TILTER = 1;
int LIFT = -9;
int LEFT_ROLLER = 6;
int RIGHT_ROLLER = -10;

//MOTOR DECLARATION
Motor tilter(TILTER);
Motor lift(LIFT);
Motor rollerL(LEFT_ROLLER);
Motor rollerR(RIGHT_ROLLER);
Potentiometer pot(1);

Controller controller;

ControllerButton intakeIn(ControllerDigital::R1);
ControllerButton intakeOut(ControllerDigital::R2);

ControllerButton magazineForward(ControllerDigital::X);
ControllerButton magazineBackward(ControllerDigital::B);

ControllerButton liftUp(ControllerDigital::A);
ControllerButton liftDown(ControllerDigital::Y);

ControllerButton driveToggle(ControllerDigital::L1);

ControllerButton right(ControllerDigital::right);
ControllerButton left(ControllerDigital::left);

bool fastDrive=true;

auto drive = ChassisControllerBuilder()
.withMotors({DRIVE_MOTOR_FL, DRIVE_MOTOR_BL}, {DRIVE_MOTOR_FR, DRIVE_MOTOR_BR})
.withDimensions(AbstractMotor::gearset::green, {{4_in, 12_in}, imev5GreenTPR})
.build();

const double rollerkP = 0.001;
const double rollerkI = 0.0001;

auto rollerController = AsyncPosControllerBuilder()
.withMotor(6)
.withMotor(-10)
.build();

auto liftController = AsyncPosControllerBuilder()
.withMotor(-9)
.build();



void rollers(int speed){
	rollerL.moveVelocity(speed);
	rollerR.moveVelocity(speed);
}

void intakeControl(){
	if(intakeIn.isPressed()){
		rollerL.moveVelocity(200);
		rollerR.moveVelocity(200);
	}
	else if(intakeOut.isPressed()){
		rollerL.moveVelocity(-200);
		rollerR.moveVelocity(-200);
	}
	else{
		rollerL.moveVelocity(0);
		rollerR.moveVelocity(0);
		rollerL.setBrakeMode(AbstractMotor::brakeMode::hold);
		rollerR.setBrakeMode(AbstractMotor::brakeMode::hold);
	}
}

void magazineControl(){
	if(magazineForward.isPressed())
  {
		tilter.moveVelocity(50);
  }
	else if(magazineBackward.isPressed()){
		tilter.moveVelocity(-50);
  }
	else{
		tilter.moveVelocity(0);
		tilter.setBrakeMode(AbstractMotor::brakeMode::hold);
	}
}

void liftControl(){
	int target=3747;
	int error=pot.get()-target;
	if(liftUp.isPressed()){
		lift.moveVelocity(100);
		tilter.moveVelocity(30);
		/*while(error>10){
    	tilter.moveVelocity(30);}
		while(error<-10){
	    	tilter.moveVelocity(-30);}*/
	}
	else if(liftDown.isPressed()){
		lift.moveVelocity(-100);
		tilter.moveVelocity(-30);
		/*while(error>10){
    	tilter.moveVelocity(30);}
		while(error<-10){
	    	tilter.moveVelocity(-30);}*/
	}
	else{
		lift.moveVelocity(0);
		lift.setBrakeMode(AbstractMotor::brakeMode::hold);
	}
}

void driveControl(){
	if(driveToggle.isPressed())
		fastDrive=false;
}

void stack(){

}

void stackAndBack(){

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
 //auto  liftTask = AsyncVelControllerBuilder::posPID()
void autonomous() {

  /*drive->moveDistance(2_ft);
	rollerController->setTarget(100);
	drive->waitUntilSettled();
	//liftController->waitUntilSettled();
	drive->turnAngle(-270_deg);
	drive->moveDistance(1.5_ft);
	liftController->setTarget(150);
	drive->moveDistance(-1_ft);*/

	//BACK BLUE AUTON
	/*rollers(-200);
	drive->setMaxVelocity(75);
	pros::delay(3000);
	rollers(200);
	drive->moveDistance(52.5_in);
	drive->waitUntilSettled();
	rollers(0);
	drive->moveDistance(-35_in);
	drive->waitUntilSettled();
	drive->setMaxVelocity(80);
	drive->turnAngle(-106_deg);
	drive->waitUntilSettled();
	drive->moveDistance(13_in);
	drive->waitUntilSettled();
	rollers(-70);
	pros::delay(1000);
	rollers(0);
	tilter.moveRelative(2000, 100);
	pros::delay(3000);
	rollers(-100);
	drive->moveDistance(-13_in);
	drive->waitUntilSettled();
	rollers(0);*/

	//BACK RED AUTON
	/*rollers(-200);
	drive->setMaxVelocity(75);
	pros::delay(3000);
	rollers(200);
	drive->moveDistance(52.5_in);
	drive->waitUntilSettled();
	rollers(0);
	drive->moveDistance(-35_in);
	drive->waitUntilSettled();
	drive->setMaxVelocity(80);
	drive->turnAngle(106_deg);
	drive->waitUntilSettled();
	drive->moveDistance(13_in);
	drive->waitUntilSettled();
	rollers(-70);
	pros::delay(1000);
	rollers(0);
	tilter.moveRelative(2000, 100);
	pros::delay(3000);
	rollers(-100);
	drive->moveDistance(-13_in);
	drive->waitUntilSettled();
	rollers(0);*/

	//FRONT AUTON
	drive->setMaxVelocity(75);
	rollers(-200);
	drive->moveDistance(6_in);
	drive->waitUntilSettled();
	drive->moveDistance(-7_in);
	rollers(0);
	drive->waitUntilSettled();
	drive->moveDistance(1.5_ft);
	drive->waitUntilSettled();
	rollers(-200);
	drive->moveDistance(-2_ft);
	rollers(0);

	//FRONT AUTON ANTITIP
	/*drive->setMaxVelocity(75);
	drive->moveDistance(10_in);
	drive->waitUntilSettled();
	drive->turnAngle(85_deg);
	drive->waitUntilSettled();
	drive->moveDistance(10_in);
	drive->waitUntilSettled();
	rollers(-200);
	pros::delay(1000);
	rollers(0);
	drive->moveDistance(-1_ft);*/
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
		if(driveToggle.isPressed())
    	drive->getModel()->arcade(controller.getAnalog(ControllerAnalog::leftY)/4,(controller.getAnalog(ControllerAnalog::rightX)/2));
		else
			drive->getModel()->arcade((controller.getAnalog(ControllerAnalog::leftY)), (controller.getAnalog(ControllerAnalog::rightX)/2));
		intakeControl();
		magazineControl();
		liftControl();

		//if(right.isPressed()){
		//	tilter.moveRelative(2500, 100);
			//pros::delay(3000);}
		if(left.isPressed()){
			drive->setMaxVelocity(75);
			lift.moveVelocity(-100);
			tilter.moveRelative(2500, 100);
			pros::delay(3000);
			rollerR.moveRelative(-1000,125);
			rollerL.moveRelative(-1000,125);
			drive->moveDistance(-13_in);
		}

		pros::delay(20);
	}
}
