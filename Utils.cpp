#include "Utils.hpp"


double Utils::utility::max(double max, double value) {
	//cause im gonna forget
	//(condition) ? truevalue : falsevalue
	return (value < max) ? value : max;
}

double Utils::utility::min(double min, double value) {
	//cause im gonna forget
	//(condition) ? truevalue : falsevalue
	return (value > min) ? value : min;
}

double Utils::utility::wrap(double x, double min, double max) {
	if (x > min && x < max) { return x; }
	if (x < min) { return max; }
	if (x > max) { return min; }

	//just so c++ stops complaining, first if should take care of this
	return x;
}
