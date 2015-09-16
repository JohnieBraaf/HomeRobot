#include <config_i2s.h> 

I2S_HandleTypeDef hi2s3;

void MX_I2S3_Init(void)
{
	hi2s3.Instance = SPI3;
	hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
	hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
	hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
	hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
	hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
	hi2s3.Init.CPOL = I2S_CPOL_LOW;
	hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
	//hi2s2.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
	HAL_I2S_Init(&hi2s3);
}


void HAL_I2S_MspInit(I2S_HandleTypeDef* hi2s)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	if (hi2s->Instance == SPI3)
	{
	  /* Peripheral clock enable */
		__SPI3_CLK_ENABLE();
		//__DMA1_CLK_ENABLE();

		/**I2S3 GPIO Configuration    		    
		PA4      ------> I2S3_WS
		PC7      ------> I2S2_MCK
		PC10     ------> I2S3_CK
		PC12     ------> I2S3_SD 
		*/
		GPIO_InitStruct.Pin = GPIO_PIN_4;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
		HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_12;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FAST;
		GPIO_InitStruct.Alternate = GPIO_AF6_SPI3;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		/* Peripheral DMA init*/
/*
		hdma_i2s2_ext_tx.Instance = DMA1_Stream4;
		hdma_i2s2_ext_tx.Init.Channel = DMA_CHANNEL_2;
		hdma_i2s2_ext_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
		hdma_i2s2_ext_tx.Init.PeriphInc = DMA_PINC_DISABLE;
		hdma_i2s2_ext_tx.Init.MemInc = DMA_MINC_ENABLE;
		hdma_i2s2_ext_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
		hdma_i2s2_ext_tx.Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
		hdma_i2s2_ext_tx.Init.Mode = DMA_CIRCULAR;
		hdma_i2s2_ext_tx.Init.Priority = DMA_PRIORITY_HIGH;
		hdma_i2s2_ext_tx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
		hdma_i2s2_ext_tx.Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
		hdma_i2s2_ext_tx.Init.MemBurst = DMA_MBURST_SINGLE;
		hdma_i2s2_ext_tx.Init.PeriphBurst = DMA_PBURST_SINGLE;
		HAL_DMA_Init(&hdma_i2s2_ext_tx);

		__HAL_LINKDMA(hi2s, hdmatx, hdma_i2s2_ext_tx);

		HAL_NVIC_SetPriority(DMA1_Stream4_IRQn, 0, 1);
		HAL_NVIC_EnableIRQ(DMA1_Stream4_IRQn);
*/

	}
}

void HAL_I2S_MspDeInit(I2S_HandleTypeDef* hi2s)
{

	if (hi2s->Instance == SPI2)
	{
		__SPI3_CLK_DISABLE();


		/**I2S3 GPIO Configuration    		    
		PA4      ------> I2S3_WS
		PC7      ------> I2S2_MCK
		PC10     ------> I2S3_CK
		PC12     ------> I2S3_SD 
		*/
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4);
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_7 | GPIO_PIN_10 | GPIO_PIN_12);

		/* Peripheral DMA DeInit*/
		//HAL_DMA_DeInit(hi2s->hdmatx);
		//HAL_DMA_DeInit(hi2s->hdmarx);
	}
} 
/*
void
HAL_I2S_TxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	uint32_t newTx = 0;
}

void
HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
	uint32_t newTx = 0;
}

void DMA1_Stream3_IRQHandler(void) {
	HAL_DMA_IRQHandler(hi2s2.hdmarx);
}

void DMA1_Stream4_IRQHandler(void) {
	HAL_DMA_IRQHandler(hi2s2.hdmatx);
}*/