/*
 * Coverage.c
 *
 *  Created on: 27 Nov 2019
 *      Author: adilah
 */

#include <CarLocation.h>


void initCarLocationmaps(){
    /*
     * Initialise Car Location Map based on 20 x 20 array.
     * This map keeps track of the car's(robot) position
     *
     * Map dimensions: 2m x 2m, each square is 10cm x 10cm
     */
    int i,j;
    for (i = 0; i < 20; i++) {
            for (j = 0; j < 20; j++) {
                mapsCarLo[i][j] = 0;
            }
        }
    //Set current map pointer to initial location which is at the bottom left of map.
    currCLPointer[0]=19;
    currCLPointer[1]=0;

    //set car current position in map as 1, which means covered.
    mapsCarLo[19][0]=1;

    //Set previous car direction as 0 = North;
    prevPointer=0;

}

void updateCarLocationMap(int boxes, int pointer){
    /*
    updates the car location map
    params: 
    - boxes: the number grid squares the robot has passed
    - pointer: the direction the car is facing
    */

    if (prevPointer != pointer) {  // check if robot pointer has changed direction
        prevPointer = pointer;
    }


    int i;
    int x = currCLPointer[0];
    int y = currCLPointer[1];

    //Set previous position as 0 to indicate car is no longer within that box.
    mapsCarLo[x][y] = 0;

    //according to direction, fill according to no. of box
    for (i = 1;i < boxes+1;i++) {
        switch (prevPointer) {
        case 0:
            //move North, y - 1 ; since initial pos is at 19(bottom)
            currCLPointer[0] = currCLPointer[0] - 1;
            break;
        case 1:
            //move East, x + 1
            currCLPointer[1] = currCLPointer[1] + 1;
            break;
        case 2:
            //move South, y + 1
            currCLPointer[0] = currCLPointer[0] + 1;
            break;
        case 3:
            //move West, x-1
            currCLPointer[1] = currCLPointer[1] - 1;
            break;
        }
        x = currCLPointer[0];
        y = currCLPointer[1];
        printf("%d : x pointer ", x);
        printf("%d : y pointer\n", y);

        //Set current position as car position
        mapsCarLo[x][y] = 1;

    }
}

void displayCarLocationMap(){
    /*
     * Print out the car location map based on the map dimensions
     */
    int i,j;
    for (i = 0; i < 20; i++) {
        printf("[");
        for (j = 0; j < 20; j++) {
            if (mapsCarLo[i][j]) {
                printf("%d,", mapsCarLo[i][j]);
            }
            else {
                printf(" ,");
            }
        }
        printf("]\n");
    }

}







colArray* getCarLocationMap(){
    /*
     * return the car location map
     */
    return mapsCarLo;
}

void clearCarLocationMap(){
    /*
     * clear the car location map
     */
    int i,j;
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            mapsCarLo[i][j] = 0;
        }
    }
}

int* getCurrentPosition(){

    /*
     * return the car's current position based on the map dimensions
     */
    return currCLPointer;
}







