#include <config_i2c.h>

I2C_HandleTypeDef hi2c1;

void MX_I2C1_Init(void)
{
	hi2c1.Instance = I2C1;
	hi2c1.Init.ClockSpeed = 100000;
	hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
	hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLED;
	hi2c1.Init.OwnAddress1 = 0x33;
	hi2c1.Init.OwnAddress2 = 0x33;
	hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLED;
	hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLED;
	HAL_I2C_Init(&hi2c1);
	
	//HAL_I2CEx_AnalogFilter_Config(&hi2c1, I2C_ANALOGFILTER_ENABLED);
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if (hi2c->Instance == I2C1)
	{
	    /**I2C1 GPIO Configuration    
	    PB6     ------> I2C1_SCL
	    PB9     ------> I2C1_SDA 
	    */
		GPIO_InitStruct.Pin = GPIO_PIN_6 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	    /* Peripheral clock enable */
		__I2C1_CLK_ENABLE();
		
		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(I2C1_EV_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(I2C1_EV_IRQn);
		
		HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	}
	else if (hi2c->Instance == I2C3)
	{
	    /**I2C3 GPIO Configuration    
	    PC9     ------> I2C3_SDA
	    PA8     ------> I2C3_SCL 
	    */
		GPIO_InitStruct.Pin = GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_8;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_InitStruct.Alternate = GPIO_AF4_I2C3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* Peripheral clock enable */
		__I2C3_CLK_ENABLE();
		
		/* Peripheral interrupt init*/
		HAL_NVIC_SetPriority(I2C3_EV_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(I2C3_EV_IRQn);
		
		HAL_NVIC_SetPriority(I2C3_ER_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(I2C3_ER_IRQn);
	}
}

void HAL_I2C_MspDeInit(I2C_HandleTypeDef* hi2c)
{
	if (hi2c->Instance == I2C1)
	{
	    /* Peripheral clock disable */
		__I2C1_CLK_DISABLE();
  
	    /**I2C1 GPIO Configuration    
	    PB6     ------> I2C1_SCL
	    PB9     ------> I2C1_SDA 
	    */
		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_6 | GPIO_PIN_9);

		/* Peripheral interrupt DeInit*/
		HAL_NVIC_DisableIRQ(I2C1_EV_IRQn);
		HAL_NVIC_DisableIRQ(I2C1_ER_IRQn);
	}
	else if (hi2c->Instance == I2C3)
	{
	    /* Peripheral clock disable */
		__I2C3_CLK_DISABLE();
  
	    /**I2C3 GPIO Configuration    
	    PC9     ------> I2C3_SDA
	    PA8     ------> I2C3_SCL 
	    */
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_9);
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8);
		
		HAL_NVIC_DisableIRQ(I2C3_EV_IRQn);
		HAL_NVIC_DisableIRQ(I2C3_ER_IRQn);
	}
}