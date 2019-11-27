#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

int seconds;
int distance;
long sensor;
int average_distance;
int usRange[10];
int sum_distance;


void main(void)

{
    WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;       // stop watchdog timer

    TIMER_A0->CTL = TIMER_A_CTL_SSEL__SMCLK | TIMER_A_CTL_MC_1 | TIMER_A_CTL_CLR
            | TIMER_A_CTL_IE;
//TRIGGER
    P2DIR |= BIT4;
//ECHO
    P1->DIR &= ~BIT5;

    P1IFG = 0x00;
    P1IE |= BIT5;
    P1IES |= BIT5;
    NVIC->ISER[1] |= 1 << (PORT1_IRQn & 31);
    __enable_interrupt();

    TA0CCR0 = 60000;
    trigger();
    while (1)
    {
    }
}

void trigger()
{
    P2OUT |= BIT4;
    __delay_cycles(10);
    P2OUT &= ~BIT4;
    TIMER_A0->CTL |= TIMER_A_CTL_CLR;
}

void TA0_0_IRQHandler(void)
{
    seconds++;
}

void delayUs(int n)
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
    sensor = TA0R / 3;
    distance = sensor / 58;
    if (P1IFG & BIT5)
    {
        int rangeCount =0;
        for (rangeCount = 0; rangeCount < 10; rangeCount++)
        {
            if (distance > 200)
            {
                //donothing, won't check so far
            }
            else
            {

                usRange[rangeCount] = distance;
                sum_distance = sum_distance + usRange[rangeCount];
            }
        }
        average_distance = sum_distance / 10;
        if (average_distance > distance + 10)
            {
                // ADILAH turn here
            }
            else
            {
                //ADILAH go straight
            }


//      printf(("%d",)previous_distance);

        printf("%d\n", distance);
        __delay_cycles(20 * 60 * 1000);
        trigger();
    }
    P1IFG &= ~BIT5;
    sum_distance = 0;
}
