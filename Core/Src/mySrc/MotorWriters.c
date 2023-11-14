/*
 * MotorWriters.c
 *
 *  Created on: Aug 7, 2023
 *      Author: 00coo
 */

#include "myInc/MotorWriters.h"
#include <stdlib.h>

uint8_t softwareMotorLock;
void applySoftwareLock(){
	disableYMotor();
	disableXMotor();
	softwareMotorLock = 0x0F;
}
void disableSoftwareLock(){
	softwareMotorLock = 0x00;
}



void enableXMotor(){
	if (!softwareMotorLock){
		HAL_GPIO_WritePin(GPIOD, Xen_Pin, GPIO_PIN_RESET);
	}
}
void disableXMotor(){
	HAL_GPIO_WritePin(GPIOD, Xen_Pin, GPIO_PIN_SET);
}



void enableYMotor(){
	if (!softwareMotorLock){
		HAL_GPIO_WritePin(GPIOA, Yen_Pin, GPIO_PIN_SET);
	}

}
void disableYMotor(){
	HAL_GPIO_WritePin(GPIOA, Yen_Pin, GPIO_PIN_RESET);
}

int32_t currentXSpeed;
uint8_t currentXDirection;
uint16_t XSpeedTwelveBit;
uint8_t boundaryWasOverrun = 0;
uint8_t boundaryOverrunIllegalDirection;

// Set X Speed from -2^12 to +2^12
uint8_t setXSpeed(int32_t speed, Coordinate currentRobotPosition){
	currentXSpeed = speed;
	currentXDirection = (currentXSpeed >= 0);
	if (boundaryWasOverrun && (boundaryOverrunIllegalDirection != currentXDirection)){
		enableXMotor();
		boundaryWasOverrun = 0;
	}

	// Handle boundary overrun checks
	if (	(!currentXDirection && (currentRobotPosition.xPosition < TABLE_X_BOUNDARY_MIN)) ||
			(currentXDirection && (currentRobotPosition.xPosition > TABLE_X_BOUNDARY_MAX)) ) {
		currentXSpeed = 0;
		// for now, disable motor to get around the offset issue
		disableXMotor();
		boundaryWasOverrun = 1;
	}

	// Get the twelve bit number to give to the DAC
	XSpeedTwelveBit = (uint16_t)abs(currentXSpeed);

	// Correct any high cable resistance offset, avoiding overflow.
	/*
	if (XSpeedTwelveBit < (AXIS_MAX_POS_SPEED - X_HIGH_RESISTANCE_DAC_OFFSET)){
		XSpeedTwelveBit += X_HIGH_RESISTANCE_DAC_OFFSET;
	}
	else{
		XSpeedTwelveBit = AXIS_MAX_POS_SPEED;
	}
	*/

	// Assume DAC has started
	HAL_GPIO_WritePin(GPIOA, Xdir_Pin, currentXDirection);
	HAL_DAC_SetValue(p_hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, XSpeedTwelveBit);
	return 1;
}


// Set Y Speed from -2^12 to +2^12
int32_t currentYSpeed;
uint8_t currentYDirection;
uint16_t YSpeedTwelveBit;
uint8_t setYSpeed(int32_t speed, Coordinate currentRobotPosition){
	currentYSpeed = speed;
	currentYDirection = currentYSpeed >= 0;

	// Handle boundary overrun checks
	if (	(!currentYDirection && (currentRobotPosition.yPosition < TABLE_Y_BOUNDARY_MIN)) ||
			(currentYDirection && (currentRobotPosition.yPosition > TABLE_Y_BOUNDARY_MAX)) ) {
		currentYSpeed = 0;
	}

	// Get the twelve bit number to give to the DAC
	YSpeedTwelveBit = (uint16_t)abs(currentYSpeed);
	if (currentYDirection){
		YSpeedTwelveBit = 2048 + (YSpeedTwelveBit>>1);
	}
	else{
		YSpeedTwelveBit = 2048 - (YSpeedTwelveBit>>1);
	}

	// Correct any high cable resistance offset, avoiding overflow.
	/*
	if (YSpeedTwelveBit < (AXIS_MAX_POS_SPEED - Y_HIGH_RESISTANCE_DAC_OFFSET)){
		YSpeedTwelveBit += Y_HIGH_RESISTANCE_DAC_OFFSET;
	}
	else{
		YSpeedTwelveBit = AXIS_MAX_POS_SPEED;
	}
	*/

	// Assume DAC has started
	HAL_DAC_SetValue(p_hdac, DAC_CHANNEL_2, DAC_ALIGN_12B_R, YSpeedTwelveBit);
	return 1;
}
