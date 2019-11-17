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
  int fullTile=600;
  int fullCircle=1200;


  robot.intakeAndMove(fullTile*3, 100, 1);
  robot.driveFor(fullTile*-3, 100);
  robot.turnFor(.25*fullCircle);
  robot.driveFor(2*fullTile, 100);
  robot.magazineFor(1);
  robot.intakeAndMove(fullTile*-1, 100, -1);
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
    robot.drive(controller1.Axis2.position());

    if(controller1.Axis1.position()>10)
      robot.strafe(-1);
    else if (controller1.Axis1.position()<-10)
      robot.strafe(1);
    else
      robot.strafe(0);
      
    if(controller1.ButtonR1.pressing())
      robot.turnRight();
    else if(controller1.ButtonL1.pressing())
      robot.turnLeft();
    
    if(controller1.ButtonUp.pressing())
    {
      robot.moveMagazine(1);
    }
    else if(controller1.ButtonDown.pressing())
    {
      robot.moveMagazine(-1);
    }
    else {
      robot.moveMagazine(0);
    }

    if(controller1.ButtonA.pressing())
      robot.intake(1);
    else if(controller1.ButtonB.pressing())
      robot.intake(-1);
    else
      robot.intake(0);

    if(controller1.ButtonRight.pressing())
    {
      robot.lift(1);
      //robot.moveMagazine(.5);
    }
    else if(controller1.ButtonLeft.pressing())
    {
      robot.lift(-1);
      //robot.moveMagazine(-.5);
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
