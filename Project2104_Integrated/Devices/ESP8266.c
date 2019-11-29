#include "ESP8266.h"

#define RESET_PIN       GPIO_PIN1
#define RESET_PORT      GPIO_PORT_P6
#define AT              "AT\r\n"
#define AT_RST          "AT+RST\r\n"
#define AT_GMR          "AT+GMR\r\n"
#define AT_GSLP         "AT+GSLP\r\n"
#define ATE             "ATE\r\n"
#define AT_CWMODE       "AT+CWMODE\r\n"
#define AT_CWJAP        "AT+CWJAP"
#define AT_CWLAP        "AT+CWLAP\r\n"
#define AT_CWQAP        "AT+CWQAP\r\n"
#define AT_CWLIF        "AT+CWLIF\r\n"
#define AT_CWDHCP       "AT+CWDHCP\r\n"
#define AT_CIPSTAMAC    "AT+CIPSTAMAC\r\n"
#define AT_CIPAPMAC     "AT+CIPAPMAC\r\n"
#define AT_CIPSTA       "AT+CIPSTA\r\n"
#define AT_CIPAP        "AT+CIPAP"
#define AT_CIPSTATUS    "AT+CIPSTATUS\r\n"
#define AT_CIPSTART     "AT+CIPSTART"
#define AT_CIPSEND      "AT+CIPSEND"
#define AT_CIPCLOSE     "AT+CIPCLOSE"
#define AT_CIFSR        "AT+CIFSR\r\n"
#define AT_CIPMUX       "AT+CIPMUX"
#define AT_CIPSERVER    "AT+CIPSERVER"
#define AT_CIPMODE      "AT+CIPMODE\r\n"
#define AT_CIPSTO       "AT+CIPSTO\r\n"
#define AT_CIUPDATE     "AT+CIUPDATE\r\n"
#define IPD             "+IPD\r\n"

char ESP8266_Buffer[ESP8266_BUFFER_SIZE]; //define size of buffer

bool ESP8266_WaitForAnswer(uint32_t Tries)
{
    uint32_t c;
    uint32_t i = 0;

    while(Tries)//10
    {
        if(UART_Available(EUSCI_A2_BASE)){
            while(UART_Available(EUSCI_A2_BASE)){
                UART_Read(EUSCI_A2_BASE, (uint8_t*)&c, 1);

                if(i > ESP8266_BUFFER_SIZE) //2048
                {
                    return false;
                }
                else
                {
                    ESP8266_Buffer[i++] = c;
                }
            }

            ESP8266_Buffer[i++] = 0;
            return true;
        }
        Tries--;
        __delay_cycles(2400);
    }

    return false; //will time out
}

bool ESP8266_CheckConnection(void)  //check if we have communication link between the MSP432 and the ESP8266
{
    MSPrintf(EUSCI_A2_BASE, AT); //Test AT startup
    __delay_cycles(12000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true; //ESP8266_Buffer will return OK, communication succeeded
}

bool ESP8266_AvailableAPs(void) //API to list all the access point near you
{
    MSPrintf(EUSCI_A2_BASE, AT_CWLAP);
    __delay_cycles(48000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

bool ESP8266_ConnectToAP(char *SSID, char *Password) //API to join access point
{
    MSPrintf(EUSCI_A2_BASE, "%s=\"%s\",\"%s\"\r\n", AT_CWJAP, SSID, Password);

    __delay_cycles(24000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true; //authentication the AP was successful
    //the ESP8266 is ready to connect to a server and start sending and receiving data
}

bool ESP8266_EnableMultipleConnections(void) //API to enable for multiple connection to server
{
    MSPrintf(EUSCI_A2_BASE, "%s=1\r\n", AT_CIPMUX); //set o 1 = enable MUX mode

    __delay_cycles(40000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true; //successfully enabled
}

bool ESP8266_EstablishConnection(char ID, uint8_t type, char *address, char *port) // Establish TCP connection or register UDP port and start a connection
{
    char ct[3];

    switch(type) //get 3 char user input
    {
    case TCP:   //check if  input is TCP
        ct[0] = 'T';
        ct[1] = 'C';
        ct[2] = 'P';
        break;
    case UDP:   //check if  input is UDP
        ct[0] = 'U';
        ct[1] = 'D';
        ct[2] = 'P';
        break;
    }

    MSPrintf(EUSCI_A2_BASE, "%s=%c,\"%s\",\"%s\",%s\r\n", AT_CIPSTART, ID, ct, address, port); //In Multiple connection mode

    __delay_cycles(24000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}
bool ESP8266_EnableServer(void) //API to set up server
{

    MSPrintf(EUSCI_A2_BASE, "%s=1,80\r\n", AT_CIPSERVER); // set up server at port 80 (HTTP)

    __delay_cycles(54000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES)) //exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true; //connected to port 80
}

bool ESP8266_SendData(char ID, char *Data, uint32_t DataSize) //API to send data to server browser
{
    char size[5];

    ltoa(DataSize, size);
    MSPrintf(EUSCI_A2_BASE, "%s=0,%s\r\n", AT_CIPSEND, size); //DataSize sets the length of the message

    __delay_cycles(24000000); //20ms interval between each packet
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, ">") == NULL)
    {
        return false;
    }

    MSPrintf(EUSCI_A2_BASE, Data);

    wait(50000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true; //data sent
}
bool ESP8266_Close(void) //API to close connection
{
    MSPrintf(EUSCI_A2_BASE, "%s=0\r\n", AT_CIPCLOSE);

    __delay_cycles(54000000);
    if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
    {
        return false;
    }

    if(strstr(ESP8266_Buffer, "OK") == NULL)
    {
        return false;
    }

    return true;
}

void ESP8266_Terminal(void)
{
    while(1)
    {
        MSPgets(EUSCI_A0_BASE, ESP8266_Buffer, 128); //reads user's command from terminal
        MSPrintf(EUSCI_A2_BASE, ESP8266_Buffer); //print on uart

        __delay_cycles(48000000);
        if(!ESP8266_WaitForAnswer(ESP8266_RECEIVE_TRIES))//exit if exceed number of tries
        {
            MSPrintf(EUSCI_A0_BASE, "ESP8266 receive timeout error\r\n");
        }
        else
        {
            MSPrintf(EUSCI_A0_BASE, ESP8266_Buffer);
        }

    }
}

char *ESP8266_GetBuffer(void)
{
    return ESP8266_Buffer;
}

void ESP8266_HardReset(void) //reset ESP8266
{
    GPIO_Low(RESET_PORT, RESET_PIN); //set pin to low -stop

    __delay_cycles(24000000);

    GPIO_High(RESET_PORT, RESET_PIN);//set pin to high -restart
}

void wait(int N) //TIMER32 delay
{
    TIMER32_1->CONTROL=0XC2;
    TIMER32_1->LOAD=N-1;
    while((TIMER32_1->RIS & 1) == 0);
    TIMER32_1->INTCLR = 0;
}
