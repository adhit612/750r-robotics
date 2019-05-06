#include "vex.h"
#include <map>

class AutonSelector {
  private:
    vex::brain::lcd screen;
    std::map<char *, int> redOptions;
    std::map<char *, int> blueOptions;
    

  public:
    AutonSelector(vex::brain cortex);
    AutonSelector & addRedOption(char * name, int code);
    AutonSelector & addBlueOption(char * name, int code);
    int getCode();
};