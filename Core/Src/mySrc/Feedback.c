/*
 * Feedback.c
 *
 *  Created on: Aug 8, 2023
 *      Author: 00coo
 */

#include "myInc/globals.h"
#include "myInc/Feedback.h"
#include <stdlib.h>


int32_t sign(int32_t x) {
    return (x > 0) - (x < 0);
}

// INTEGRATORS //
// X Integrator
float lastError_X = 0;
uint32_t lastUpdateTicks_X;
float rollingIntegral_X = 0;
uint8_t integralFirstUpdateNeeded_X = 1;
float integrationStep_X(float error){
	if (integralFirstUpdateNeeded_X) {
		lastUpdateTicks_X = HAL_GetTick();
		integralFirstUpdateNeeded_X = 0;
		return 0;
	}
	uint32_t currentTicks = HAL_GetTick();
	rollingIntegral_X += ((error + lastError_X) * ((float)(currentTicks-lastUpdateTicks_X))) / 2.0;

	// anti-windup
	if (rollingIntegral_X > 100){
		rollingIntegral_X = 100;
	}
	else if (rollingIntegral_X < -100){
		rollingIntegral_X = -100;
	}

	// saving data for next loop
	lastError_X = error;
	lastUpdateTicks_X = currentTicks;
	return rollingIntegral_X;
}

// Y Integrator
float lastError_Y = 0;
uint32_t lastUpdateTicks_Y;
float rollingIntegral_Y = 0;
uint8_t integralFirstUpdateNeeded_Y = 1;
float integrationStep_Y(float error){
	if (integralFirstUpdateNeeded_Y) {
		lastUpdateTicks_Y = HAL_GetTick();
		integralFirstUpdateNeeded_Y = 0;
		return 0;
	}
	uint32_t currentTicks = HAL_GetTick();
	rollingIntegral_Y += error;//((error + lastError_Y) * ((float)(currentTicks-lastUpdateTicks_Y))) / 2.0;

	// anti-windup
	if (rollingIntegral_Y > 100){
		rollingIntegral_Y = 100;
	}
	else if (rollingIntegral_Y < -100){
		rollingIntegral_Y = -100;
	}

	lastError_Y = error;
	lastUpdateTicks_Y = currentTicks;
	return rollingIntegral_Y;
}

float x_P = 0.5; // 0.5
float x_I = 0.0;
float x_D = 0.5; // 0.5

float y_P = 0.85; // 0.85
float y_I = 0.03; // 0.03
float y_D = 0.9; // 0.9

int32_t xTopSpeed = 3616;
int32_t yTopSpeed = 2000;
float lastXError = 0;
float lastYError = 0;
void PIDControlStep(Coordinate setpoint, Coordinate currentPosition, int32_t* newSpeedX, int32_t* newSpeedY){
	// Get error between -100 and 100
	int xErrorTicks = (int32_t)(setpoint.xPosition) - (int32_t)(currentPosition.xPosition);
	int yErrorTicks = (int32_t)(setpoint.yPosition) - (int32_t)(currentPosition.yPosition);
	float xError = 100.0 * (xErrorTicks/((float)(TABLE_X_BOUNDARY_MAX - TABLE_X_BOUNDARY_MIN)));
	float yError = 100.0 * (yErrorTicks/((float)(TABLE_Y_BOUNDARY_MAX - TABLE_Y_BOUNDARY_MIN)));

	// Calculate new x speed as % of top speed
	float xSpeedPercent = 0;
	xSpeedPercent += x_P * xError;
	xSpeedPercent += x_I * integrationStep_X(xError);
	xSpeedPercent += x_D * (xError - lastXError);
	lastXError = xError;
	if (xSpeedPercent > 100){xSpeedPercent = 100;}
	if (xSpeedPercent < -100){xSpeedPercent = -100;}

	// Calculate new y speed as % of top speed
	float ySpeedPercent = 0;
	ySpeedPercent += y_P * yError;
	ySpeedPercent += y_I * integrationStep_Y(yError);
	ySpeedPercent += y_D * (yError - lastYError);
	lastYError = yError;
	if (ySpeedPercent > 100){ySpeedPercent = 100;}
	if (ySpeedPercent < -100){ySpeedPercent = -100;}

	// Convert the % speeds to absolute units.
	*newSpeedX = (xSpeedPercent/100.0) * xTopSpeed;
	*newSpeedY = (ySpeedPercent/100.0) * yTopSpeed;

	if (abs(*newSpeedX) > xTopSpeed){*newSpeedX = sign(*newSpeedX)*xTopSpeed;}
	if (abs(*newSpeedY) > yTopSpeed){*newSpeedY = sign(*newSpeedY)*yTopSpeed;}
}



/* LEGACY CODE TRYING TO FIX MOTOR SPEEDS TO THE ENCODER SPEEDS IN TICKS/MS
float targetXSpeed, targetYSpeed;
float speedXError, 	speedYError;
// THE GOAL IS TO MAINTAIN TRAVEL AT SOME TICKS/SEC UNTIL WE REACH THE TARGET.
// 1. FIGURE OUT WHAT THE TARGET SPEED SHOULD BE TO GET TO THE COORDINATE
//		BASED ON HOW CLOSE THE TARGET IS
// Units for the target speed are ticks/ms
if (abs(xDistanceToTargetPosition) < SPEED_TAPER_DISTANCE){
	targetXSpeed = (X_STEADY_SPEED_TICKSpMS*xDistanceToTargetPosition)/SPEED_TAPER_DISTANCE;
}
else { // ALWAYS TRAVEL AT 1 ticks/ms
	targetXSpeed = (float) X_STEADY_SPEED_TICKSpMS*sign(xDistanceToTargetPosition); // steady velocity is 1000 ticks/sec = 1 ticks/ms
}
if (abs(yDistanceToTargetPosition) < SPEED_TAPER_DISTANCE){
	targetYSpeed = (Y_STEADY_SPEED_TICKSpMS*yDistanceToTargetPosition)/SPEED_TAPER_DISTANCE;
}
else {
	targetYSpeed = (float) Y_STEADY_SPEED_TICKSpMS*sign(yDistanceToTargetPosition);
}
speedXError = targetXSpeed - actualXSpeed;
speedYError = targetYSpeed - actualYSpeed;
newXSpeed = 200*targetXSpeed + PID_X_P * ((int32_t) speedXError);
newYSpeed = 100*targetYSpeed + PID_Y_P * ((int32_t) speedYError);
*/
