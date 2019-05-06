#include "vex.h"
#include "robot.h"
#include "config.h"
#include "auton_selector.h"
#include <vector>
#include <cmath>

Robot::Robot(vex::brain cortex) : cortex(cortex), driveMotors(), roller(INTAKE_PORT),
			 flywheel(FLYWHEEL_PORT), indexer(INDEXER_PORT), capLift(LIFT_PORT, true), lineSensor(cortex.ThreeWirePort.C), g(cortex.ThreeWirePort.F),
			 autonIndex(0), x(0.0), y(0.0), angle(0.0), flywheelSpeed(180.0), driveApprox(-0.75), dsFlag(false) {
	driveMotors.push_back(vex::motor(D_MOTOR_FL, false));
	driveMotors.push_back(vex::motor(D_MOTOR_FR, true));
	driveMotors.push_back(vex::motor(D_MOTOR_BL, false));
	driveMotors.push_back(vex::motor(D_MOTOR_BR, true));
  startingLineValue = lineSensor.value(vex::percentUnits::pct);
}

void Robot::drive(int x, int y) {
  
	for(int i = 0; i < driveMotors.size(); i++) {
		vex::motor m = driveMotors.at(i);
		if(i % 2 == 0) {
			m.spin(vex::directionType::fwd, y + x, vex::percentUnits::pct);
		} else {
			m.spin(vex::directionType::fwd, y - x, vex::percentUnits::pct);
		}
	}

}

void Robot::intake(int mode) {
	roller.spin(vex::directionType::fwd, 100 * mode, vex::percentUnits::pct);
  if(lineSensor.value(vex::percentUnits::pct) >= startingLineValue)
    indexer.spin(vex::directionType::fwd, 100 * mode, vex::percentUnits::pct);
  else
    indexer.stop(vex::brakeType::brake);
}

void Robot::lift(int mode) {
  if(mode != INTAKE_MODE_STOP)
	  capLift.spin(vex::directionType::fwd, 75 * mode, vex::percentUnits::pct);
  else
    capLift.stop(vex::brakeType::hold);
}

void Robot::engageFlywheel(bool engage) {
	indexer.spin(vex::directionType::fwd, (engage ? 200 : 0), vex::percentUnits::pct);
}

double Robot::getX() { return x; }
double Robot::getY() { return y; }
double Robot::getAngle() { return angle; }
double Robot::getFlywheelSpeed() { return flywheelSpeed; }
double Robot::getDriveApprox() { return driveApprox; }

void Robot::setX(double newX) { x = newX; }
void Robot::setY(double newY) { y = newY; }
void Robot::setAngle(double newAngle) { angle = newAngle; }
void Robot::setFlywheelSpeed(double newSpeed) { flywheelSpeed = newSpeed; }
void Robot::setDriveApprox(double newApprox) { driveApprox = newApprox; }

void Robot::gyroTurn(double angle) {
    
    double t = 0;

    //ENDPOINT defines the final value which the gyro should
    //report after it turns through the desired angle
    const double SCALE_FACTOR = 1.88;
    const double ENDPOINT = g.value(vex::rotationUnits::deg) * SCALE_FACTOR + angle;
    
    double currentValue = g.value(vex::rotationUnits::deg) * SCALE_FACTOR;
    double currentError = ENDPOINT - currentValue;
    double previousError = 0.00;
    double totalError = 0.00;
    const double INTEGRAL_LIMIT = 5.0;
    
    //While loop ensures that the robot will keep
    //turning until it reaches the endpoint
    while(fabs(currentError) > 0.75 && (t < 1500 * fabs(angle) / 90)) {
        
        if(fabs(currentError) < INTEGRAL_LIMIT)
            totalError += currentError;
        else
            totalError = 0;
        
        if(previousError * currentError <= 0) totalError = 0;
        
        //double kP = 1.000; //0.50 //KU = 4.00 //TU = 0.93
        //double kI = 0.200; //0.03
        //double kD = 7.000; //0.74

        double kP = 0.850;
        double kI = 0.000;
        double kD = 5.000;
        
        double p = kP * currentError;
        double i = kI * totalError;
        double d = kD * (currentError - previousError);
        
        driveMotors.at(0).spin(vex::directionType::fwd, p + i + d, vex::velocityUnits::pct);
        driveMotors.at(1).spin(vex::directionType::rev, p + i + d, vex::velocityUnits::pct);
        driveMotors.at(2).spin(vex::directionType::fwd, p + i + d, vex::velocityUnits::pct);
        driveMotors.at(3).spin(vex::directionType::rev, p + i + d, vex::velocityUnits::pct);
        
        vex::task::sleep(5);
        
        currentValue = g.value(vex::rotationUnits::deg) * SCALE_FACTOR;
        previousError = currentError;
        currentError = ENDPOINT - currentValue;
        
        t += 5;
    }
    
    //Once turning is complete, stop all motors by
    //braking to prevent overturning
    for(int i = 0; i < driveMotors.size(); i++) {
        vex::motor m = driveMotors.at(i);
        m.stop(vex::brakeType::brake);
    }
    
}

