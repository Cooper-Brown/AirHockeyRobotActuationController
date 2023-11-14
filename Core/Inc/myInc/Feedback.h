/*
 * Feedback.h
 *
 *  Created on: Aug 8, 2023
 *      Author: 00coo
 */

#ifndef INC_MYINC_FEEDBACK_H_
#define INC_MYINC_FEEDBACK_H_

#include "globals.h" // to get the Coordinate type

int32_t updateIntegral(int32_t error, uint32_t timeSinceLastUpdate);
void PIDControlStep(Coordinate setpoint, Coordinate currentPosition, int32_t* newSpeedX, int32_t* newSpeedY);


#endif /* INC_MYINC_FEEDBACK_H_ */
