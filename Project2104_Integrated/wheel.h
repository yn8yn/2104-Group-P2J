/*
 * wheel.h
 *
 *  Created on: 28 Oct 2019
 *      Author: adilah
 */

#include "msp.h"
#include "MapManagement.h"

#ifndef WHEEL_H_
#define WHEEL_H_

/*
Motor Control API
*/


void init_wheel(uint16_t inputL, uint16_t inputR,uint16_t inputAL, uint16_t inputAR,uint16_t pwma,uint16_t pwmb,uint16_t stby);
void forward();
void backward();
void turnright();
void turnleft();
void stop();
void forwardlp();
void pwm_init();
#endif /* WHEEL_H_ */