void Robot::pidDriveFor(double distance, double limitSpeed) {
    
  const double ENDPOINT = driveMotors.at(0).rotation(vex::rotationUnits::deg) + distance;

  double currentError = ENDPOINT - driveMotors.at(0).rotation(vex::rotationUnits::deg);

  while(fabs(currentError) > 10) {
    double kP = 0.160;

    double p = currentError * kP;

    if(p > limitSpeed) p = limitSpeed;
    if(p < -limitSpeed) p = -limitSpeed;

    driveMotors.at(0).spin(vex::directionType::fwd, p, vex::percentUnits::pct);
    driveMotors.at(1).spin(vex::directionType::fwd, p, vex::percentUnits::pct);
    driveMotors.at(2).spin(vex::directionType::fwd, p, vex::percentUnits::pct);
    driveMotors.at(3).spin(vex::directionType::fwd, p, vex::percentUnits::pct);

    currentError = ENDPOINT - driveMotors.at(0).rotation(vex::rotationUnits::deg);
  }

  driveMotors.at(0).stop(vex::brakeType::brake);
  driveMotors.at(1).stop(vex::brakeType::brake);
  driveMotors.at(2).stop(vex::brakeType::brake);
  driveMotors.at(3).stop(vex::brakeType::brake);
    
}

