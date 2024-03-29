/*
 * MapManagement.c
 *
 *  Created on: 27 Nov 2019
 *      Author: adilah
 */

#include <MapManagement.h>

int inarea=0;
void mapManagementInit(){
    /*
     * initialise all maps to be used
     */
    initCoveragemaps();
    initCarLocationmaps();
    initObstaclemaps();
    carInit();

}

void updateAreaCoverage(int boxes){
    /*
     * update both area coverage map and car location map
     */
    if(isForward==1){ // check if car is moving forward or turning/reversing
        inarea++;
        updateCoverageMap(boxes,getPointer());
        updateCarLocationMap(boxes,getPointer());
    }
}

colArray* getAreaCoverageMap(){
    /*
     * return coverage map
     */
    return getCoverageMap();
}

void clearAreaCoverageMap(){
    /*
     * clear coverage map
     */
    clearCoverageMap();
}

int* getRobotPosition(){
    /*
     * return car current position
     */
    return getCurrentPosition();

}

colArray* getRobotPositionMap(){
    /*
     * get car location map
     */
    return getCarLocationMap();
}

void clearRobotPosition(){
    /*
     * clear car location map
     */
    clearCarLocationMap();
}

void updateObstacleDetectionMap(){
    /*
     * update obstacle map based on car location getCurrentPosition and the car object direction
     */
    int success=updateObstacleMap(getCurrentPosition(),getPointer());
    if(success==1){  // check if reached boundary. if boundary reached then turn once, so the next turning in ultrasonic object will turn the car in u-turn direction
        turnright();

    }


}

colArray* getObstacleDetectionMap(){
    /*
     * return obstacle detection map
     */
    return getObstacleMap();
}

void clearObstacleDetectionMap(){
    /*
     * clear obstacle direction map
     */
    clearObstacleMap();
}


void clearAllMaps(){
    clearCoverageMap();
    clearCarLocationMap();
    clearObstacleMap();
}


//testing purposes
void displayMaps(){
    printf("\n");
    displayCoverageMap();
    printf("\n");
    displayCarLocationMap();
    printf("\n");
    displayObstacleMap();

}

void setCarPointer(int pointer){
    setPointer(pointer);
}



