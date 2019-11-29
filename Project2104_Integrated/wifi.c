/*
 * wifi.c
 *
 *  Created on: 28 Nov 2019
 *      Author: Ramya & Yi Ning
 */

#include <wifi.h>

/*Pointer to ESP8266 global buffer*/
char *ESP8266_Data;

void delayMsWifi(int n);

/* UART Configuration Parameter. These are the configuration parameters to
 * make the eUSCI A UART module to operate with a 115200 baud rate. These
 * values were calculated using the online calculator that TI provides
 * at:
 * http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 */
eUSCI_UART_ConfigV1 UART2Config = //configure UART2
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     13,
     0,
     37,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};
eUSCI_UART_ConfigV1 UART0Config = //configure UART0
{
     EUSCI_A_UART_CLOCKSOURCE_SMCLK,
     13,
     0,
     37,
     EUSCI_A_UART_NO_PARITY,
     EUSCI_A_UART_LSB_FIRST,
     EUSCI_A_UART_ONE_STOP_BIT,
     EUSCI_A_UART_MODE,
     EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION
};


void initWIFIModule(void){
    ESP8266_Data = ESP8266_GetBuffer();

    MAP_WDT_A_holdTimer();

    /*MSP432 Running at 24 MHz*/
    CS_Init();

    /*Initialize required hardware peripherals for the ESP8266*/
    UART_Init(EUSCI_A0_BASE, UART0Config);
    UART_Init(EUSCI_A2_BASE, UART2Config);
    GPIO_Init(GPIO_PORT_P6, GPIO_PIN1);

    MAP_Interrupt_enableMaster();

    /*Hard Reset ESP8266*/
    ESP8266_HardReset();
    __delay_cycles(48000000);

    /*flush reset data, we do this because a lot of data received cannot be printed*/
    UART_Flush(EUSCI_A2_BASE);
}

void setupWIFI(void){
    //  ESP8266_Terminal(); //Terminal to set up configurations manually
    while(!ESP8266_ConnectToAP("SIT-GUEST","3UIcF@uC")) //Keep trying to connect to wifi
    {
        MSPrintf(EUSCI_A0_BASE,ESP8266_Data); //AT+CWJAP
    }

    MSPrintf(EUSCI_A0_BASE, "Success MSP432 connected to SIT -GUEST\r\n"); //connected to wifi

    __delay_cycles(48000000); //delay to allow data in buffer to be fully transmitted

   if(ESP8266_EnableMultipleConnections()) //set ESP8266 to allow for multiple connections (1)
   {
       MSPrintf(EUSCI_A0_BASE,ESP8266_Data); //AT+CIPMUX
       MSPrintf(EUSCI_A0_BASE, "Success MSP432  MUX connection ok\r\n");
   }
   else //Fail (0) to set multiple connections to 1
   {
       MSPrintf(EUSCI_A0_BASE, "MSP432  MUX connection failed \r\n");
   }

   __delay_cycles(48000000); //delay to allow data in buffer to be fully transmitted

   while(!ESP8266_EnableServer()) // keep trying to enable and connect to server
   {
      MSPrintf(EUSCI_A0_BASE,ESP8266_Data); // AT+CIPSERVER fails
   }
   MSPrintf(EUSCI_A0_BASE,ESP8266_Data); //success
   MSPrintf(EUSCI_A0_BASE, "Success server enabled at port 80\r\n");

   __delay_cycles(48000000);
   delayMsWifi(6000);
   printf("set up wifi done");

}
void sendmap(char *mapdata)
{
    //takes map array with length 419 (static for demo)
    ESP8266_SendData(0,mapdata,419);
    MSPrintf(EUSCI_A0_BASE,ESP8266_Data); //AT+CIPSEND
    MSPrintf(EUSCI_A0_BASE, "Sent\r\n"); //data is sent successfully

    __delay_cycles(48000000);// delay to allow for data to be sent to browser before closing

    while(!ESP8266_Close()) //not close, wait
    {
       MSPrintf(EUSCI_A0_BASE,ESP8266_Data); //AT+CIPCLOSE
    }

    MSPrintf(EUSCI_A0_BASE,ESP8266_Data);
    MSPrintf(EUSCI_A0_BASE, "closed success\r\n"); //close success
    __delay_cycles(48000000);  //delay to allow data in buffer to be fully transmitted

}

void delayMsWifi(int n) //Timer32_1 delay
{
    int i;

    TIMER32_1->LOAD = 3000 - 1;// ---------------------------------------change to timer a2
    TIMER32_1->CONTROL = 0xC2; //- register T32, bit 1100 0010,

    for(i = 0; i < n; i++) {
        while((TIMER32_1->RIS & 1) == 0); /* wait until the RAW_IFG is set  - start from 2999, starts counting until 0*/
        TIMER32_1->INTCLR = 0;            /* clear RAW_IFG flag  - once it hits 0, the flag will go down. clear the flag*/
    }
}

