
#include <msp432.h>

unsigned int up_counter;
unsigned int down_counter;
unsigned int distance_cm;




int main(void)
{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;     // stop watchdog timer

    /* set P2.4 to output direction (trigger) */
    P2DIR |= BIT4;
    P2OUT &= ~BIT4;                 // keep trigger at low

    // Set P2.6 to input direction (echo)

    P2DIR &= ~BIT6;
    P2SEL0 |= BIT6;
    TA0CCTL3 |= CM_3 + SCS + CCIS_0 + CAP + CCIE;
    TA0CTL |= TASSEL_2 + MC_2 + ID_0;

    NVIC->ISER[0] |= 1 << (PORT2_IRQn & 31);

    // Global Interrupt Enable
    __enable_interrupt();
    TA0CCR3 = 60000;
    trigger();


     while (1)
    {

    }

}
void PORT2_IRQHandler(void)
{

   if (P2IFG & BIT6)        // Raising edge
      {
          up_counter = TA0R / 3 ;      // Copy counter to variable
          printf("UPCOUNTER =  %d\n", up_counter);
          trigger();
      }
   else                        // Falling edge
   {
       // Formula: Distance in cm = (Time in uSec)/58
       distance_cm = ( (TA0R/3 ) - up_counter) / 58;
       printf("Distance = %d\n", distance_cm);
       trigger();

   }
   P2IFG &= ~BIT6;
}
void trigger(){
            P2OUT |= BIT4;              // assert
           __delay_cycles(10);         // 10us wide
           P2OUT &= ~BIT4;              // deassert
           __delay_cycles(60000);      // 60ms measurement cycle
           TIMER_A0->CTL |= TIMER_A_CTL_CLR;
}
