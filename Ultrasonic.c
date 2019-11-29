/*
 * Ultrasonic.c
 *
 *  Created on: 27 Nov 2019
 *      Author: Jerry
 */

#include <Ultrasonic.h>
#include "msp.h"

int seconds;
int distanceU;
long sensor;
int average_distance;
int usRange[10];
int sum_distance;

void US_init()

{

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR
            | TIMER_A_CTL_IE;
    //INIT PINS
    //TRIGGER
    P2DIR |= BIT4;
    //ECHO
    P1->DIR &= ~BIT5;
    P1IE |= BIT5;
    P1IES |= BIT5;
    //SET INTERRUPT FLAGS for P1
    P1IFG = 0x00;

    NVIC->ISER[1] |= 1 << (PORT1_IRQn & 31);
    __enable_interrupt();
    //SET TIMER_A TO
    TA0CCR0 = 60000;
//    printf("init");
    continues=1;
    callTrigger();
}
void callTrigger()
{
    if(continues==1){
        P2OUT |= BIT4;
        __delay_cycles(10);
        P2OUT &= ~BIT4;
        TIMER_A0->CTL |= TIMER_A_CTL_CLR;
    }
//    printf("init");

}
void delayCycles(int n)
{
    int i;

    TIMER32_2->LOAD = 3; /* reload with number of clocks per millisecond */
    /* no prescaler, periodic wrapping mode, disable interrupt, 32-bit timer. */
    TIMER32_2->CONTROL = 0xC2;

    for (i = 0; i < n; i++)
    {
        while ((TIMER32_2->RIS & 1) == 0)
            ; /* wait until the RAW_IFG is set */
        TIMER32_2->INTCLR = 0; /* clear RAW_IFG flag */
    }
}

void PORT1_IRQHandler(void)
{
    if(P1->IFG & BIT1){
        continues=0;
        stop();
//        callTrigger();
        displayMaps();
        exit(0);
    }

    if (P1IFG & BIT5)
    {
        sensor = TA0R / 3;
        distanceU = sensor / 58;
        int rangeCount = 0;
        for (rangeCount = 0; rangeCount < 10; rangeCount++)
        {
            if (distanceU > 100)
            {
                //do nothing, no need to check so far
            }
            else
            {
                sum_distance += distanceU;
                usRange[rangeCount] = distanceU;
            }
        }
        average_distance = getAverageDistance(usRange);

        checkDistance(average_distance, distanceU);
        forward();
        sensor = TA0R / 3;

//      printf(("%d",)previous_distance);
        printf("%d\n", distanceU);
//        __delay_cycles(20 * 60 * 1000);
        callTrigger();
    }
    P1IFG &= ~BIT5 +~BIT1;
    sum_distance = 0;
}
void TA0_0_IRQHandler(void)
{
    seconds++;
}
int getAverageDistance(int distance[10])
{
    average_distance = sum_distance / 10;
    return average_distance;
}
void checkDistance(int x_average, int y_distance){
    if(y_distance<10){
        stop();
        updateObstacleDetectionMap();
        turnright();
        __delay_cycles(20 * 60 * 1000);
        P2->OUT ^= BIT2;
        forward();
    }
    else{
        forward();
        P1->OUT ^= BIT0;

    }
//    //distance<10
//    //turn the car
//    //set delay so that the distance from sonar wont be read again until the car finish turning. so wont trigger another turn
//    //forward()
//    if (x_average > y_distance + 10){
//        stop();
//        updateObstacleDetectionMap();
//        turnright();
//        delayCycles
//
//    }
//    else{
//        forward();
//    }
}
