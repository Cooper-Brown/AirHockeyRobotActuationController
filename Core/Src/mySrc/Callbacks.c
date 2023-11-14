
#include "myInc/globals.h"
#include "myInc/Callbacks.h"
#include "myInc/MotorWriters.h"

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef *hspi){
	return;
}

void my_HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	// Redundant timer for the time being
	if (htim->Instance == TIM9){
		return;
	}
}

uint32_t xRotCount;
uint32_t yRotCount;
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == Xz_Pin){
		xRotCount++;
	}
	// y encoder
	else if (GPIO_Pin == Yz_Pin){
		yRotCount++;
	}
	else if (GPIO_Pin == ESTOP_Pin){
		//applySoftwareLock();
		// Indicator LED??
		// HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);
	}
}
