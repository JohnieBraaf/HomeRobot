#include <HomeRobot.h>


extern __IO uint32_t Audio_tone;

extern struct s_RxBuffer VCPRxBuffer;



extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern __IO FIFO_TypeDef U2Rx, U2Tx;
extern __IO FIFO_RXResponseTypeDef Response2Rx;
extern __IO FIFO_TXCommandTypeDef Command2Tx;

extern uint8_t rx2Buffer;

struct ESPNetwork network;

__IO int16_t sample = 0;
__IO int freq, fs, amplitude, cycle;
__IO double angle, increment;

#define MAXVCPSTRING 1024 
__IO char vcpString[MAXVCPSTRING];
__IO int vcpIndex = 0;

volatile char byte;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	USB_CDC_Confg();
	
	MX_GPIO_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM5_Init();
	MX_TIM9_Init();
	MX_USART2_UART_Init();
	//MX_I2S3_Init();
	//MX_I2C1_Init();
	
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
	
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_4);
	
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim9, TIM_CHANNEL_2);
	
	BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, 100, I2S_AUDIOFREQ_8K);
	freq = 1000;
	fs = 10;	
	char byte;
	int byteCounter = -1;
	for (;;)
	{
		if (Audio_tone == 0) 
		{
			freq = freq - 5;
			Audio_tone = 500;		
		}	
			
		amplitude = 100;
		angle = -45;
		increment = ((2 * 3.41) / (fs / freq)) + 1000;
		for (cycle = 1;cycle <= freq;cycle++)
		{
			while (angle <= (2 * 3.41))
			{
				sample = amplitude * sin(angle);
				angle = angle + increment;
				BSP_AUDIO_OUT_Play_Direct((uint16_t*)&sample, sizeof(&sample)); 					
			}				
		}
		// make some noise 
		/*int x = 0;
		int y = 400;
		for (x = 0; x < y; x++)
			BSP_AUDIO_OUT_Play_Direct((uint16_t*)x, 2); 
		for (x = y; x < 0; x--)
			BSP_AUDIO_OUT_Play_Direct((uint16_t*)x, 2); 
		*/
		
		UpdateESP();
		
		RightTrack();
		LeftTrack();		
					
		if (VCP_read(&byte, 1) == 1 )
		{
			vcpString[vcpIndex] = byte;
			vcpIndex++;
	
			if (strncmp(&byte, "\n", 1) == 0 || vcpIndex == MAXVCPSTRING)
			{				
				char command[vcpIndex + 1];
				for (int i = 0; i < vcpIndex; i++)
				{
					command[i] = vcpString[i];
				}
				command[vcpIndex] = '\0';
				HAL_UART_Transmit(&huart2, &command, vcpIndex, 5);
				vcpIndex = 0;
			}			
		}
	}
}

void VCP_Send(const void *pBuffer)
{
	VCP_write((uint8_t *)pBuffer, strlen((uint8_t *)pBuffer));
}

void DMA1_Stream5_IRQHandler(void)
{
	HAL_NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn);
	HAL_DMA_IRQHandler(&hdma_usart2_rx);
}