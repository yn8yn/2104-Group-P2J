/*
 * Car.c
 *
 *  Created on: 24 Nov 2019
 *      Author: adilah
 */

#include <Car.h>


void carInit(){
    pointer=0;
}

int getPointer(){
    /*
     * Retrieve car direction
     * */
    return pointer;
}
void setPointer(int value){
    /*
     * set car direction
     * 0 = North    1 = East    2 = South   3 = West
     */

    if(value==1){ //turn right
        if((pointer+value)==4){
                pointer=0;
        }else{
            pointer+=value;
        }
    }else if (value==-1){
        if((pointer-value)<0){
                pointer=3;
        }else{
            pointer-=value;
        }
    }

}
