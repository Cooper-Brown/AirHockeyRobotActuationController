/*
 * globals.h
 *
 *  Created on: Aug 3, 2023
 *      Author: 00coo
 */

#ifndef INC_MYINC_GLOBALS_H_
#define INC_MYINC_GLOBALS_H_


#define AXIS_OFF_SPEED (int16_t)0x000
#define AXIS_MAX_POS_SPEED (int16_t)0xFFF
#define AXIS_MAX_NEG_SPEED (int16_t)-0xFFF // TODO is this cast doing what is expected?

#define TABLE_X_BOUNDARY_MIN 2000
#define TABLE_X_BOUNDARY_MAX 15100
#define TABLE_Y_BOUNDARY_MIN 2000
#define TABLE_Y_BOUNDARY_MAX 17300

#include "main.h" // needed for HAL data types

typedef struct{
	uint16_t xPosition;
	uint16_t yPosition;
}Coordinate;

extern TIM_HandleTypeDef* p_htim3;
extern TIM_HandleTypeDef* p_htim4;
extern TIM_HandleTypeDef* p_htim9;
extern DAC_HandleTypeDef* p_hdac;

extern uint32_t xRotCount;
extern uint32_t yRotCount;

extern Coordinate lastRobotPosition;
extern Coordinate currentRobotPosition;
extern Coordinate targetPosition;

extern uint8_t logicEnableCommandSent;
extern Coordinate targetCoordinateIn;
extern uint8_t updatedCoordinateIn;

#endif /* INC_MYINC_GLOBALS_H_ */
