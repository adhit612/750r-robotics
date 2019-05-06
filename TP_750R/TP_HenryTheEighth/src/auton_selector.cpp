#include "vex.h"
#include "auton_selector.h"
#include <map>
#include <vector>

AutonSelector::AutonSelector(vex::brain cortex) : redOptions(), blueOptions() {
  screen = cortex.Screen;
}

AutonSelector & AutonSelector::addRedOption(char * name, int code) {
  redOptions.insert(std::pair<char *, int>(name, code));
  return * this;
}

AutonSelector & AutonSelector::addBlueOption(char * name, int code) {
  blueOptions.insert(std::pair<char *, int>(name, code));
  return * this;
}

int AutonSelector::getCode() {
  const double SCREEN_WIDTH = 480;
  const double SCREEN_HEIGHT = 272;

  int code = 0;

  screen.clearScreen();

  screen.drawRectangle(0, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, vex::color::red);
  screen.drawRectangle(SCREEN_WIDTH / 2, 0, SCREEN_WIDTH / 2, SCREEN_HEIGHT, vex::color::blue);

  screen.printAt(SCREEN_WIDTH / 4 - screen.getStringWidth("RED") / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight("RED") / 2, "RED");
  screen.printAt(3 * SCREEN_WIDTH / 4 - screen.getStringWidth("BLUE") / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight("BLUE") / 2, "BLUE");

  while(!screen.pressing()) vex::task::sleep(20);
  
  int x1 = screen.xPosition();

  screen.clearScreen();

  screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 - 125, 3 * SCREEN_WIDTH / 4, 70, vex::color::yellow);
  screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 + 55, 3 * SCREEN_WIDTH / 4, 70, vex::color::yellow);

  vex::task::sleep(1000);

  if(x1 < SCREEN_WIDTH / 2) {  //RED

    screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 - 35, 3 * SCREEN_WIDTH / 4, 70, vex::color::red);

    std::vector<char *> names;
    std::vector<int> codes;

    for(std::map<char *, int>::iterator it = redOptions.begin(); it != redOptions.end(); it++ ) {
      names.push_back(it -> first);
      codes.push_back(it -> second);
    }

    screen.printAt(SCREEN_WIDTH / 2 - screen.getStringWidth(names.at(0)) / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight(names.at(0)) / 2, names.at(0));

    bool done = false;

    int selection = 0;

    while(!done) {
      vex::task::sleep(200);

      if(screen.pressing()) {
        int x = screen.xPosition();
        int y = screen.yPosition();

        //vex::controller(vex::controllerType::primary).Screen.clearScreen();
        //vex::controller(vex::controllerType::primary).Screen.print("%d : %d", x, y);

        if(x > SCREEN_WIDTH / 8 && x < SCREEN_WIDTH * 7 / 8) {
          if(y > SCREEN_HEIGHT / 2 - 125 && y < SCREEN_HEIGHT / 2 - 55) { //UP
            if(selection > 0) {
              selection -= 1;
              screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 - 35, 3 * SCREEN_WIDTH / 4, 70, vex::color::red);
              screen.printAt(SCREEN_WIDTH / 2 - screen.getStringWidth(names.at(selection)) / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight(names.at(selection)) / 2, names.at(selection));
              //screen.render();
            }
          } else if(y > SCREEN_HEIGHT / 2 - 35 && y < SCREEN_HEIGHT / 2 + 35) { //SELECT
            code = codes.at(selection);
            screen.clearScreen();
            //vex::controller(vex::controllerType::primary).Screen.clearScreen();
            vex::controller(vex::controllerType::primary).Screen.print(names.at(selection));
            done = true;
          } else if(y > SCREEN_HEIGHT / 2 + 55 && y < SCREEN_HEIGHT / 2 + 125) { // DOWN
            if(selection < names.size() - 1) {
              selection += 1;
              screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 - 35, 3 * SCREEN_WIDTH / 4, 70, vex::color::red);
              screen.printAt(SCREEN_WIDTH / 2 - screen.getStringWidth(names.at(selection)) / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight(names.at(selection)) / 2, names.at(selection));
              //screen.render();
            }
          }
        }

      }
    }

  } else {                    //BLUE
    screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 - 35, 3 * SCREEN_WIDTH / 4, 70, vex::color::blue);

    std::vector<char *> names;
    std::vector<int> codes;

    for(std::map<char *, int>::iterator it = blueOptions.begin(); it != blueOptions.end(); it++ ) {
      names.push_back(it -> first);
      codes.push_back(it -> second);
    }

    screen.printAt(SCREEN_WIDTH / 2 - screen.getStringWidth(names.at(0)) / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight(names.at(0)) / 2, names.at(0));
    
    bool done = false;

    int selection = 0;

    while(!done) {
      if(screen.pressing()) {
        vex::task::sleep(200);

        int x = screen.xPosition();
        int y = screen.yPosition();

        if(x > SCREEN_WIDTH / 8 && x < SCREEN_WIDTH * 7 / 8) {
          if(y > SCREEN_HEIGHT / 2 - 125 && y < SCREEN_HEIGHT / 2 - 55) { //UP
            if(selection > 0) {
              selection -= 1;
              screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 - 35, 3 * SCREEN_WIDTH / 4, 70, vex::color::blue);
              screen.printAt(SCREEN_WIDTH / 2 - screen.getStringWidth(names.at(selection)) / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight(names.at(selection)) / 2, names.at(selection));
              //screen.render();
            }
          } else if(y > SCREEN_HEIGHT / 2 - 35 && y < SCREEN_HEIGHT / 2 + 35) { //SELECT
            code = codes.at(selection);
            screen.clearScreen();
            //vex::controller(vex::controllerType::primary).Screen.clearScreen();
            vex::controller(vex::controllerType::primary).Screen.print(names.at(selection));
            done = true;
          } else if(y > SCREEN_HEIGHT / 2 + 55 && y < SCREEN_HEIGHT / 2 + 125) { // DOWN
            if(selection < names.size() - 1) {
              selection += 1;
              screen.drawRectangle(SCREEN_WIDTH / 8, SCREEN_HEIGHT / 2 - 35, 3 * SCREEN_WIDTH / 4, 70, vex::color::blue);
              screen.printAt(SCREEN_WIDTH / 2 - screen.getStringWidth(names.at(selection)) / 2, SCREEN_HEIGHT / 2 - screen.getStringHeight(names.at(selection)) / 2, names.at(selection));
              //screen.render();
            }
          }
        }

      }
    }
  }

  screen.clearScreen();

  return code;
}