#include "protocol.h"
#include "main.h"
#include "stm32g0xx_hal.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;

void stop_motors(void) {
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_4);
	HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void start_motors(void) {
	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	 HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	 HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);

}
