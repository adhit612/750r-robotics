#include "main.h"
#include "path.h"
#include <string>

Path::Path(const char * pathType, double x1, double y1, double x2, double y2) : 
		pathType(pathType), x1(x1), y1(y1), x2(x2), y2(y2) {}

std::string Path::getType() { return pathType; }
double Path::getX1() { return x1; }
double Path::getY1() { return y1; }
double Path::getX2() { return x2; }
double Path::getY2() { return y2; }