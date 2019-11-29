/*
 * Coverage.c
 *
 *  Created on: 27 Nov 2019
 *      Author: adilah
 */

#include <Coverage.h>


void initCoveragemaps(){
    /*
     * Initialise Coverage Map based on 20 x 20 array.
     * This map covers the area the car has already travelled
     *
     * Map dimensions: 2m x 2m, each square is 10cm x 10cm
     */
    int i,j;
    for (i = 0; i < 20; i++) {
            for (j = 0; j < 20; j++) {
                mapsCO[i][j] = 0;
            }
        }
    //Set current map pointer to initial location which is at the bottom left of map.
    currCOPointer[0]=19;
    currCOPointer[1]=0;

    //set car current position in map as 1, which means covered.
    mapsCO[19][0]=1;

    //Set previous car direction as 0 = North;
    prevPointer=0;

}

void updateCoverageMap(int boxes, int pointer){
    /*
    updates the area coverage map
    params: 
    - boxes: the number grid squares the robot has passed
    - pointer: the direction the car is facing
    */

    if (prevPointer != pointer) {  // check if robot pointer has changed direction
        prevPointer = pointer;
    }

    //according to direction, fill according to no. of box
    int i;

    for (i = 1;i < boxes+1;i++) {
        switch (prevPointer) {
        case 0:
            //move North, y - 1 ; since initial pos is at 19(bottom)
            currCOPointer[0] = currCOPointer[0] - 1;
            break;
        case 1:
            //move East, x + 1
            currCOPointer[1] = currCOPointer[1] + 1;
            break;
        case 2:
            //move South, y + 1
            currCOPointer[0] = currCOPointer[0] + 1;
            break;
        case 3:
            //move West, x-1
            currCOPointer[1] = currCOPointer[1] - 1;
            break;
        }

        //set current position to the box to be filled
        int x = currCOPointer[0];
        int y = currCOPointer[1];
        printf("%d : x pointer ", x);
        printf("%d : y pointer\n", y);

        // set element to 1 to indicate covered
        //if this cell is 1, turn right
        mapsCO[x][y] = 1;

    }

}

void displayCoverageMap(){
    /*
     * Print out the area coverage map based on the map dimensions
     */
    int i,j;
    for (i = 0; i < 20; i++) {
        printf("[");
        for (j = 0; j < 20; j++) {
            if (mapsCO[i][j]) {
                printf("%d,", mapsCO[i][j]);
            }
            else {
                printf(" ,");
            }
        }
        printf("]\n");
    }

}







colArray* getCoverageMap(){
    /*
     * return the area coverage map
     */
    return mapsCO;
}
void clearCoverageMap(){
    /*
     * clear the coverage map
     */
    int i,j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            mapsCO[i][j] = 0;
        }
    }
}







