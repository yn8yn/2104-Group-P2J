/*
 * Ultrasonic.c
 *
 *  Created on: 27 Nov 2019
 *      Author: Jerry
 */

#include <Ultrasonic.h>
#include "msp.h"

int seconds;
int distance;
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
    callTrigger();
}
void callTrigger()
{
    P2OUT |= BIT4;
    __delay_cycles(10);
    P2OUT &= ~BIT4;
    TIMER_A0->CTL |= TIMER_A_CTL_CLR;
}
void delayCycles(int n)
{
    int i;

    TIMER32_1->LOAD = 3; /* reload with number of clocks per millisecond */
    /* no prescaler, periodic wrapping mode, disable interrupt, 32-bit timer. */
    TIMER32_1->CONTROL = 0xC2;

    for (i = 0; i < n; i++)
    {
        while ((TIMER32_1->RIS & 1) == 0)
            ; /* wait until the RAW_IFG is set */
        TIMER32_1->INTCLR = 0; /* clear RAW_IFG flag */
    }
}

void PORT1_IRQHandler(void)
{
    if (P1IFG & BIT5)
    {
        sensor = TA0R / 3;
        distance = sensor / 58;
        int rangeCount = 0;
        for (rangeCount = 0; rangeCount < 10; rangeCount++)
        {
            if (distance > 100)
            {
                //do nothing, no need to check so far
            }
            else
            {
                sum_distance += distance;
                usRange[rangeCount] = distance;
            }
        }
        average_distance = getAverageDistance(usRange);
        sensor = TA0R / 3;

//      printf(("%d",)previous_distance);
        printf("%d\n", distance);
        __delay_cycles(20 * 60 * 1000);
        callTrigger();
    }
    P1IFG &= ~BIT5;
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
