/*
 * Communication.h
 *
 *  Created on: Aug 27, 2023
 *      Author: 00coo
 */

#ifndef INC_MYINC_COMMUNICATION_H_
#define INC_MYINC_COMMUNICATION_H_

#include <stdio.h>
#include <string.h>

#include "globals.h"

uint8_t logicEnableCommandSent;

Coordinate targetCoordinateIn;
uint8_t updatedCoordinateIn;

void commandReceived(char* charAccumulator, uint8_t commandSize);

#endif /* INC_MYINC_COMMUNICATION_H_ */
