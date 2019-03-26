#include "main.h"
#include "config.h"

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	//competition_initialize();
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
 pros::Mutex driveMutex;
 int autonIndex;
void competition_initialize() {
	pros::Controller primary = pros::Controller(pros::E_CONTROLLER_MASTER);
	//pros::delay(50);
	//primary.clear();
	pros::delay(50);
	
	primary.print(0, 0, "> FR - NO PLAT  ");
	pros::delay(50);
	primary.print(1, 0, "FB - NO PLAT    ");
	pros::delay(50);
	primary.print(2, 0, "FR - PLAT		 ");
	pros::delay(50);
	primary.clear_line(0);
	pros::delay(100);
	primary.clear_line(1);
	pros::delay(100);
	primary.clear_line(2);
	pros::delay(100);
	
	
	int currentSelection = 0;
	bool redraw = false;
	bool done = false;
	
	while(!done) {
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
			pros::delay(50);
			primary.clear();
			pros::delay(50);
			primary.print(0, 0, "   YOU CHOSE   	");
			pros::delay(50);
			switch(currentSelection) {
				case 0:
					primary.print(1, 0, "FR - NO PLAT	");
					pros::delay(50);
					break;
				case 1:
					primary.print(1, 0, "FB - NO PLAT	");
					pros::delay(50);
					break;
				case 2:
					primary.print(1, 0, "FR - PLAT		");
					pros::delay(50);
					break;
				case 3:
					primary.print(1, 0, "FB - PLAT		");
					pros::delay(50);
					break;
				case 4:
					primary.print(1, 0, "BR - DESCORE	");
					pros::delay(50);
					break;
				case 5:
					primary.print(1, 0, "BB - DESCORE	");
					pros::delay(50);
					break;
				case 6:
					primary.print(1, 0, "BR - MID FLAG	");
					pros::delay(50);
					break;
				case 7:
					primary.print(1, 0, "BB - MID FLAG	");
					pros::delay(50);
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
					done = confirm; 
					break;
				}
				
				if(redraw2) {
					if(confirm) {
						primary.print(2, 0, "    NO  >YES   ");
						pros::delay(50);
					} else {
						primary.print(2, 0, "   >NO   YES   ");
						pros::delay(50);
					}
				}
			}
			
		}
		
		if(redraw) {
			redraw = false;
			//pros::delay(50);
			//primary.clear();	
			switch(currentSelection) {
				case 0:
					pros::delay(50);
					primary.print(0, 0, "> FR - NO PLAT   ");	
					pros::delay(50);
					primary.print(1, 0, "FB - NO PLAT	  ");	
					pros::delay(50);
					primary.print(2, 0, "FR - PLAT		  ");	
					pros::delay(50);
					break;
				case 1:
					pros::delay(50);
					
					primary.print(0, 0, "FR - NO PLAT	  ");	
					pros::delay(50);
					primary.print(1, 0, "> FB - NO PLAT	  ");	
					pros::delay(50);
					primary.print(2, 0, "FR - PLAT		  ");	
					pros::delay(50);
					break;
				case 2:
					pros::delay(50);
					
					primary.print(0, 0, "FB - NO PLAT	  ");	
					pros::delay(50);
					primary.print(1, 0, "> FR - PLAT	  ");	
					pros::delay(50);
					primary.print(2, 0, "FB - PLAT	 	  ");	
					pros::delay(50);
					break;
				case 3:
					pros::delay(50);
					
					primary.print(0, 0, "FR - PLAT		  ");	
					pros::delay(50);
					primary.print(1, 0, "> FB - PLAT	  ");	
					pros::delay(50);
					primary.print(2, 0, "BR - DESCORE	  ");	
					pros::delay(50);
					break;
				case 4:
					pros::delay(50);
					
					primary.print(0, 0, "FB - PLAT		  ");	
					pros::delay(50);
					primary.print(1, 0, "> BR - DESCORE	  ");	
					pros::delay(50);
					primary.print(2, 0, "BB - DESCORE	  ");	
					pros::delay(50);
					break;
				case 5:	
					pros::delay(50);
					
					primary.print(0, 0, "BR - DESCORE	  ");	
					pros::delay(50);
					primary.print(1, 0, "> BB - DESCORE	  ");	
					pros::delay(50);
					primary.print(2, 0, "BR - MID FLAG	  ");	
					pros::delay(50);
					break;
				case 6:
					pros::delay(50);
					
					primary.print(0, 0, "BB - DESCORE	  ");	
					pros::delay(50);
					primary.print(1, 0, "> BR - MID FLAG  ");	
					pros::delay(50);
					primary.print(2, 0, "BB - MID FLAG	  ");	
					pros::delay(50);
					break;
				case 7:
					pros::delay(50);
					
					primary.print(0, 0, "BB - DESCORE	  ");	
					pros::delay(50);
					primary.print(1, 0, "BR - MID FLAG	  ");	
					pros::delay(50);
					primary.print(2, 0, "> BB - MID FLAG  ");	
					pros::delay(50);
					break;
			}
		}
		pros::delay(50);
	}
	
	autonIndex = currentSelection;
}