/*
void Robot::selectAuton() {
  const int BORDER_WIDTH = 12;
  const int RECT_WIDTH = 105;
  const int RECT_HEIGHT = 105;

  vex::brain::lcd screen = cortex.Screen;
  screen.clearScreen();
  for(int i = 0; i < 4; i++) {
    screen.drawRectangle(BORDER_WIDTH + (RECT_WIDTH + BORDER_WIDTH) * i, BORDER_WIDTH, RECT_WIDTH, RECT_HEIGHT, vex::color::red);
  }
  for(int i = 0; i < 4; i++) {
    screen.drawRectangle(BORDER_WIDTH + (RECT_WIDTH + BORDER_WIDTH) * i, BORDER_WIDTH + RECT_HEIGHT + BORDER_WIDTH, RECT_WIDTH, RECT_HEIGHT, vex::color::blue);
  }

  screen.printAt(BORDER_WIDTH + 0.25 * RECT_WIDTH, 5 * BORDER_WIDTH, "FRONT");
  screen.printAt(BORDER_WIDTH + 0.30 * RECT_WIDTH, 6.5 * BORDER_WIDTH, "PLAT");
  screen.printAt(2 * BORDER_WIDTH + 1.25 * RECT_WIDTH, 5 * BORDER_WIDTH, "FRONT");
  screen.printAt(2 * BORDER_WIDTH + 1.2 * RECT_WIDTH, 6.5 * BORDER_WIDTH, "NO PLAT");
  screen.printAt(3 * BORDER_WIDTH + 2.30 * RECT_WIDTH, 5 * BORDER_WIDTH, "BACK");
  screen.printAt(3 * BORDER_WIDTH + 2.2 * RECT_WIDTH, 6.5 * BORDER_WIDTH, "DESCORE");
  screen.printAt(4 * BORDER_WIDTH + 3.30 * RECT_WIDTH, 5 * BORDER_WIDTH, "BACK");
  screen.printAt(4 * BORDER_WIDTH + 3.10 * RECT_WIDTH, 6.5 * BORDER_WIDTH, "MID FLAGS");
  screen.printAt(BORDER_WIDTH + 0.25 * RECT_WIDTH, 6 * BORDER_WIDTH + RECT_HEIGHT, "FRONT");
  screen.printAt(BORDER_WIDTH + 0.30 * RECT_WIDTH, 7.5 * BORDER_WIDTH + RECT_HEIGHT, "PLAT");
  screen.printAt(2 * BORDER_WIDTH + 1.25 * RECT_WIDTH, 6 * BORDER_WIDTH + RECT_HEIGHT, "FRONT");
  screen.printAt(2 * BORDER_WIDTH + 1.2 * RECT_WIDTH, 7.5 * BORDER_WIDTH + RECT_HEIGHT, "NO PLAT");
  screen.printAt(3 * BORDER_WIDTH + 2.30 * RECT_WIDTH, 6 * BORDER_WIDTH + RECT_HEIGHT, "BACK");
  screen.printAt(3 * BORDER_WIDTH + 2.2 * RECT_WIDTH, 7.5 * BORDER_WIDTH + RECT_HEIGHT, "DESCORE");
  screen.printAt(4 * BORDER_WIDTH + 3.30 * RECT_WIDTH, 6 * BORDER_WIDTH + RECT_HEIGHT, "BACK");
  screen.printAt(4 * BORDER_WIDTH + 3.10 * RECT_WIDTH, 7.5 * BORDER_WIDTH + RECT_HEIGHT, "MID FLAGS");

  while(!screen.pressing());

  int mx = screen.xPosition();
  int my = screen.yPosition();

  if(my >= BORDER_WIDTH && my <= BORDER_WIDTH + RECT_HEIGHT) {
    if(mx >= BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 0;
      x = 0.5;
      y = 3.5;
    } else if(mx >= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 2;
      x = 0.5;
      y = 3.5;
    } else if(mx >= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 4;
      x = 0.5;
      y = 1.5;
    } else if(mx >= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 6;
      x = 0.5;
      y = 1.5;
    }
  } else if(my >= BORDER_WIDTH + RECT_HEIGHT + BORDER_WIDTH && my <= BORDER_WIDTH + RECT_HEIGHT + BORDER_WIDTH + RECT_HEIGHT) {
    if(mx >= BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 1;
      x = 5.5;
      y = 3.5;
    } else if(mx >= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 3;
      x = 5.5;
      y = 3.5;
    } else if(mx >= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 5;
      x = 5.5;
      y = 1.5;
    } else if(mx >= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH && mx <= BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH + BORDER_WIDTH + RECT_WIDTH) {
      autonIndex = 7;
      x = 5.5;
      y = 1.5;
    }
  }

  screen.clearScreen();

}
*/

void Robot::selectAuton() {
  AutonSelector selector(cortex);
  selector.addRedOption("Front - Plat", 0)
          .addRedOption("Front - No Plat", 2)
          .addRedOption("Back - Descore", 4)
          .addRedOption("Back - Mid Flag", 6)
          .addRedOption("Front - Deny", 8)
          .addBlueOption("Front - Plat", 1)
          .addBlueOption("Front - No Plat", 3)
          .addBlueOption("Back - Descore", 5)
          .addBlueOption("Back - Mid Flag", 7)
          .addBlueOption("Front - Deny", 9);
  
  autonIndex = selector.getCode();
}

