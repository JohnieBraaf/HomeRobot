#include <movement.h>
#include <stm32f4xx_hal.h>

extern TIM_HandleTypeDef htim4;

int rightTrackActive = 0;
int leftTrackActive = 0;

void RightTrack(int speed, int direction, int miliseconds)
{
	if (direction == 48)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 1500);
	}
	else if (direction == 49)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 1500);
	}
	else
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
	}
}

void LeftTrack(int speed, int direction, int miliseconds)
{
	
}