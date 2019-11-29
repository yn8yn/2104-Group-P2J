/*
 * Obstacle.h
 *
 *  Created on: 24 Nov 2019
 *      Author: adilah
 */

#ifndef OBSTACLE_H_
#define OBSTACLE_H_



#include <stdio.h>
/*
 * Create an obstacle map  that tracks all obstacles detected on the 2m x 2m map.
 */

int mapsObstacle[20][20];
int currPointer[2];
int prevPointer;
void initObstaclemaps(void);
int updateObstacleMap(int* position, int pointer);
void displayObstacleMap();
void boundaryEncounter();
typedef int colArray[20];
colArray* getObstacleMap();
void clearObstacleMap();

#endif /* OBSTACLE_H_ */