void Robot::auton() {
  const double FULL_TILE = 660;
  const double FULL_CIRCLE = 1075;

  //autonIndex = 5;
  
  switch(autonIndex){
    case -1:
      resetGyro();
      gyroTurn(90);
      vex::controller(vex::controllerType::primary).Screen.setCursor(1, 1);
      vex::controller(vex::controllerType::primary).Screen.print("DONE");
      break;
    case 0: //FR - PLAT
      setFlywheelSpeed(195);
      resetGyro();
      pidDriveFor(1.65 * FULL_TILE, 100);
      autoIntake();
      driveFor(-1.790 * FULL_TILE, 55);
      gyroTurn(-103);
      driveFor(0.37 * FULL_TILE, 30);
      doubleShot(1);
      driveFor(-1.54 * FULL_TILE, 80);
      gyroTurn(98);
      roller.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
      driveFor(2.15 * FULL_TILE, 100);
      break;
    case 1: //FB - PLAT
      setFlywheelSpeed(180);
      resetGyro();
      driveFor(1.8 * FULL_TILE, 50);
      autoIntake();
      driveFor(-1.970 * FULL_TILE, 50);
      gyroTurn(101);
      driveFor(0.37 * FULL_TILE, 30);
      doubleShot(2);
      driveFor(-1.49 * FULL_TILE, 80);
      gyroTurn(-90);
      roller.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
      driveFor(2.15 * FULL_TILE, 100);
      break;
    case 2: //FR - NO PLAT
      setFlywheelSpeed(195);
      resetGyro();
      pidDriveFor(1.65 * FULL_TILE, 100);
      roller.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
      vex::task::sleep(1000);
      driveFor(-1.870 * FULL_TILE, 70);
      roller.stop();
      gyroTurn(-97);
      //driveFor(0.200 * FULL_TILE, 50);
      doubleShot(2);
      gyroTurn(-14.5);
      roller.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
      driveFor(1.95 * FULL_TILE, 80);
      roller.stop();
      vex::task::sleep(500);
      driveFor(-1.10 * FULL_TILE, 80);
      gyroTurn(120);
      autoFlip(1.25 * FULL_TILE, 70);
      
      break;
    case 3: //FB - NO PLAT
      setFlywheelSpeed(200);
      resetGyro();
      driveFor(1.73 * FULL_TILE, 70);
      autoIntake();
      driveFor(-1.910 * FULL_TILE, 70);
      gyroTurn(96);
      driveFor(0.460 * FULL_TILE, 50);
      modifiedDoubleShot(1.5);
      gyroTurn(17.5);
      roller.spin(vex::directionType::fwd, 100, vex::percentUnits::pct);
      driveFor(1.8 * FULL_TILE, 100);
      roller.stop();
      driveFor(-1.20 * FULL_TILE, 100);
      gyroTurn(-110);
      autoFlip(1.25 * FULL_TILE, 80);
      break;
    case 4: //BR - DESCORE
      setFlywheelSpeed(190);
      resetGyro();
      driveFor(1.8 * FULL_TILE, 60);
      autoIntake();
      gyroTurn(-94);
      driveFor(-0.32 * FULL_TILE, 40);
      liftTo(60);
      liftTo(0);
      vex::task::sleep(500);
      driveFor(-0.18 * FULL_TILE, 40);
      liftTo(60);
      driveFor(0.28 * FULL_TILE, 40);
      gyroTurn(20.5);
      autoShoot(1.5);
      liftTo(120);
      break;
    case 5: //BB - DESCORE
      setFlywheelSpeed(200);
      vex::task::sleep(1000);
      resetGyro();
      pidDriveFor(1.70 * FULL_TILE, 60);
      autoIntake();
      gyroTurn(68);
      autoShoot(1.5);
      gyroTurn(-68);
      driveFor(-0.42 * FULL_TILE, 30);
      gyroTurn(108);
      driveFor(-0.32 * FULL_TILE, 30);
      driveFor(1.88 * FULL_TILE, 100);
      break;
    case 6: //BR - MID FLAGS
      setFlywheelSpeed(200);
      vex::task::sleep(1000);
      resetGyro();
      pidDriveFor(1.67 * FULL_TILE, 100);
      autoIntake();
      driveFor(-0.34 * FULL_TILE, 30);
      gyroTurn(-105);
      driveFor(0.32 * FULL_TILE, 30);
      driveFor(-0.15 * FULL_TILE, 30);
      gyroTurn(15);
      modifiedDoubleShot(1.5);
      //doubleShot(0.5);
      gyroTurn(-15);
      driveFor(1.55 * FULL_TILE, 100);
      break;
    case 7: //BB - MID FLAGS
      setFlywheelSpeed(200);
      vex::task::sleep(1000);
      resetGyro();
      pidDriveFor(1.70 * FULL_TILE, 100);
      autoIntake();
      driveFor(-0.44 * FULL_TILE, 50);
      gyroTurn(110);
      driveFor(0.32 * FULL_TILE, 30);
      driveFor(-0.20 * FULL_TILE, 30);
      gyroTurn(-19);
      autoShoot(1.5);
      gyroTurn(19);
      driveFor(1.6 * FULL_TILE, 100);
      break;
    case 8:
      setFlywheelSpeed(180);
      resetGyro();
      gyroTurn(-45);
      autoFlip(1.75 * FULL_TILE, 70);
      driveFor(-1.75 * FULL_TILE, 70);
      gyroTurn(45);
      driveFor(1.8 * FULL_TILE, 60);
      autoIntake();
      driveFor(-1.970 * FULL_TILE, 60);
      gyroTurn(-98);
      modifiedDoubleShot(1.5);
      break;
    case 9:
      setFlywheelSpeed(190);
      resetGyro();
      gyroTurn(55);
      autoFlip(1.3 * FULL_TILE, 60);
      gyroTurn(-98);
      driveFor(1.4 * FULL_TILE, 60);
      autoIntake();
      gyroTurn(28);
      driveFor(-2.1 * FULL_TILE, 80);
      gyroTurn(102);
      driveFor(0.3 * FULL_TILE, 40);
      doubleShot(1.5);
      /*
      gyroTurn(45);
      autoFlip(1.50 * FULL_TILE, 60);
      driveFor(-1.50 * FULL_TILE, 60);
      gyroTurn(-45);
      driveFor(1.8 * FULL_TILE, 60);
      autoIntake();
      driveFor(-1.970 * FULL_TILE, 60);
      gyroTurn(110);
      driveFor(1.7 * FULL_TILE, 60);
      vex::task::sleep(500);
      driveFor(-1.34 * FULL_TILE, 100);
      gyroTurn(-10.5);
      modifiedDoubleShot(1.5);
      */
      break;
  }
}

