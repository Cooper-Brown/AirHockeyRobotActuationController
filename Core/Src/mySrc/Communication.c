
#include "myInc\Communication.h"

// Used locally to decode inputs.
const char* xLeftCommand = "x.l";
const char* xRightCommand = "x.r";
const char* xStopCommand = "x.s";
const char* yUpCommand = "y.u";
const char* yDownCommand = "y.d";
const char* yStopCommand = "y.s";

const char* enableCommand = "E";
const char* startOfCoordinateCommand = "C";
const char* resetCommand = "R";

#define TABLE_X_BOUNDARY_MIN 2000
#define TABLE_X_BOUNDARY_MAX 15100

void receiveTargetCoordinate(char* commandInput){
	// commandFormat - C:xInt:yInt:
	char xCoordinateChar[10];
	char yCoordinateChar[10];
	int xCoordinateStartIndex = 2;
	int commandIndex = xCoordinateStartIndex;
	while (commandInput[commandIndex] != ':'){
		xCoordinateChar[commandIndex - xCoordinateStartIndex] = commandInput[commandIndex];
		commandIndex++;
	}
	xCoordinateChar[commandIndex - xCoordinateStartIndex] = '\0';
	commandIndex++; // Skip past the : character
	int yCoordinateStartIndex = commandIndex;
	while (commandInput[commandIndex] != ':'){
		yCoordinateChar[commandIndex - yCoordinateStartIndex] = commandInput[commandIndex];
		commandIndex++;
	}
	yCoordinateChar[commandIndex - yCoordinateStartIndex] = 0;

	int xPositionInt = 0;
	int yPositionInt = 0;
	sscanf(xCoordinateChar, "%du", &xPositionInt);
	sscanf(yCoordinateChar, "%du", &yPositionInt);
	uint16_t xPosition = (uint16_t) xPositionInt;
	uint16_t yPosition = (uint16_t) yPositionInt;

	if (xPosition < TABLE_X_BOUNDARY_MIN){
		xPosition = TABLE_X_BOUNDARY_MIN;
	}
	if (xPosition > TABLE_X_BOUNDARY_MAX) {
		xPosition = TABLE_X_BOUNDARY_MAX;
	}
	if (yPosition < TABLE_Y_BOUNDARY_MIN){
		yPosition = TABLE_Y_BOUNDARY_MIN;
	}
	if (yPosition > TABLE_Y_BOUNDARY_MAX) {
		yPosition = TABLE_Y_BOUNDARY_MAX;
	}
	targetCoordinateIn.xPosition = xPosition;
	targetCoordinateIn.yPosition = yPosition;
	updatedCoordinateIn = 1;
	return;
}

// TODO decode the command and add it to a buffer to be processed when the time is right
void commandReceived(char* charAccumulator, uint8_t commandSize){
	char commandInput[commandSize+1];
	int c;
	for (c = 0; c < commandSize; c++)
		commandInput[c] = charAccumulator[c];
	commandInput[c] = '\0';

	if (!strcmp(enableCommand, commandInput)){
		logicEnableCommandSent = 1;
	}

	if (commandInput[0] == 67){
		receiveTargetCoordinate(commandInput);
	}
	if (!strcmp(resetCommand, commandInput)){
		HAL_NVIC_SystemReset();
	}

	/*
	else if (!strcmp(xRightCommand, commandInput)){
		x_goRight();
	}
	else if (!strcmp(xStopCommand, commandInput)){
		x_stop();
	}
	else if (!strcmp(yUpCommand, commandInput)){
		y_goUp();
	}
	else if (!strcmp(yDownCommand, commandInput)){
		y_goDown();
	}
	else if (!strcmp(yStopCommand, commandInput)){
		y_stop();
	}
	*/
	return;
}
