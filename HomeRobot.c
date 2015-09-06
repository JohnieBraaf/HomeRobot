#include <stm32f4xx_hal.h>
#include <config_system.h>
#include <config_usb.h>
#include <config_timer.h>
#include <config_gpio.h>

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

UART_HandleTypeDef huart6;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	USB_CDC_Confg();
/*
	PA2:	Torso FW		TIM5_CH3, TIM2_CH3, TIM9_CH1
	PA3:	Torso BW		TIM5_CH4, TIM2_CH4, TIM9_CH2
	PB4:	Arms Open		TIM3_CH1
	PB5:	Arms Close		TIM3_CH2
	PE5:	Chest CW		TIM9_CH1
	PE6:	Chest CCW		TIM9_CH2
	PE9:	Right Track FW	TIM1_CH1
	PE11:	Right Track BW	TIM1_CH2
	PE13:	Left Track FW	TIM1_CH3
	PE14:	Left Track BW	TIM1_CH4
*/
	
	MX_GPIO_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM5_Init();
	MX_TIM9_Init();
	MX_USART6_UART_Init();

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	int pwm = 0;
	char byte;
	for (;;)
	{
		for (pwm = 1000; pwm <= 1850; pwm++)
		{
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pwm);
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pwm);
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pwm);
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, pwm);
			HAL_Delay(5);
		}
		HAL_Delay(25);
		for (pwm = 1850; pwm >= 1000; pwm--)
		{
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, pwm);
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, pwm);
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, pwm);
			__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, pwm);
			HAL_Delay(5);
		}
		HAL_Delay(25);
		if (VCP_read(&byte, 1) != 1)
			continue;
		VCP_write("\r\nYou typed ", 12);
		VCP_write(&byte, 1);
		VCP_write("\r\n", 2);
	}
}