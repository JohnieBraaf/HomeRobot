#include <HomeRobot.h>

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

extern UART_HandleTypeDef huart2;
extern DMA_HandleTypeDef hdma_usart2_rx;
extern __IO FIFO_TypeDef U2Rx, U2Tx;
extern __IO FIFO_CommandTypeDef Command2Rx;

extern uint8_t rx2Buffer;

__IO int16_t sample = 0;
__IO int freq, fs, amplitude, cycle;
__IO double angle, increment;

#define MAXVCPSTRING          1024 // Biggest string the user will type
__IO char vcpString[MAXVCPSTRING]; // where we build our string from characters coming in
__IO int vcpIndex = 0;

volatile char byte;
extern int waitingForAck;
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

		while (Command2Rx.count > 0)
		{
			char *c;
			c = CommandBufferGet(&Command2Rx, c);
			VCP_write(c, strlen(c));
		
			char *val = substrdelim(c, "\"");
			VCP_write(val, strlen(val));	
			
			if (strncmp(c, "OK\r\n", 4) == 0) 
			{
				waitingForAck = 0;
			}
			else if (strncmp(c, "ERROR\r\n", 7) == 0)
			{
				waitingForAck = 2;
			}
			
			if (strncmp(c, "+", 1) == 0) 
			{
					
			}
		}
		
		if (waitingForAck != 1)
		{
			
		}
		
		if (VCP_read(&byte, 1) == 1 )
		{
			vcpString[vcpIndex] = byte;
			vcpIndex++;

			int ret = strcmp(&byte, "\n");
			if (ret == 192 || vcpIndex == MAXVCPSTRING)
			{				
				ret = strncmp(&vcpString, "123\r\n", 5);
				if (ret == 0)
				{
					waitingForAck = 1;					
					HAL_UART_Transmit(&huart2, (char *)"AT\r\n", 4, 5);					
					//while (waitingForAck == 1) {}

					waitingForAck = 1;					
					HAL_UART_Transmit(&huart2, (char *)"AT+CIFSR\r\n", 10, 5);					
					//while (waitingForAck == 1) {}

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

void DMA1_Stream5_IRQHandler(void)
{
	HAL_NVIC_ClearPendingIRQ(DMA1_Stream5_IRQn);
	HAL_DMA_IRQHandler(&hdma_usart2_rx);
}