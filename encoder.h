/*
 * encoder.h
 *
 *  Created on: 24 Oct 2019
 *      Author: adilah
 */
#include "msp.h"
#include "MapManagement.h"
#ifndef ENCODER_H_
#define ENCODER_H_


 void encoder_init();
 void encoder_right(void);
 float getSpeed(void);
 void minuteDelay(void);
 void changeRev(void);


#endif /* ENCODER_H_ */
