#include "main.h"
#include "path.h"
#include <vector>
#include <string>
#include <tuple>

class PathGenerator {
	private:
		std::vector<Path> paths;
		double slope_field [18][18];

	public:
		PathGenerator();
		PathGenerator & addPath(Path path);
		void generate();
		void clear();
		double getSlopeAt(double x, double y);
};