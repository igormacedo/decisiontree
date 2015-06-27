/*
 * RandomPoint.cpp
 *
 *  Created on: Jun 18, 2015
 *      Author: igormacedo
 */

#include "RandomPoint.h"
#include <cmath>

RandomPoint::RandomPoint() {
	// TODO Auto-generated constructor stub

}

RandomPoint::~RandomPoint() {
	// TODO Auto-generated destructor stub
}

ColorPoint RandomPoint::getRandomPoint(float xmax, float xmin, float ymax, float ymin, Color color){
	printf("-----------------\n");
	float x = xmin + (((float) rand() / (RAND_MAX)) * (xmax - xmin));
	//float x = xmin + ((float) rand() / (RAND_MAX / xmax - xmin));
	printf("x = %0.3f\n", x);
	float y = ymin + (((float) rand() / (RAND_MAX)) * (ymax - ymin));
	//float y = ymin + fmod(((float) rand() / RAND_MAX),ymax);
	printf("y = %0.3f\n", y);
	ColorPoint p(x,y,color);
	return p;
}

