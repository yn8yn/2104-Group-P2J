/*
 * Ultrasonic.h
 *
 *  Created on: 27 Nov 2019
 *      Author: Jerry
 */

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_


#include "MapManagement.h"
int continues;


/*
initilise and create Ultrasonic sensor monitoring task

*/
void US_init(void);
void callTrigger();
int getAverageDistance(int distance[10]);
void delayCycles(int n);
void PORT1_IRQHandler(void);
void TA0_0_IRQHandler(void);
void checkDistance(int average_distance, int distance);

#endif /* ULTRASONIC_H_ */
