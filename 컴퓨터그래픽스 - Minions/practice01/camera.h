#pragma once
#include <glut.h>

#define _USE_MATH_DEFINES
#include <cmath>


class camera {
public:
	GLfloat x, y, z;//location
	GLfloat distance;//from object(min:1)
	double angle_v, angle_h;
	
	void setLocation(float, float);
	void setDistance(float);

	camera() {
		distance = 5;
		x = 0;
		y = 1;
		z = -5;
		
		angle_v = 0;
		angle_h = 0;
		
	}

private:
	int getNear(double);
};