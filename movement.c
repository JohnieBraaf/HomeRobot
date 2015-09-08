#include <movement.h>
#include <stm32f4xx_hal.h>

extern TIM_HandleTypeDef htim4;

// switch 0-1
int rightTrackForward	= 1;
int leftTrackForward	= 1;
int torsoForward		= 1;
int armsOpen			= 1;
int chestCW				= 1;

// range 0-255
int rightTrackPower		= 0;
int leftTrackPower		= 0;
int torsoPower			= 0;
int armsPower			= 0;
int chestPower			= 0;

void RightTrack()
{
	if (rightTrackForward == 1)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, rightTrackPower);
	}
	else if (rightTrackForward == 0)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, rightTrackPower);
	}
	else
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
	}
}

void LeftTrack()
{
	if (leftTrackForward == 1)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, leftTrackPower);
	}
	else if (leftTrackForward == 0)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, leftTrackPower);
	}
	else
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
	}
}