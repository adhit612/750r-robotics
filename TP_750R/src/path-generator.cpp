#include "main.h"
#include "path-generator.h"
#include <vector>
#include <cmath>
#include <string>
#include <tuple>

PathGenerator::PathGenerator() : paths() {
	for(int i = 0; i < 18; i++)
		for(int j = 0; j < 18; j++)
			slope_field[i][j] = 0.0;
}

PathGenerator & PathGenerator::addPath(Path path) {
	paths.push_back(path);
	return *this;
}

void PathGenerator::generate() {
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
			
			double targetAngle = atan2(dy, dx);
			
			if(dx == 0 && dy == 0) {
				slope_field[fromX][fromY] = targetAngle;
			} else {
				if(fabs(dy) < fabs(dx)) {
					double slope = dy / dx;
					double pitch = fromY - slope * fromX;
					while(fromX != toX) {
						slope_field[fromX][(int)(slope * fromX + pitch)] = targetAngle;
						fromX += sx;
					}
				} else {
					double slope = dx / dy;
					double pitch = fromX - slope * fromY;
					while(fromY != toY) {
						slope_field[(int)(slope * fromY + pitch)][fromY] = targetAngle;
						fromY += sy;
					}
				}
			}
			
			slope_field[toX][toY] = targetAngle;
			
		} else if(path.getType().compare("curved") == 0) {
			double centerX = path.getX1();
			double centerY = path.getY1();
			double fromA = path.getX2();
			double toB = path.getY2();
			
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

void PathGenerator::clear() {
	paths.clear();
	for(int i = 0; i < 18; i++)
		for(int j = 0; j < 18; j++)
			slope_field[i][j] = 0.0;
}

double PathGenerator::getSlopeAt(double x, double y) {
	int xIndex = floor(x * 3);
	int yIndex = floor(y * 3);
	return slope_field[xIndex][yIndex];
}