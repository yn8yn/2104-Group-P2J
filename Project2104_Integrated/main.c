#include "msp.h"
#include "MapManagement.h"
#include "encoder.h"
#include "Ultrasonic.h"
#include "wifi.h"
//#include "wheel.h"

/**
 * main.c
 */

#define PWM_PERIOD_MAIN 1500

int sec=0;

void delayMs(int n);
void main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    //---------------------reset pins
    P4DIR=0;
    P4OUT=0;
    P1DIR=0;
    P1OUT=0;
    P2DIR=0;
    P2OUT=0;
    //---------------------reset pins

    //setup LED for LED monitoring
    P1DIR |= BIT0;
    P2DIR |= BIT1+BIT2+BIT0;


    //--------------------setup button interrupt
    P1DIR &= ~BIT1;
    P1REN |= BIT1;
    P1OUT |= BIT1;

    P1IES &= ~BIT1 ;
    P1IE |= BIT1;
    P1IFG = 0;

    //---------------------classes intialization
    mapManagementInit();
    encoder_init();
    init_wheel( BIT5,  BIT7, BIT2,  BIT3, BIT1, BIT6, BIT4);
    pwm_init();
    //---------------------classes intialization

    __enable_irq();

    US_init();


    while(1){

    }


    









}


void delayMs(int n)
{
    int i;

    TIMER32_1->LOAD = 3000 - 1;
    TIMER32_1->CONTROL = 0xC2; //- register T32, bit 1100 0010,

    for(i = 0; i < n; i++) {
        while((TIMER32_1->RIS & 1) == 0); /* wait until the RAW_IFG is set  - start from 2999, starts counting until 0*/
        TIMER32_1->INTCLR = 0;            /* clear RAW_IFG flag  - once it hits 0, the flag will go down. clear the flag*/
    }
}


