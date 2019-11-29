#include "msp.h"
#include "MapManagement.h"
#include "encoder.h"
#include "Ultrasonic.h"
//#include "wheel.h"

/**
 * main.c
 */

#define PWM_PERIOD_MAIN 1500

int sec=0;

void delayMs(int n);
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer

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


    __enable_irq();

    US_init();
//    forward();
//    delayMs(6000);
//    stop();
//    turnright();
//    delayMs(2000);
//    turnright();

    while(1){
//        delayMs(1000);
//        delayMs(6000);
//        stop();
//
//        break;
    }


    //---------------------classes intialization









}


void delayMs(int n)
{
    int i;

    TIMER32_1->LOAD = 3000 - 1;// ---------------------------------------change to timer a2
    TIMER32_1->CONTROL = 0xC2; //- register T32, bit 1100 0010,

    for(i = 0; i < n; i++) {
        while((TIMER32_1->RIS & 1) == 0); /* wait until the RAW_IFG is set  - start from 2999, starts counting until 0*/
        TIMER32_1->INTCLR = 0;            /* clear RAW_IFG flag  - once it hits 0, the flag will go down. clear the flag*/
    }
}

//void PORT1_IRQHandler(void){
//    if(P1->IFG & BIT1){
//        continues=0;
//        displayMaps();
//    }
//}


//void TA3_setup(){
//    P8->DIR  |=BIT2 ;     // P2.6 - P2.7 output
//    P8->SEL0 |=BIT2;     // P2.6 - P2.7  Port Map functions  = 1
//    P8->SEL1 &= ~ BIT2;
//
//    // Setup TA0
//  TIMER_A3->CCR[0] = PWM_PERIOD_MAIN;              // PWM Period = 1500/1500000sec = 1/1000 = 1ms, the length of time the light is on
//  TIMER_A3->CCTL[2] = TIMER_A_CCTLN_OUTMOD_7; // CCR3 reset/set
//  TIMER_A3->CCR[2] = PWM_PERIOD_MAIN;              // CCR3 PWM duty cycle, 0.5ms -----------control pwm speed, but at pwm_period/2, the motor can start, must jolt
//
//
//  TIMER_A3->CTL = TIMER_A_CTL_SSEL__SMCLK|    // SMCLK
//                  TIMER_A_CTL_MC_1 |          // Up mode
//                  TIMER_A_CTL_CLR|            // Clear TAR
//                  TIMER_A_CTL_IE;             // Enable overflow interrupt
//  // Connect TA1_0 interrupt request to NVIC
// NVIC->ISER[1] |= 1 << ((TA3_0_IRQn) & 31);   //TA1_0_IRQn
//
//}
//
//void TA3_0_IRQHandler(void){
//    TIMER_A0->CCTL[0] &= ~0x0001;
//
//    P2->OUT^=BIT0;
//    sec++;
//    if(sec!=4000){
//        sec++;
//    }else{
//        continues=0;
//        displayMaps();
//    }
//
//}