void Robot::driveFor(double distance, double velocity) {

  driveMotors.at(0).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(2).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(3).rotateFor(vex::directionType::fwd, distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);
  
  vex::task::sleep(100);
}

void Robot::autoFlip(double distance, double velocity) {
  roller.spin(vex::directionType::rev, 100, vex::percentUnits::pct);

  driveMotors.at(0).rotateFor(distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(2).rotateFor(distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(3).rotateFor(distance, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);

  roller.stop();
  vex::task::sleep(100);
}

void Robot::wheelie(double distance) {
  const double FULL_TILE = 660;

  driveMotors.at(0).rotateFor(0.50 * FULL_TILE, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(0.50 * FULL_TILE, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  driveMotors.at(2).rotateFor(0.50 * FULL_TILE, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  driveMotors.at(3).rotateFor(0.50 * FULL_TILE, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);

  driveMotors.at(0).stop(vex::brakeType::brake);
  driveMotors.at(1).stop(vex::brakeType::brake);
  driveMotors.at(2).stop(vex::brakeType::brake);
  driveMotors.at(3).stop(vex::brakeType::brake);

  driveMotors.at(0).rotateFor(-distance, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(-distance, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  driveMotors.at(2).rotateFor(-distance, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, false);
  driveMotors.at(3).rotateFor(-distance, vex::rotationUnits::deg, 100, vex::velocityUnits::pct, true);
}

void Robot::cwTurn(double angle, double velocity) {

  driveMotors.at(0).rotateFor(angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(-angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(2).rotateFor(angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(3).rotateFor(-angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);

  vex::task::sleep(100);
}

void Robot::ccwTurn(double angle, double velocity) {

  driveMotors.at(0).rotateFor(-angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(1).rotateFor(angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(2).rotateFor(-angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, false);
  driveMotors.at(3).rotateFor(angle, vex::rotationUnits::deg, velocity, vex::velocityUnits::pct, true);

  vex::task::sleep(100);
}

void Robot::autoIntake() {
  roller.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  vex::task::sleep(1000);
  roller.stop();

  vex::task::sleep(100);
}

void Robot::autoShoot(double time) {
  roller.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  indexer.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  vex::task::sleep(time * 1000);
  roller.stop();
  indexer.stop();

  vex::task::sleep(100);
}

void Robot::doubleShot(double delay) {
  dsFlag = true;
  roller.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  indexer.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  vex::task::sleep(500);
  roller.stop();
  indexer.stop();
  vex::task::sleep(delay * 1000);
  roller.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  indexer.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  vex::task::sleep(750);
  roller.stop();
  indexer.stop();
  dsFlag = false;

  vex::task::sleep(100);
}

void Robot::modifiedDoubleShot(double delay) {
  dsFlag = true;
  roller.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  indexer.spin(vex::directionType::fwd, 100, vex::velocityUnits::pct);
  vex::task::sleep(delay * 1000);
  roller.stop();
  indexer.stop();
  dsFlag = false;

  vex::task::sleep(100);
}

void Robot::liftTo(double angle) {
  capLift.rotateTo(angle * 3 * 210 / 90, vex::rotationUnits::deg, 30, vex::velocityUnits::pct, true);

  vex::task::sleep(100);
}

vex::motor Robot::getFlywheelMotor() {
	return flywheel;
}

/*
void Robot::startFlywheel() {
  vex::task flywheelTask([](void) -> int {
    vex::motor flywheel = robot.getFlywheelMotor();

    while(true) {
      flywheel.spin(vex::directionType::fwd, robot.getFlywheelSpeed(), vex::velocityUnits::rpm);
      vex::task::sleep(100);
    }

    return 0;
  });
}
*/

bool Robot::getDSFlag() {
  return dsFlag;
}

///*
void Robot::startFlywheel() {
	
  vex::task flywheelTask([](void) -> int {
    vex::motor flywheel = robot.getFlywheelMotor();
    flywheel.spin(vex::directionType::fwd);

    //ENDPOINT defines the final value which the gyro should
    //report after it turns through the desired angle
    
    double currentValue = flywheel.velocity(vex::velocityUnits::rpm);
    double currentError = robot.getFlywheelSpeed() - currentValue;
    double previousError = 0.00;
    double totalError = 0.00;
    
    const double LO_INTEGRAL_LIMIT = 100.0;
    const double HI_INTEGRAL_LIMIT = 20.0;

    //While loop ensures that the robot will keep
    //turning until it reaches the endpoint
    while(true) {

        if(robot.getDSFlag()) {
          flywheel.stop(vex::brakeType::coast);
        } else {
          flywheel.spin(vex::directionType::fwd);
          if(/*fabs(currentError) < LO_INTEGRAL_LIMIT &&*/ fabs(currentError) > HI_INTEGRAL_LIMIT)
              totalError += currentError;
          else
              totalError = 0;
        
          double kP = 2.250;
          double kI = 4.000;
          double kD = 10.000;
        
          double p = kP * currentError;
          double i = kI * totalError;
          double d = kD * (currentError - previousError);

          double command = -(p + i + d);
          if(command > 0) command = 0;
        
          flywheel.setVelocity(command, vex::velocityUnits::rpm);
        
          currentValue = flywheel.velocity(vex::velocityUnits::rpm);
          previousError = currentError;
          currentError = robot.getFlywheelSpeed() - currentValue;
        
          vex::task::sleep(20);
          //vex::controller(vex::controllerType::primary).Screen.clearScreen();
          //vex::controller(vex::controllerType::primary).Screen.setCursor(1, 1);
          //vex::controller(vex::controllerType::primary).Screen.print("%lf", currentValue);
        }
    }

    return 0;
  });

}
//*/
/*
void Robot::startFlywheel() {
  vex::task flywheelTask([](void) -> int {
    vex::motor flywheel = robot.getFlywheelMotor();
    flywheel.spin(vex::directionType::fwd);

    double currentError = robot.getFlywheelSpeed() - flywheel.velocity(vex::velocityUnits::rpm);
    double previousError = 0.0;

    double totalError = 0.0;
    bool firstCrossing = true;
    double previousTotalError;

    while(true) {
      currentError = robot.getFlywheelSpeed() - flywheel.velocity(vex::velocityUnits::rpm);
      
      double kI = 0.0025;
      totalError += kI * currentError;

      if(totalError < -1) totalError = -1;
      if(totalError > 0) totalError = 0;

      flywheel.setVelocity(200.0 * totalError, vex::velocityUnits::rpm);
      
      if(currentError * previousError < 0) {
        if(!firstCrossing) {
          double replacement = (totalError + previousTotalError) / 2;
          totalError = replacement;
          previousTotalError = replacement;
        } else {
          totalError = robot.getDriveApprox();
          previousTotalError = robot.getDriveApprox();
        }
      }

      previousError = currentError;

      vex::task::sleep(20);
      //vex::controller(vex::controllerType::primary).Screen.setCursor(1, 1);
      //vex::controller(vex::controllerType::primary).Screen.print("%lf : %lf", totalError, currentError);
    }

    return 0;
  });

}
//*/

/*
void Robot::startFlywheel() {
  flywheel.spin(vex::directionType::rev, flywheelSpeed, vex::velocityUnits::rpm);
}
//*/

void Robot::startFieldPosition() {
	vex::task fieldPositionTask([](void) -> int {
		const double FULL_TILE = 660;
		const double FULL_CIRCLE = 1200;
		
		while(true) {
      double fl = robot.getMotorAt(0).rotation(vex::rotationUnits::deg);
			double fr = robot.getMotorAt(1).rotation(vex::rotationUnits::deg);
			
			double d = (fl + fr) / (2 * FULL_TILE);
			double turns = (fl - fr) / (2 * FULL_CIRCLE);
			
			if(!(d == 0 && turns == 0)) {
			  robot.setAngle(robot.getAngle() + turns * 360);
        robot.setAngle((int)robot.getAngle() % 360);
				robot.setX(robot.getX() + d * cos(robot.getAngle()));
				robot.setY(robot.getY() + d * sin(robot.getAngle()));
				
				//robot.tareDriveMotors();
			}
			
			vex::task::sleep(50);
		}
	});
}

void Robot::tareDriveMotors() {
  for(int i = 0; i < driveMotors.size(); i++) {
    driveMotors.at(i).setRotation(0, vex::rotationUnits::deg);
  }
}

vex::motor Robot::getMotorAt(int index) {
	return driveMotors.at(index);
}

void Robot::resetGyro() {
  g.startCalibration(1000);
  vex::task::sleep(1000);
}

double Robot::getGyro() {
  return g.value(vex::rotationUnits::deg);
}

double Robot::getLineSensor() {
  return lineSensor.value(vex::percentUnits::pct);
}

void Robot::followPath(PathGenerator generator) {
	if(generator.isGenerated()) {
    //turnTo(generator.getAngleAt(x, y), 70);

    driveMotors.at(0).setVelocity(90, vex::percentUnits::pct);
    driveMotors.at(1).setVelocity(90, vex::percentUnits::pct);
    driveMotors.at(2).setVelocity(90, vex::percentUnits::pct);
    driveMotors.at(3).setVelocity(90, vex::percentUnits::pct);

    driveMotors.at(0).spin(vex::directionType::fwd);
    driveMotors.at(1).spin(vex::directionType::fwd);
    driveMotors.at(2).spin(vex::directionType::fwd);
    driveMotors.at(3).spin(vex::directionType::fwd);

    bool done = false;
    while(!done) {
      double targetAngle = generator.getAngleAt(x, y);
      if(targetAngle != angle) {
        driveMotors.at(0).setVelocity(driveMotors.at(0).velocity(vex::velocityUnits::pct) + 0.10 * (angle - targetAngle), vex::percentUnits::pct);
        driveMotors.at(1).setVelocity(driveMotors.at(1).velocity(vex::velocityUnits::pct) - 0.10 * (angle - targetAngle), vex::percentUnits::pct);
        driveMotors.at(2).setVelocity(driveMotors.at(2).velocity(vex::velocityUnits::pct) + 0.10 * (angle - targetAngle), vex::percentUnits::pct);
        driveMotors.at(3).setVelocity(driveMotors.at(3).velocity(vex::velocityUnits::pct) - 0.10 * (angle - targetAngle), vex::percentUnits::pct);
      }

      if(fabs(x - generator.getEndX()) < 0.75 && fabs(y - generator.getEndY()) < 0.75)
        done = true;

      vex::task::sleep(20);
    }
  }
}
