/*
 * myMain.h
 *
 *  Created on: Aug 3, 2023
 *      Author: 00coo
 */

#ifndef INC_MYINC_MYMAIN_H_
#define INC_MYINC_MYMAIN_H_

#include "main.h"
#include "myInc/globals.h"

extern int32_t spiRxCount9;

void myInit(
	DAC_HandleTypeDef* p_hdac_arg,
	TIM_HandleTypeDef* p_htim3_arg,
	TIM_HandleTypeDef* p_htim4_arg,
	TIM_HandleTypeDef* p_htim9_arg,
	SPI_HandleTypeDef* p_hspi2_arg
);

void myLoopInternals();

void my_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);

#endif /* INC_MYINC_MYMAIN_H_ */
