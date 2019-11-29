/*
 * MapManagement.h
 *
 *  Created on: 27 Nov 2019
 *      Author: adilah
 */

#ifndef MAPMANAGEMENT_H_
#define MAPMANAGEMENT_H_

#include "Coverage.h"
#include "CarLocation.h"
#include "Obstacle.h"
#include "Car.h"
#include "wheel.h"
#include "wifi.h"
int isForward;

void mapManagementInit();
void updateAreaCoverage(int boxes);

colArray* getAreaCoverageMap();
void clearAreaCoverageMap();
int* getRobotPosition();
colArray* getRobotPositionMap();
void clearRobotPosition();
void updateObstacleDetectionMap();
colArray* getObstacleDetectionMap();
void clearObstacleDetectionMap();
void clearAllMaps();

//testing purposes
void displayMaps();
void setCarPointer(int pointer);

#endif /* MAPMANAGEMENT_H_ */
