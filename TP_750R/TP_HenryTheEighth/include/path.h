#include "vex.h"
#include <string>

#ifndef PATH_H
#define PATH_H

class Path {
	private:
		std::string pathType;
		double x1, y1, x2, y2;
		
	public:
		Path(const char * type, double x1, double y1, double x2, double y2);
		std::string getType();
		double getX1();
		double getY1();
		double getX2();
		double getY2();
};

#endif