/*
 * MotorWriters.h
 *
 *  Created on: Aug 7, 2023
 *      Author: 00coo
 */

#ifndef INC_MYINC_MOTORWRITERS_H_
#define INC_MYINC_MOTORWRITERS_H_

#include "main.h"
#include "globals.h"

#define DAC_MAX_VALUE 0xFFF
#define DAC_MIN_VALUE 0x000
// #define X_HIGH_RESISTANCE_DAC_OFFSET 150
// #define Y_HIGH_RESISTANCE_DAC_OFFSET 150 // this won't matter as much, as low DAC voltage only hampers the top speed of the Y axis

void applySoftwareLock();
void disableSoftwareLock();

void enableXMotor();
void disableXMotor();
void enableYMotor();
void disableYMotor();

// Set X Speed from -12bit to +12bit
uint8_t setXSpeed(int32_t speed, Coordinate currentRobotPosition);
// Set Y Speed from -12bit to +12bit. NOTE: The resolution is actually 1/2 of this
uint8_t setYSpeed(int32_t speed, Coordinate currentRobotPosition);

#endif /* INC_MYINC_MOTORWRITERS_H_ */
