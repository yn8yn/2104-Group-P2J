/*
 * wheel.c
 *
 *  Created on: 28 Oct 2019
 *      Author: adilah
 */

#include <wheel.h>
#define PWM_PERIOD 1500
uint16_t AIN1; //p4.2
uint16_t AIN2; //p4.3
uint16_t BIN1; //p4.5
uint16_t BIN2; //p4.7
uint16_t PWM1; //p4.1
uint16_t PWM2; //p4.6
uint16_t STBY; //p4.4


//STBY MUST BE 1 FOR MOTORS TO MOVE
void delayMsW(int n);

void init_wheel(uint16_t inputL, uint16_t inputR,uint16_t inputAL, uint16_t inputAR,uint16_t pwma,uint16_t pwmb,uint16_t stby){
    AIN1=inputAL;
    AIN2=inputAR;
    BIN1=inputL;
    BIN2=inputR;
    PWM1=pwma;
    PWM2=pwmb;
    STBY=stby;

    P4DIR=0;
    P4OUT=0;
    P4DIR |=AIN1+AIN2+BIN1+BIN2+PWM1+PWM2+STBY;



}

void TA1_0_IRQHandler(void){
    if(TIMER_A2->CCR[3]>=PWM_PERIOD && TIMER_A2->CCR[3]>(PWM_PERIOD/2)){
        TIMER_A2->CCR[3]-=100;
    }
    if((P2->IN&BIT6)==BIT6)
        {
            P4->OUT|=BIT6;          // ON LED1
            P4->OUT|=BIT1;
            P1->OUT|=BIT0;
        }
        else if((P2->IN&BIT6)==0)
        {
            P4->OUT&=~BIT6;         // OFF LED1
            P4->OUT&=~BIT1;
            P1->OUT&=~BIT0;
        }
    TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
}

void forward(){
    isForward=1;
    P4OUT |= STBY+AIN1+PWM1+BIN2+PWM2;
//    P4OUT |= STBY+AIN1+BIN2;
    P4OUT &=~AIN2+~BIN1;


    //in1 in2 pwm out1 out2
    //H   L    H-> H   L   => MOVE FORWARD

}
void forwardlp(){
    isForward=1;
    P4OUT |= STBY+AIN1+BIN2;
    P4OUT &=~AIN2+~BIN1+~PWM2;
//    P4OUT =




    //in1 in2 pwm out1 out2
    //H   L    H-> H   L   => MOVE FORWARD

}

void backward(){
    isForward=0;
    P4OUT |= STBY+AIN2+PWM1+BIN1+PWM2;
    P4OUT &=~AIN1+~BIN2;


    //in1 in2 pwm out1 out2
    //L   H    H-> L   H   => MOVE BACKWARD

}

void turnright(){
    setCarPointer(1);
    isForward=0;
    P4OUT |= STBY+AIN1+PWM1+BIN1+PWM2;
    P4OUT &=~AIN2+~BIN2;
    delayMsW(1000);
    stop();


    //in1 in2 pwm out1 out2
    //L   H    H-> L   H

    //in1 in2 pwm out1 out2
    //H   L    H-> H   L
    //EACH WHEEL TURN DIFF DIRECTION

}

void turnleft(){
    setCarPointer(-1);
    isForward=0;
    P4OUT |= STBY+AIN2+PWM1+BIN2+PWM2;
    P4OUT &=~AIN1+~BIN1;


    //in1 in2 pwm out1 out2
    //H   L    H-> H   L

    //in1 in2 pwm out1 out2
    //L   H    H-> L   H
    //EACH WHEEL TURN DIFF DIRECTION

}

void stop(){
    isForward=0;
    TIMER_A2->CCR[3]=PWM_PERIOD;
    P4OUT |= PWM1+PWM2+BIN1+BIN2;
    P4OUT &=~AIN2+~AIN1+~STBY;

    //in1 in2 pwm out1 out2
    //L   L    H-> OFF   OFF
    //short break
    // H L L
}

void pwm_init(){
    //pwm_period 1500
    P6->DIR  |=BIT6 ;     // P2.6 - P2.7 output
    P6->SEL0 |=BIT6;     // P2.6 - P2.7  Port Map functions  = 1
    P6->SEL1 &= ~ BIT6;   // P2.6 - P2.7 default PM functions = TimerA CCRx = 0,  10h in technic docs is secondary module, this is taken to control pwm for led

    CS->KEY = CS_KEY_VAL ;                      // Unlock CS module for register access
    CS->CTL0 = 0;                               // Reset tuning parameters
    CS->CTL0 = CS_CTL0_DCORSEL_0;               // Set DCO to 1.5MHz (nominal, center of 1-2MHz range)

    // Select ACLK = REFO, SMCLK = MCLK = DCO
    CS->CTL1 = CS_CTL1_SELA_2 | CS_CTL1_SELS_3 | CS_CTL1_SELM_3;
    CS->KEY = 0;                                // Lock CS module from unintended accesses

    // Setup TA0
   TIMER_A2->CCR[0] = PWM_PERIOD;              // PWM Period = 1500/1500000sec = 1/1000 = 1ms, the length of time the light is on
   TIMER_A2->CCTL[3] = TIMER_A_CCTLN_OUTMOD_7; // CCR3 reset/set
   TIMER_A2->CCR[3] = PWM_PERIOD;              // CCR3 PWM duty cycle, 0.5ms -----------control pwm speed, but at pwm_period/2, the motor can start, must jolt
   TIMER_A2->CCTL[4] = TIMER_A_CCTLN_OUTMOD_7; // CCR4 reset/set
   TIMER_A2->CCR[4] = 0;                       // CCR4 PWM duty cycle

   TIMER_A2->CTL = TIMER_A_CTL_SSEL__SMCLK|    // SMCLK
                   TIMER_A_CTL_MC_1 |          // Up mode
                   TIMER_A_CTL_CLR;            // Clear TAR
                   //TIMER_A_CTL_IE;             // Enable overflow interrupt

   // Setup TA1
   //TIMER_A1->CCTL[0] &= ~TIMER_A_CCTLN_CCIFG;
   TIMER_A1->CCTL[0] = TIMER_A_CCTLN_CCIE;     // TACCR0 interrupt enabled
   TIMER_A1->CCR[0] = PWM_PERIOD/10;           // 1/10ms polling rate
   TIMER_A1->CTL |= TIMER_A_CTL_SSEL__SMCLK |  // SMCLK
                    TIMER_A_CTL_MC_1 |         // Up mode
                    TIMER_A_CTL_CLR |          // Clear TAR
                    TIMER_A_CTL_IE;            // Enable overflow interrupt

   // Connect TA1_0 interrupt request to NVIC
   NVIC->ISER[0] |= 1 << ((TA1_0_IRQn) & 31);   //TA1_0_IRQn


}

void delayMsW(int n)
{
    int i;

    TIMER32_1->LOAD = 3000 - 1;// ---------------------------------------change to timer a2
    TIMER32_1->CONTROL = 0xC2; //- register T32, bit 1100 0010,

    for(i = 0; i < n; i++) {
        while((TIMER32_1->RIS & 1) == 0); /* wait until the RAW_IFG is set  - start from 2999, starts counting until 0*/
        TIMER32_1->INTCLR = 0;            /* clear RAW_IFG flag  - once it hits 0, the flag will go down. clear the flag*/
    }
}
