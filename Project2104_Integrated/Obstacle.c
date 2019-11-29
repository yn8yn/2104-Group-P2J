/*
 * Obstacle.c
 *
 *  Created on: 24 Nov 2019
 *      Author: adilah
 */

#include <Obstacle.h>

void initObstaclemaps(){
    /*
     * Initialise obstacle Map based on 20 x 20 array.
     * This map covers the tracking of obstacles detected within the map dimension boundary
     *
     * Map dimensions: 2m x 2m, each square is 10cm x 10cm
     */
    int i,j;
    for (i = 0; i < 20; i++) {
            for (j = 0; j < 20; j++) {
                mapsObstacle[i][j] = 0;
            }
        }

    //Set current map pointer to initial location which is at the bottom left of map.
    currPointer[0]=19;
    currPointer[1]=0;

    //Set previous car direction as 0 = North;
    prevPointer=0;

}

int updateObstacleMap(int* position, int pointer){
    /*
    updates the obstacle map
    params: 
    - position: the current position of th robot in the 2m x 2m map
    - pointer: the direction the car is facing
    */
    if (prevPointer != pointer) {  // check if robot pointer has changed direction
        prevPointer = pointer;
    }


    int x, y;

    //set current pointer to the input position which retrieves from Car Location getCurrentPosition

    currPointer[0] = position[0];
    currPointer[1] = position[1];
    y = currPointer[0];
    x = currPointer[1];
    if(currPointer[0]<20 && currPointer[1]<20 && currPointer[0]>=0 && currPointer[1]>=0){   //check if consecutive box is within map boundary
        //based on current position and car direction, fill in the cell directly infront
        switch (prevPointer) {
        case 0:
            //fill in 1 box directly infront in 0 (North) direction
            mapsObstacle[y-1][x] = 1;

            break;
        case 1:
            //fill in 1 box directly infront in 1 (East) direction
            mapsObstacle[y][x+1] = 1;

            break;
        case 2:
            //fill in 1 box directly infront in 2 (South) direction
            mapsObstacle[y + 1][x] = 1;

            break;
        case 3:
            //fill in 1 box directly infront in 3 (West) direction
            mapsObstacle[y][x-1] = 1;

            break;
        }
        return 0;
    }else{
        return 1;
    }


}


void displayObstacleMap(){
    /*
    * Print out the obstacle map based on the map dimensions
    */
    int i,j;
    for (i = 0; i < 20; i++) {
        printf("[");
        for (j = 0; j < 20; j++) {
            if (mapsObstacle[i][j]) {
                printf("%d,", mapsObstacle[i][j]);
            }
            else {
                printf(" ,");
            }
        }
        printf("]\n");
    }

}







colArray* getObstacleMap(){
    /*
     * return the area coverage map
     */
    return mapsObstacle;
}

void clearObstacleMap(){
    /*
     * clear the coverage map
     */
    int i,j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            mapsObstacle[i][j] = 0;
        }
    }
}





