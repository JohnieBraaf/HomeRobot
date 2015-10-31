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
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, leftTrackPower);
	}
	else if (leftTrackPower < 0 && LeftTrack_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, abs(leftTrackPower));
	}
	else
	{
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_2, 0);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_1, 0);
	}
}

void RightTrack()
{
	if (rightTrackPower > 0 && RightTrack_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, rightTrackPower);
	}
	else if (rightTrackPower < 0 && RightTrack_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, 0);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, abs(rightTrackPower));
	}
	else
	{
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim1, TIM_CHANNEL_3, 0);
	}
}

void Torso()
{
	if (torsoPower > 0 && Torso_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, 0);
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, torsoPower);
	}
	else if (torsoPower < 0 && Torso_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, abs(torsoPower));
	}
	else
	{
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_4, 0);
		__HAL_TIM_SetCompare(&htim5, TIM_CHANNEL_3, 0);
	}
}

void Arms()
{
	if (armsPower > 0 && Arms_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, armsPower);
	}
	else if (armsPower < 0 && Arms_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, abs(armsPower));
	}
	else
	{
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim3, TIM_CHANNEL_2, 0);
	}
}

void Chest()
{
	if (chestPower > 0 && Chest_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, chestPower);
	}
	else if (chestPower < 0 && Chest_RemainActive > 0)
	{
		__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, abs(chestPower));
	}
	else
	{
		__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_1, 0);
		__HAL_TIM_SetCompare(&htim9, TIM_CHANNEL_2, 0);
	}
}