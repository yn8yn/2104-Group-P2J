/*
 * wifi.h
 *
 *  Created on: 28 Nov 2019
 *      Author: adilah
 */

#ifndef WIFI_H_
#define WIFI_H_
#include <Hardware/CS_Driver.h>
#include <Devices/MSPIO.h>
#include <Devices/ESP8266.h>

void initWIFIModule(void);
void setupWIFI(void);
void sendmap(char *mapdata);


#endif /* WIFI_H_ */
