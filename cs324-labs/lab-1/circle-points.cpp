#include <cmath> 
#include <iostream>

int main(int, char** argv[])
{
	// here is some code find vertices on a circle

	const float radius = 2.0f; // a given radius
	const float pi = atan(1)*4; // define pi

	const size_t steps = 32; // how many steps once round the circle
	const float theta_step = 2*pi/float(steps); // angle step in radians

	float theta = 0.0f;

	// however many times once round the circle
	for (size_t i=0;i<steps;i++, theta+=theta_step) 
	{
		float x = radius * cos(theta);
		float y = radius * sin(theta);

		// debug to standard out
		std::cout << "x = " << x << ", y = " << y << std::endl;
	}
	
	return 0;
}

