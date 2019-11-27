#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>

/* Timer_A Continuous Mode Configuration Parameter */
const Timer_A_ContinuousModeConfig continuousModeConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,           // SMCLK Clock Source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,       // SMCLK/1 = 3MHz
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_SKIP_CLEAR                   // Skup Clear Counter
};

/* Timer_A Capture Mode Configuration Parameter */
const Timer_A_CaptureModeConfig captureModeConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,        // CC Register 2
        TIMER_A_CAPTUREMODE_RISING_EDGE,          // Rising Edge and falling
        TIMER_A_CAPTURE_INPUTSELECT_CCIxA,        // CCIxA Input Select
        TIMER_A_CAPTURE_SYNCHRONOUS,              // Synchronized Capture
        TIMER_A_CAPTURECOMPARE_INTERRUPT_ENABLE,  // Enable interrupt
        TIMER_A_OUTPUTMODE_OUTBITVALUE            // Output bit value
};

static void Delay(uint32_t loop)
{
    volatile uint32_t i;

    for (i = 0 ; i < loop ; i++);
}

int meas1 = 0;
int meas2 = 0;
int main(void)
{
    /* Stop watchdog timer */
    MAP_WDT_A_holdTimer();

    CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_24);                    // 24000000 Hz

    CS_initClockSignal(CS_MCLK,  CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_1); // 24000000 Hz
    CS_initClockSignal(CS_SMCLK, CS_DCOCLK_SELECT, CS_CLOCK_DIVIDER_8); //  3000000 Hz

    /* Configuring P2.4 as peripheral input for capture */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P2, GPIO_PIN4,
    GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN5);

    /* Configuring Capture Mode */
    Timer_A_initCapture(TIMER_A0_BASE, &captureModeConfig);

    /* Configuring Continuous Mode */
    Timer_A_configureContinuousMode(TIMER_A0_BASE, &continuousModeConfig);

    /* Enabling interrupts */
    Interrupt_enableInterrupt(INT_TA0_N);
    Interrupt_enableMaster();

    /* Starting the Timer32 */
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    Timer32_disableInterrupt(TIMER32_0_BASE);
    Timer32_setCount(TIMER32_0_BASE, 1);
    Timer32_startTimer(TIMER32_0_BASE, true);

    /* Starting the Timer_A0 in continuous mode */
    Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_CONTINUOUS_MODE);
    while(1)
    {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN5);
        Timer32_setCount(TIMER32_0_BASE, 24 * 10);
        while (Timer32_getValue(TIMER32_0_BASE) > 0); // Wait 10us
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN5);                //software delays
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
        Delay(40000);
    }
}

void TA0_N_IRQHandler(void)
{
    int rising  = 0;

    Timer_A_clearCaptureCompareInterrupt(TIMER_A0_BASE,
            TIMER_A_CAPTURECOMPARE_REGISTER_1);

    if(P2IN&0x10) rising=1; else rising=0;

    if(rising) // Start
    {
        meas1 = Timer_A_getCaptureCompareCount(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
    }
    else
    {
        meas2 = Timer_A_getCaptureCompareCount(TIMER_A0_BASE, TIMER_A_CAPTURECOMPARE_REGISTER_1);
    }
}
