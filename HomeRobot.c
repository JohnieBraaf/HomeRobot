#include <stm32f4xx_hal.h>
#include <usbd_cdc_if.h>
#include <config_system.h>
#include <config_usb.h>
#include <config_timer.h>
#include <config_gpio.h>
#include <config_i2s.h> 
#include <config_i2c.h> 
#include <movement.h>
#include <string.h>
#include <stm32f4_discovery_audio.h>
#include <math.h>

void VCP_Send(const void *pBuffer);

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

extern __IO uint32_t LeftTrack_RemainActive;
extern __IO uint32_t RightTrack_RemainActive;
extern __IO uint32_t Torso_RemainActive;
extern __IO uint32_t Arms_RemainActive;
extern __IO uint32_t Chest_RemainActive;
extern __IO uint32_t Audio_tone;

extern struct s_RxBuffer VCPRxBuffer;

extern int rightTrackForward;
extern int leftTrackForward;
extern int torsoForward;
extern int armsOpen;
extern int chestCW;

int rightTrackPower;
int leftTrackPower;
int torsoPower;
int armsPower;
int chestPower;
int activeDuration;

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
uint8_t rx2Buffer = '\000';

__IO int16_t sample = 0;
__IO int freq, fs, amplitude, cycle;
__IO double angle, increment;

#define MAXVCPSTRING          1024 // Biggest string the user will type
__IO char vcpString[MAXVCPSTRING]; // where we build our string from characters coming in
__IO int vcpIndex = 0;

volatile char byte;
int waitingForAck = 0;
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
	MX_USART6_UART_Init();
	MX_USART2_UART_Init();
	//MX_I2S3_Init();
	//MX_I2C1_Init();
			
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);

	BSP_AUDIO_OUT_Init(OUTPUT_DEVICE_BOTH, 100, I2S_AUDIOFREQ_8K);
	freq = 1000;
	fs = 10;	
	char byte;
	
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
		RightTrack();
		LeftTrack();			

		if (VCP_read(&byte, 1) == 1)
		{
			vcpString[vcpIndex] = byte;
			vcpIndex++;
			
			//int ret = strcmp(&vcpString, "OK\r\n");

			
			int ret = strcmp(&byte, "\n");
			if (ret == 192 || vcpIndex == MAXVCPSTRING)
			{				
				ret = strcmp(&vcpString, "123\r\n");
				if (ret == 0)
				{
					char send[5] = "AT\r\n";					
					HAL_UART_Transmit(&huart2, &send, 4, 5);
					waitingForAck = 1;
				} 
				else
				{
					HAL_UART_Transmit(&huart2, &vcpString, vcpIndex, 5);
				}
				
				vcpIndex = 0;
			}			
		}
					
		/*
		if (size == 8)
		{
			connected = 1;
			// header byte
			VCP_read(&byte, 1); 						
			leftTrackForward	= (byte >> 0) & 0x01;
			rightTrackForward	= (byte >> 1) & 0x01;
			torsoForward		= (byte >> 2) & 0x01;
			armsOpen			= (byte >> 3) & 0x01;
			chestCW				= (byte >> 4) & 0x01;
			int f				= (byte >> 5) & 0x01;
			int g				= (byte >> 6) & 0x01;
			int h				= (byte >> 7) & 0x01;
			
			// payload
			VCP_read(&byte, 1);
			leftTrackPower = byte;
			VCP_read(&byte, 1);
			rightTrackPower  = byte;
			VCP_read(&byte, 1);
			torsoPower		= byte;
			VCP_read(&byte, 1);
			armsPower		= byte;
			VCP_read(&byte, 1);
			chestPower		= byte;
			VCP_read(&byte, 1);
			activeDuration	= byte;
			LeftTrack_RemainActive = (int)activeDuration * 50;
			RightTrack_RemainActive = activeDuration * 50;
			Torso_RemainActive = activeDuration * 50;
			Arms_RemainActive = activeDuration * 50;
			Chest_RemainActive = activeDuration * 50;
			
			// tail
			VCP_read(&byte, 1);
			
			// return debug messages
			char *leftTrackText[30];
			char *rightTrackText[30];
			char *torsText[30];
			char *armsText[30];
			char *chestText[30];
			char *durationText[30];
			sprintf((char*)leftTrackText, "Right Track Forward:\t%d, %d\r\n", leftTrackForward, leftTrackPower); 
			VCP_Send(&leftTrackText);
			sprintf((char*)rightTrackText, "Left Track Forward:\t\t%d, %d\r\n", rightTrackForward, rightTrackPower); 
			VCP_Send(&rightTrackText);
			sprintf((char*)torsText, "Torso Forward:\t\t%d, %d\r\n", torsoForward, torsoPower); 
			VCP_Send(&torsText);
			sprintf((char*)armsText, "ArmsOpen:\t\t%d, %d\r\n", armsOpen, armsPower); 
			VCP_Send(&armsText);
			sprintf((char*)chestText, "ChestCW:\t\t%d, %d\r\n", chestCW, chestPower); 
			VCP_Send(&chestText);
			sprintf((char*)durationText, "Duration:\t\t\t\%dms\r\n", activeDuration * 50); 
			VCP_Send(&durationText);
			VCP_Send("**********");
		}		
		else if ((size > 0 && size < 6) || size > 6)
		{
			VCP_write("Unrecongnized command, expecting 6 bytes.\r\n", 43);
			VCP_write("\r\nYou typed: \t", 14);
			VCP_write(&byte, size);
			VCP_write("\r\n", 2);
			VCP_read(&byte, 1);
		}
		*/
	}
}

