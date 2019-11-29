/*
 * Coverage.h
 *
 *  Created on: 27 Nov 2019
 *      Author: adilah
 */

#ifndef COVERAGE_H_
#define COVERAGE_H_

#include <stdio.h>
/*
 * Create an area coverage that tracks the area covered by the robot on the 2m x 2m map.
 */

int mapsCO[20][20];
int currCOPointer[2];
int prevPointer;
void initCoveragemaps(void);
void updateCoverageMap(int boxes, int pointer);
void displayCoverageMap();

typedef int colArray[20];
colArray* getCoverageMap();
void clearCoverageMap();



#endif /* COVERAGE_H_ */
