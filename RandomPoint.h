/*
 * RandomPoint.h
 *
 *  Created on: Jun 18, 2015
 *      Author: igormacedo
 */

#ifndef RANDOMPOINT_H_
#define RANDOMPOINT_H_

#include "DecisionTree.h"
#include <stdio.h>
#include <stdlib.h>

class RandomPoint {
public:
	RandomPoint();
	virtual ~RandomPoint();

	static ColorPoint getRandomPoint(float,float,float,float,Color);
};

#endif /* RANDOMPOINT_H_ */
