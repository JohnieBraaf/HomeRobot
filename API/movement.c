#include <movement.h>
#include <stm32f4xx_hal.h>

extern TIM_HandleTypeDef htim4;
extern __IO uint32_t LeftTrack_RemainActive;
extern __IO uint32_t RightTrack_RemainActive;
extern __IO uint32_t Torso_RemainActive;
extern __IO uint32_t Arms_RemainActive;
extern __IO uint32_t Chest_RemainActive;

// range 255-255
int rightTrackPower		= 0;
int leftTrackPower		= 0;
int torsoPower			= 0;
int armsPower			= 0;
int chestPower			= 0;

void LeftTrack()
{
	if (leftTrackPower > 0 && LeftTrack_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_2, leftTrackPower);
	}
	else if (leftTrackPower < 0 && LeftTrack_RemainActive > 0)
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

void RightTrack()
{
	if (rightTrackPower > 0 && RightTrack_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_3, 0);
		__HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, rightTrackPower);
	}
	else if (rightTrackPower < 0 && RightTrack_RemainActive > 0)
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