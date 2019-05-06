#include "vex.h"
#include "path.h"
#include <vector>
#include <string>
#include <tuple>

#ifndef PATH_GENERATOR_H
#define PATH_GENERATOR_H

class PathGenerator {
	private:
		std::vector<Path> paths;
		double real_slope_field [18][18];
    double imag_slope_field [18][18];
    double endX, endY;
    bool generated;

	public:
		PathGenerator();
		PathGenerator & addPath(Path path);
    PathGenerator & setEndPoint(double x, double y);
		void generate();
    bool isGenerated();
		void clear();
		double getAngleAt(double x, double y);
    double getEndX();
    double getEndY();
};

#endif