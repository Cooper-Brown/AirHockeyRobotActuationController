/*
 * Callbacks.h
 *
 *  Created on: Aug 27, 2023
 *      Author: 00coo
 */

#ifndef INC_MYINC_CALLBACKS_H_
#define INC_MYINC_CALLBACKS_H_

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi);
void my_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);

#endif /* INC_MYINC_CALLBACKS_H_ */
