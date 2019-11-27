//#include "msp.h"
//
//unsigned int up_counter;
//unsigned int distance_cm;
//
///*
// * Ultrasonic Sensor
// * Trigger pin is P2.4
// * Echo Pin is P2.6
// *
//
// */
//int main(void)
//{
//    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer
//    //Set PIN 2.4 to setup Ultrasonic sensor trigger
//    P2->DIR |= BIT4;
//    P2->DIR &= ~BIT4;
//
//    /* Set P2.6 to input direction (echo)
//       input for Timer A0 - Compare/Capture input */
//        P2DIR &= ~BIT6;
//        // Select P2.6 as timer trigger input select (echo from sensor)
//        P2SEL0 |=BIT6;
//
//
//
//        /* Timer A0 configure to read echo signal:
//            Timer A Capture/Compare Control 3 =>
//            capture mode: 1 - both edges +
//            capture synchronize +
//            capture input select 0
//            capture mode +
//            capture compare interrupt enable */
//            TA0CCTL3 |= CM_3 + SCS + CCIS_0 + CAP + CCIE;
//
//            /* Timer A Control configuration =>
//            Timer A clock source select: 1 - SMClock +
//            Timer A mode control: 2 - Continuous up +
//            Timer A clock input divider 0 - No divider */
//            TA0CTL |= TASSEL_2 + MC_2 + ID_0;
//
//            NVIC->ISER[0] |= 1 << (PORT1_IRQn & 31);
//            __enable_interrupt();
//
//
//            while (1)
//            {
//                P2OUT ^= BIT4;              // assert
//                __delay_cycles(20*60*1000);
//
//                P2OUT ^= BIT4;              // deassert
//                __delay_cycles(20*60*1000);
//                TimerA0();
//
//            }
//}
//
// void TimerA0(void)
//{
//    if (TA0CCTL3 & CCI)            // Raising edge
//    {
//        up_counter = TA0CCR3;      // Copy counter to variable
//    }
//    else                        // Falling edge
//    {
//        int sensor_value;
//        // Formula: Distance in cm = (Time in uSec)/58
//        sensor_value = (TA0CCR3 - up_counter)/8;
//        distance_cm = sensor_value / 58;
//        printf("Distance : %d\n", distance_cm);
//
//    }
//    TA0CTL &= ~TAIFG;           // Clear interrupt flag - handled
//}
