#include <config_system.h>
#include <stm32f4xx_hal.h>

void SystemClock_Config(void)
{
	RCC_ClkInitTypeDef RCC_ClkInitStruct;
	RCC_OscInitTypeDef RCC_OscInitStruct;
	
	/*
	Clock configuration:
		Source:		8Mhz external oscillator
		Speed:		168.000.000	cycles	1 second
			/1000	168.000		cycles	1 milisecond  <-- Systick interval
			/1000	168			cycles	1 microsecond
	*/
	
	__PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
	RCC_OscInitStruct.PLL.PLLN = 336;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&RCC_OscInitStruct);

	RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK |
	                               RCC_CLOCKTYPE_PCLK1 |
	                               RCC_CLOCKTYPE_PCLK2);

	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
	HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5);

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

__IO uint32_t LeftTrack_RemainActive;
__IO uint32_t RightTrack_RemainActive;
__IO uint32_t Torso_RemainActive;
__IO uint32_t Arms_RemainActive;
__IO uint32_t Chest_RemainActive;
__IO uint32_t Audio_tone;
void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
	
	if (LeftTrack_RemainActive != 0) LeftTrack_RemainActive--;
	if (RightTrack_RemainActive != 0) RightTrack_RemainActive--;
	if (Torso_RemainActive != 0) Torso_RemainActive--;
	if (Arms_RemainActive != 0) Arms_RemainActive--;
	if (Chest_RemainActive != 0) Chest_RemainActive--;
	if (Audio_tone != 0) Audio_tone--;
}