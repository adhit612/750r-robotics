#include "main.h"
#include "config.h"

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
 int autonIndex;
void competition_initialize() {
	pros::Controller primary = pros::Controller(pros::E_CONTROLLER_MASTER);
	
	primary.clear();
	
	primary.print(1, 0, "Auton Selector");
	pros::delay(1000);
	primary.clear();
	
	primary.print(0, 0, "> FR - NO PLAT");
	primary.print(1, 0, "FB - NO PLAT");
	primary.print(2, 0, "FR - PLAT");
	
	int currentSelection = 0;
	bool redraw = false;
	bool done = false;
	
	while(done) {
		if(primary.get_digital(DIGITAL_UP)) {
			if(currentSelection > 0) {
				currentSelection--;
				redraw = true;
			} else {
				primary.rumble("-");
			}
		} else if(primary.get_digital(DIGITAL_DOWN)) {
			if(currentSelection < 7) {
				currentSelection++;
				redraw = true;
			} else {
				primary.rumble("-");
			}
		} else if(primary.get_digital(DIGITAL_A)) {
			primary.clear();
			primary.print(0, 0, "   YOU CHOSE   ");
			switch(currentSelection) {
				case 0:
					primary.print(1, 0, "FR - NO PLAT");
					break;
				case 1:
					primary.print(1, 0, "FB - NO PLAT");
					break;
				case 2:
					primary.print(1, 0, "FR - PLAT");
					break;
				case 3:
					primary.print(1, 0, "FB - PLAT");
					break;
				case 4:
					primary.print(1, 0, "BR - DESCORE");
					break;
				case 5:
					primary.print(1, 0, "BB - DESCORE");
					break;
				case 6:
					primary.print(1, 0, "BR - MID FLAG");
					break;
				case 7:
					primary.print(1, 0, "BB - MID FLAG");
					break;
			}
			primary.print(2, 0, "   >NO   YES   ");
			
			bool confirm = false;
			bool redraw2 = false;
			
			while(true) {
				if(primary.get_digital(DIGITAL_LEFT)) {
					if(confirm) {
						confirm = false;
						redraw2 = true;
					} else {
						primary.rumble("-");
					}
				} else if(primary.get_digital(DIGITAL_RIGHT)) {
					if(!confirm) {
						confirm = true;
						redraw2 = true;
					} else {
						primary.rumble("-");
					}
				} else if(primary.get_digital(DIGITAL_A)) {
					break;
				}
				
				if(redraw2) {
					if(confirm) {
						primary.print(2, 0, "    NO  >YES   ");
					} else {
						primary.print(2, 0, "   >NO   YES   ");
					}
				}
			}
			
			done = confirm; 
		}
		
		if(redraw) {
			redraw = false;
			switch(currentSelection) {
				case 0:
					primary.clear();
					
					primary.print(0, 0, "> FR - NO PLAT");
					primary.print(1, 0, "FB - NO PLAT");
					primary.print(2, 0, "FR - PLAT");
					break;
				case 1:
					primary.clear();
					
					primary.print(0, 0, "FR - NO PLAT");
					primary.print(1, 0, "> FB - NO PLAT");
					primary.print(2, 0, "FR - PLAT");
					break;
				case 2:
					primary.clear();
					
					primary.print(0, 0, "FB - NO PLAT");
					primary.print(1, 0, "> FR - PLAT");
					primary.print(2, 0, "FB - PLAT");
					break;
				case 3:
					primary.clear();
					
					primary.print(0, 0, "FR - PLAT");
					primary.print(1, 0, "> FB - PLAT");
					primary.print(2, 0, "BR - DESCORE");
					break;
				case 4:
					primary.clear();
					
					primary.print(0, 0, "FB - PLAT");
					primary.print(1, 0, "> BR - DESCORE");
					primary.print(2, 0, "BB - DESCORE");
					break;
				case 5:
					primary.clear();
					
					primary.print(0, 0, "BR - DESCORE");
					primary.print(1, 0, "> BB - DESCORE");
					primary.print(2, 0, "BR - MID FLAG");
					break;
				case 6:
					primary.clear();
					
					primary.print(0, 0, "BB - DESCORE");
					primary.print(1, 0, "> BR - MID FLAG");
					primary.print(2, 0, "BB - MID FLAG");
					break;
				case 7:
					primary.clear();
					
					primary.print(0, 0, "BB - DESCORE");
					primary.print(1, 0, "BR - MID FLAG");
					primary.print(2, 0, "> BB - MID FLAG");
					break;
			}
		}
	}
	
	autonIndex = currentSelection;
	
}
