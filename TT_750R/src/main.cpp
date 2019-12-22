#include "vex.h"
#include "config.h"
#include "robot.h"
using namespace vex;

// A global instance of vex::brain used for printing to the V5 brain screen
vex::brain       Brain;
// A global instance of vex::competition
vex::competition Competition;

// define your global instances of motors and other devices here
vex::controller  controller1    (vex::controllerType::primary);

void liftTo()
{
  
}
/*---------------------------------------------------------------------------*/
/*                          Pre-Autonomous Functions                         */
/*                                                                           */
/*  You may want to perform some actions before the competition starts.      */
/*  Do them in the following function.  You must return from this function   */
/*  or the autonomous and usercontrol tasks will not be started.  This       */
/*  function is only called once after the cortex has been powered on and    */ 
/*  not every time that the robot is disabled.                               */
/*---------------------------------------------------------------------------*/
vex::mutex driveMutex;
Robot robot(Brain);

void pre_auton( void ) {
  // All activities that occur before the competition starts
  // Example: clearing encoders, setting servo positions, ...
  
}

void turnEncoderPID(double angle)
{
 vex::motor m = vex::motor(D_MOTOR_L);
 vex::motor m2 = vex::motor(D_MOTOR_R);
 
 double newMValue;
 double newM2Value;
 
 if(m.value() < 0)
 {
   newMValue = m.value() * -1;
 }
 else
 {
   newMValue = m.value();
 }
  if(m2.value() < 0)
 {
   newM2Value = m2.value() * -1;
 }
 else
 {
   newM2Value = m2.value();
 }
 
double average = (newM2Value + newMValue) / 2.0;
 
const double ENDPOINT = (average + angle);
 
double currentValue = average;
double currentError = ENDPOINT - currentValue;
 
while(fabs(currentError) > 2.5){
controller1.Screen.setCursor(1, 1);
controller1.Screen.print(currentError);
 
double kP = 0.1;
 
currentError = ENDPOINT - currentValue;
 
double p = kP * currentError;
 
m.spin(directionType::fwd, p, velocityUnits::pct);
m2.spin(directionType::rev, p, velocityUnits::pct);
 
if(m.value() < 0)
 {
   newMValue = m.value() * -1;
 }
 else
 {
   newMValue = m.value();
 }
  if(m2.value() < 0)
 {
   newM2Value = m2.value() * -1;
 }
 else
 {
   newM2Value = m2.value();
 }
 
currentValue = (m.value() + m2.value()) / 2.0;
 
task::sleep(20);
}
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              Autonomous Task                              */
/*                                                                           */
/*  This task is used to control your robot during the autonomous phase of   */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void autonomous( void ) {
  // ..........................................................................
  // Insert autonomous user code here.
  // ..........................................................................  
  int fullTile=550;
  int fullCircle=515;
  bool redSide=false;
  bool blueSide=true;

  if(blueSide){
    robot.deploy();
    vex::task::sleep(100);
    robot.intakeAndMove(fullTile*2.6, 50, 1);
    robot.driveFor(fullTile*-1.5, 50);
    robot.turnFor(.36*fullCircle);
    robot.driveFor(fullTile*1, 50);
    robot.driveFor(fullTile*-.15, 50);
    robot.magazineFor(1700, 1);
    robot.driveFor(fullTile*.1, 50);
    vex::task::sleep(200);
    robot.magazineBack();
    robot.intakeAndMove(fullTile*-1, 50, -1);
  }

  if(redSide)
  {
    robot.intakeAndMove(fullTile*2.6, 50, 1);
    robot.driveFor(fullTile*-1.5, 50);
    robot.turnFor(-.36*fullCircle);
    robot.driveFor(fullTile*.8, 50);
    robot.driveFor(fullTile*-.15, 50);
    robot.magazineFor(1700, 1);
    robot.driveFor(fullTile*.1, 50);
    vex::task::sleep(200);
    robot.magazineBack();
    robot.intakeAndMove(fullTile*-1, 50, -1);
  }
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*                              User Control Task                            */
/*                                                                           */
/*  This task is used to control your robot during the user control phase of */
/*  a VEX Competition.                                                       */
/*                                                                           */
/*  You must modify the code to add your own robot specific commands here.   */
/*---------------------------------------------------------------------------*/

void usercontrol( void ) {
  // User control code here, inside the loop
  while (1) {
    // This is the main execution loop for the user control program.
    // Each time through the loop your program should update motor + servo 
    // values based on feedback from the joysticks.

    // ........................................................................
    // Insert user code here. This is where you use the joystick values to 
    // update your motors, etc.
    // ........................................................................
    robot.drive(controller1.Axis2.value());

    if(controller1.Axis1.value()>10)
      robot.strafe(1);
    else if (controller1.Axis1.value()<-10)
      robot.strafe(-1);
    else
      robot.strafe(0);
      
    if(controller1.ButtonR1.pressing())
    {
      robot.turnRight();
    }
    else if(controller1.ButtonL1.pressing())
      robot.turnLeft();
    
    if(controller1.ButtonX.pressing())
    {
      robot.moveMagazine(1);
    }
    else if(controller1.ButtonB.pressing())
    {
      robot.moveMagazine(-1);
    }
    else {
      robot.moveMagazine(0);
    }

    if(controller1.ButtonRight.pressing())
      robot.intake(1);
    else if(controller1.ButtonDown.pressing())
      robot.intake(-1);
    else
      robot.intake(0);
    
  
    if(controller1.ButtonA.pressing())
    {
        robot.lift(1);
        robot.moveMagazine(.75);
    }
    else if(controller1.ButtonY.pressing())
    {
      robot.lift(-1);
      robot.moveMagazine(-.75);
    }
    else
    {
      robot.lift(0);
    }
    
    vex::task::sleep(20); //Sleep the task for a short amount of time to prevent wasted resources. 
  }
}

//
// Main will set up the competition functions and callbacks.
//
int main() {

    if(controller1.ButtonR2.pressing())
    {
      controller1.ButtonRight.pressed(liftTo);
    }
    //Set up callbacks for autonomous and driver control periods.
    Competition.autonomous( autonomous );
    Competition.drivercontrol( usercontrol );
    
    //Run the pre-autonomous function. 
    pre_auton();
       
    //Prevent main from exiting with an infinite loop.                        
    while(1) {
      vex::task::sleep(100);//Sleep the task for a short amount of time to prevent wasted resources.
    }    
       
}
