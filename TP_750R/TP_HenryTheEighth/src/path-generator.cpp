#include "vex.h"
#include "path-generator.h"
#include <vector>
#include <cmath>
#include <string>
#include <tuple>

PathGenerator::PathGenerator() : paths() {
	for(int i = 0; i < 18; i++)
		for(int j = 0; j < 18; j++) {
			real_slope_field[i][j] = 0.0;
      imag_slope_field[i][j] = 0.0;
    }
}

PathGenerator & PathGenerator::addPath(Path path) {
	paths.push_back(path);
	return *this;
}

PathGenerator & PathGenerator::setEndPoint(double x, double y) {
	endX = x;
  endY = y;
	return *this;
}

double PathGenerator::getEndX() {return endX;}
double PathGenerator::getEndY() {return endY;}

void PathGenerator::generate() {
  if(!generated) {
    generated = true;
	  for(int i = 0; i < paths.size(); i++) {
		  Path path = paths.at(i);
		  
      if(path.getType().compare("straight") == 0) {
			  int fromX = floor(path.getX1() * 3);
  			int fromY = floor(path.getY1() * 3);
	  		int toX = floor(path.getX2() * 3);
		  	int toY = floor(path.getY2() * 3);
			
			  double dx = toX - fromX;
  			double sx = (dx < 0 ? -1 : 1);
	  		double dy = toY - fromY;
		  	double sy = (dy < 0 ? -1 : 1);

        double real_comp = dx / sqrt(dx * dx + dy * dy);
        double imag_comp = dy / sqrt(dx * dx + dy * dy);
			
  			if(dx == 0 && dy == 0) {
	  			real_slope_field[fromX][fromY] = real_comp;
          imag_slope_field[fromX][fromY] = imag_comp;
		  	} else {
			  	if(fabs(dy) < fabs(dx)) {
				  	double slope = dy / dx;
					  double pitch = fromY - slope * fromX;
					  while(fromX != toX) {
						  real_slope_field[fromX][(int)(slope * fromX + pitch)] = real_comp;
              imag_slope_field[fromX][(int)(slope * fromX + pitch)] = imag_comp;
						  fromX += sx;
  					}
	  			} else {
		  			double slope = dx / dy;
			  		double pitch = fromX - slope * fromY;
				  	while(fromY != toY) {
					  	real_slope_field[(int)(slope * fromY + pitch)][fromY] = real_comp;
              imag_slope_field[(int)(slope * fromY + pitch)][fromY] = imag_comp;
						  fromY += sy;
					  }
				  }
			  }
			
			  real_slope_field[toX][toY] = real_comp;
        imag_slope_field[toX][toY] = imag_comp;
			
  		} else if(path.getType().compare("curved") == 0) {
	  		double centerX = path.getX1();
		  	double centerY = path.getY1();
			  double fromA = path.getX2();
			  double toB = path.getY2();
			
		  }
  	}
  }
}


/*

Path Description Formatting Guide

Should follow 

"straight [fromX] [fromY] [toX] [toY]"
				            OR
"curved [centerX] [centerY] [fromA] [toB]"

*/

bool PathGenerator::isGenerated() {
  return generated;
}

void PathGenerator::clear() {
	paths.clear();
	for(int i = 0; i < 18; i++)
		for(int j = 0; j < 18; j++) {
			real_slope_field[i][j] = 0.0;
      imag_slope_field[i][j] = 0.0;
    }
}

double PathGenerator::getAngleAt(double x, double y) {
	int xIndex = floor(x * 3);
	int yIndex = floor(y * 3);
	return atan2(imag_slope_field[xIndex][yIndex], real_slope_field[xIndex][yIndex]) * 180 / 3.14159;
}