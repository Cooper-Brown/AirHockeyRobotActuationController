/*
 * myMain.c
 *
 *  Created on: Aug 3, 2023
 *      Author: 00coo
 */

#include "gpio.h"
#include "myInc/myMain.h"
#include "myInc/globals.h"
#include "myInc/MotorWriters.h"
#include "myInc/Feedback.h"
#include <stdlib.h>

// Handles to peripherals
TIM_HandleTypeDef* p_htim3;
TIM_HandleTypeDef* p_htim4;
TIM_HandleTypeDef* p_htim9;
DAC_HandleTypeDef* p_hdac;
SPI_HandleTypeDef* p_hspi2;

// Helper Functions
void updateRobotPosition(Coordinate* currentPosition){
	currentPosition->xPosition = __HAL_TIM_GET_COUNTER(p_htim3);
	currentPosition->yPosition = __HAL_TIM_GET_COUNTER(p_htim4);
	return;
}

// Vars needed in initialisation.
Coordinate lastRobotPosition, currentRobotPosition;
uint32_t lastRobotPositionUpdateTime, currentRobotPositionUpdateTime;
Coordinate targetPosition;

void myInit(DAC_HandleTypeDef* p_hdac_arg, TIM_HandleTypeDef* p_htim3_arg, TIM_HandleTypeDef* p_htim4_arg, TIM_HandleTypeDef* p_htim9_arg, SPI_HandleTypeDef* p_hspi2_arg) {
	// Disable the Motors during initialisation stage
	applySoftwareLock();

	logicEnableCommandSent = 0;
	updatedCoordinateIn = 0;

	// set up feature references
	p_hdac = p_hdac_arg;
	p_htim3 = p_htim3_arg;
	p_htim4 = p_htim4_arg;
	p_htim9 = p_htim9_arg;
	p_hspi2 = p_hspi2_arg;

	// FEATURE INIT
	HAL_TIM_Encoder_Start(p_htim3, TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start(p_htim4, TIM_CHANNEL_ALL);
	HAL_TIM_Base_Start_IT(p_htim9);
	HAL_DAC_Start(p_hdac, DAC_CHANNEL_1); // x motor DAC
	HAL_DAC_Start(p_hdac, DAC_CHANNEL_2); // y motor DAC

	// Global Variable Instantiation
	xRotCount = 0;
	yRotCount = 0;

	//HAL_Delay(1);

	// Wait on signal from logic center to continue
	//logicEnableCommandSent = 1;
	while (!logicEnableCommandSent){
		HAL_Delay(10);
	}

	// Set encoder positions to start at 2000 ticks. This will prevent the timers overflowing the other direction when the table edge is met
	__HAL_TIM_SET_COUNTER(p_htim3, 2000);
	__HAL_TIM_SET_COUNTER(p_htim4, 2000);

	// Use encoder timers to get the last known position.
	updateRobotPosition(&lastRobotPosition);
	lastRobotPositionUpdateTime = HAL_GetTick();

	targetPosition.xPosition = lastRobotPosition.xPosition;
	targetPosition.yPosition = lastRobotPosition.yPosition;

	disableSoftwareLock();
	enableXMotor();
	enableYMotor();

	//targetPosition.xPosition = 7500;
	//targetPosition.yPosition = 2000;

}

// Vars needed for the main loop.
int32_t xTravel, yTravel;
uint32_t travelTime;
int32_t xDistanceToTargetPosition, yDistanceToTargetPosition;
float actualXSpeed, actualYSpeed;
int32_t newXSpeed, newYSpeed;
uint32_t estopHighOccurances = 0;

void myLoopInternals(){
	// Detect if the E-Stop was pressed. Occurrences counted to filter for noisy connections.
	if (HAL_GPIO_ReadPin(GPIOE, ESTOP_Pin)){
		estopHighOccurances ++;
		if (estopHighOccurances > 5){
			applySoftwareLock();
		}
	}
	else{
		estopHighOccurances = 0;
	}

	// Get the new robot position, record time of update.
	updateRobotPosition(&currentRobotPosition);
	currentRobotPositionUpdateTime = HAL_GetTick();

	// Calculate the robot's speed in ticks/ms.
	travelTime = currentRobotPositionUpdateTime - lastRobotPositionUpdateTime;
	if (travelTime == 0){
		return;
	}
	xTravel = ((int32_t)currentRobotPosition.xPosition) - ((int32_t)lastRobotPosition.xPosition);
	yTravel = ((int32_t)currentRobotPosition.yPosition) - ((int32_t)lastRobotPosition.yPosition);
	actualXSpeed = xTravel / ((float) travelTime);
	actualYSpeed = yTravel / ((float) travelTime);

	PIDControlStep(targetPosition, currentRobotPosition, &newXSpeed, &newYSpeed);

	// Update the speed. Current position needed to protect against over-running the stops.
	setXSpeed(newXSpeed, currentRobotPosition);
	setYSpeed(newYSpeed, currentRobotPosition);

	// OLD CONTROL LOOP
	/*
	// Calculate the distance left to travel.
	xDistanceToTargetPosition = (int32_t)(targetPosition.xPosition) - (int32_t)(currentRobotPosition.xPosition);
	yDistanceToTargetPosition = (int32_t)(targetPosition.yPosition) - (int32_t)(currentRobotPosition.yPosition);


	// Figure out what speed the robot should be set to.
	// X
	if (abs(xDistanceToTargetPosition) < 300){
		newXSpeed = (1.0/2.0)*xDistanceToTargetPosition;
	}
	else {
		newXSpeed = 500*sign(xDistanceToTargetPosition);
	}
	// Y
	if (abs(yDistanceToTargetPosition) < 500){
		newYSpeed = (1.0/4.0)*yDistanceToTargetPosition;
	}
	else {
		newYSpeed = 400*sign(yDistanceToTargetPosition);
	}
	*/




	// Check for any updates to the instruction coordinates
	if (updatedCoordinateIn){
		targetPosition = targetCoordinateIn;
		updatedCoordinateIn = 0;
	}

	// Retain information for the next loop iteration.
	lastRobotPosition = currentRobotPosition;
	lastRobotPositionUpdateTime = currentRobotPositionUpdateTime;

	return;
}