void ProcessCommand(const void *pBuffer)
{
	
}

void VCP_Send(const void *pBuffer)
{
	VCP_write((uint8_t *)pBuffer, strlen((uint8_t *)pBuffer));
}

void VCP_Collect(const void *pBuffer)
{
	byte = pBuffer;
}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{
	__GPIOD_CLK_ENABLE();
	__USART2_CLK_ENABLE();
	__DMA1_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_5 | GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.OverSampling = UART_OVERSAMPLING_16;
	HAL_UART_Init(&huart2);

	hdma_usart2_rx.Instance = DMA1_Stream5;
	hdma_usart2_rx.Init.Channel = DMA_CHANNEL_4;
	hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_usart2_rx.Init.MemInc = DMA_MINC_DISABLE;
	hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
	hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
	hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
	hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
	hdma_usart2_rx.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	HAL_DMA_Init(&hdma_usart2_rx);

	__HAL_LINKDMA(&huart2, hdmarx, hdma_usart2_rx);

	HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
	
	__HAL_UART_FLUSH_DRREGISTER(&huart2);
	HAL_UART_Receive_DMA(&huart2, &rx2Buffer, 1);
}

#define MAXCLISTRING          1024 // Biggest string the user will type

__IO char rx2String[MAXCLISTRING]; // where we build our string from characters coming in
__IO char uart_passthrough[MAXCLISTRING];
__IO int rx2index = 0;

void DMA1_Stream5_IRQHandler(void)
{
	HAL_NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn);
	HAL_DMA_IRQHandler(&hdma_usart2_rx);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart2); 
	rx2String[rx2index] = rx2Buffer;
	
	if (rx2index < MAXCLISTRING-1)
		rx2index++;		
	
	if (rx2index == MAXCLISTRING - 1) 
		rx2String[rx2index] = '\n';

	if ((rx2Buffer == '\n' &&  rx2index != 1) || rx2index == MAXCLISTRING-1)
	{		
		for (int i=0; i<= rx2index; i++) uart_passthrough[i] = rx2String[i];
		VCP_write(&uart_passthrough, rx2index); 
		//HAL_UART_Transmit(&huart2, &uart_passthrough, rx2index, rx2index);	
		char temp[rx2index];
		for (int i2 = 0; i2 < rx2index; i2++) temp[i2] = rx2String[i2];
		int ret = strcmp(&temp, "OK\r\nwl");
		if (ret != -14 && ret != -66)
			waitingForAck = 0;
		
		if (rx2index == MAXCLISTRING - 1 && rx2Buffer != '\n')
		{
			rx2String[0] = rx2Buffer;
			rx2index = 1;
		}
		else rx2index = 0;
	}	
}