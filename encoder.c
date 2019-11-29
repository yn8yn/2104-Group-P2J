/*
 * encoder.c
 *
 *  Created on: 24 Oct 2019
 *      Author: adilah
 */

#include <encoder.h>


#define PWM_PERIOD 1500

int count;
int lcount;
int rev;
int lrev;
int enSpeed;
int j;
float speed=0;
float distance;
float PREV_distance=0;
int boxes_cov=0;

void encoder_init(){


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
    // monitor input from encoder
    if(P3->IFG & BIT6)  // if flag is set on bit1, input from encoder present
    {

        count++;                                // count pulses

        P3IFG &= ~BIT6;                       // clear the flag
//        if(count>20){
//            rev++;                             // per every 4 pulse, 1 revolution
//            count=0;
//
//        }
        float round=(float)count*(22/20);
        if(round>10){
            //execute map update
            distance+=round;
            count=0;
        }
        if(distance-PREV_distance>10){
            boxes_cov++;
//            P2->OUT^=BIT1;
            //do map update here
            updateAreaCoverage(1); // change to 1 if top change from 60 to 10
            PREV_distance=distance;

        }
    }
}
void encoder_left(void){
    // monitor input from encoder
    if(P3->IFG & BIT7)  // if flag is set on bit1, input from encoder present
    {

        lcount++;                                // count pulses

        P3IFG &= ~BIT7;                       // clear the flag
        if(lcount>20){                          // do update map here------------------------
            lrev++;                             // per every 4 pulse, 1 revolution
            lcount=0;                           //calc distance here

        }
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
    rev=0;
}


float getSpeed(void){
    float round=(float)count*(22/20);
    distance+=round;
    speed=round/6; //cm/10 milliseconds
    count=0;
//    rev=0;


    return speed;
}






