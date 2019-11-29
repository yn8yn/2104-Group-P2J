/*
 * encoder.c
 *
 *  Created on: 24 Oct 2019
 *      Author: adilah
 */

#include <encoder.h>


#define PWM_PERIOD 1500

int count;          //count the number of holes that has been detected
int lcount;         //count for left encoder
int rev;            //the number of revelutions that has occured
int lrev;           // revolutions for left encoder
int enSpeed;        //
int j;
float speed=0;
float distance;
float PREV_distance=0;
int boxes_cov=0;

void encoder_init(){
    /*
    setup the encoder interupt and input port
    */

    P3DIR=0;
    P3DIR &=~BIT6+~BIT7;
    P3REN |=BIT6+BIT7;
    P3OUT |= BIT6+BIT7;

    P3IES &= ~BIT6+BIT7;
    P3IE |= BIT6+BIT7;
    P3IFG = 0;


    NVIC->ISER[1] = 1 << ((PORT3_IRQn) & 31);
        __enable_interrupts();



}

void encoder_right(void){
    // monitor input from right ewncoder
    if(P3->IFG & BIT6)  // if flag is set on bit1, input from encoder present
    {

        count++;                                // count pulses

        P3IFG &= ~BIT6;                       // clear the flag

        float round=(float)count*(22/20);
        if(round>10){
            //update distance covered
            distance+=round;
            count=0;
        }
        if(distance-PREV_distance>10){  //if the distance has passed 10cm(1 square) then update the map
            boxes_cov++;

            //Updates coverage and car location map
            updateAreaCoverage(1);
            PREV_distance=distance;

        }
    }
}
void encoder_left(void){
    // monitor input from left encoder
    if(P3->IFG & BIT7)  // if flag is set on bit1, input from encoder present
    {

        lcount++;                                // count pulses

        P3IFG &= ~BIT7;                       // clear the flag

    }
}

void minuteDelay(void){

    int n =3000;
    TIMER32_1->LOAD = 3000 - 1;
    TIMER32_1->CONTROL = 0xC2;


        for(j = 0; j < n; j++) {
            while((TIMER32_1->RIS & 1) == 0);
            TIMER32_1->INTCLR = 0;
        }
}

///* Port2 ISR */
void PORT3_IRQHandler(void)
{
    encoder_right();

}

void changeRev(void){
    /*
    reset revolution count
    */
    rev=0;
}









