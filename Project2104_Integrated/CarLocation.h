/*
 * CarLocation.h
 *
 *  Created on: 24 Nov 2019
 *      Author: adilah
 */

#ifndef CARLOCATION_H_
#define CARLOCATION_H_

#include <stdio.h>
/*
 * Create a car location maps that tracks the car's(robot) position on the 2m x 2m map.
 */

int mapsCarLo[20][20];
int currCLPointer[2];
int prevPointer;
void initCarLocationmaps(void);
void updateCarLocationMap(int boxes, int pointer);
void displayCarLocationMap();

typedef int colArray[20];
int* getCurrentPosition();
colArray* getCarLocationMap();
void clearCarLocationMap();


#endif /* CARLOCATION_H_ */
