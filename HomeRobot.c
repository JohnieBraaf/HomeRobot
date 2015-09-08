#include <stm32f4xx_hal.h>
#include <usbd_cdc_if.h>
#include <config_system.h>
#include <config_usb.h>
#include <config_timer.h>
#include <config_gpio.h>
#include <movement.h>
#include <string.h>

int printLen(int);

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;

extern __IO uint32_t RightTrack_RemainActive;
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

UART_HandleTypeDef huart6;

int main(void)
{
	HAL_Init();
	SystemClock_Config();
	USB_CDC_Confg();
/*
	PA2:	Torso FW		TIM5_CH3, TIM2_CH3, TIM9_CH1
	PA3:	Torso BW		TIM5_CH4, TIM2_CH4, TIM9_CH2
	PB4:	Arms Open		TIM3_CH1
	PB5:	Arms Close		TIM3_CH2
	PE5:	Chest CW		TIM9_CH1
	PE6:	Chest CCW		TIM9_CH2
	PE9:	Right Track FW	TIM1_CH1
	PE11:	Right Track BW	TIM1_CH2
	PE13:	Left Track FW	TIM1_CH3
	PE14:	Left Track BW	TIM1_CH4
*/
	
	MX_GPIO_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM4_Init();
	MX_TIM5_Init();
	MX_TIM9_Init();
	MX_USART6_UART_Init();

	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
	
	char byte;
	int rightTrack = 2;
	for (;;)
	{
		RightTrack();
		LeftTrack();
		
		int size = VCPRxBuffer.Size - VCPRxBuffer.Position;
		VCP_read(&byte, size); 
		if (size == 8)
		{
			VCP_write("\r\nYou typed: \t", 14);
			VCP_write(&byte, size);
			VCP_write("\r\n", 2);
			
			// create array of bytes received
			size_t len = strlen(&byte); 
			char byteArray[len];
			strncpy(byteArray, &byte, sizeof byteArray - 1);
			byteArray[len] = '\0';
					
			rightTrackForward	= (byteArray[0] >> 0) & 0x01;
			leftTrackForward	= (byteArray[0] >> 1) & 0x01;
			torsoForward		= (byteArray[0] >> 2) & 0x01;
			armsOpen			= (byteArray[0] >> 3) & 0x01;
			chestCW				= (byteArray[0] >> 4) & 0x01;
			int f				= (byteArray[0] >> 5) & 0x01;
			int g				= (byteArray[0] >> 6) & 0x01;
			int h				= (byteArray[0] >> 7) & 0x01;
			
			rightTrackPower = byteArray[1];
			leftTrackPower  = byteArray[2];
			torsoPower		= byteArray[3];
			armsPower		= byteArray[4];
			chestPower		= byteArray[5];
			
			// return debug messages
			char *rightTrackText[30];
			char *leftTrackText[30];
			char *torsText[30];
			char *armsText[30];
			char *chestText[30];
			sprintf(&rightTrackText, "Right Track Forward:\t%d, %d\r\n", rightTrackForward, rightTrackPower); 
			VCP_write(&rightTrackText, strlen(&rightTrackText));
			sprintf(&leftTrackText, "Left Track Forward:\t\t%d, %d\r\n", leftTrackForward, leftTrackPower); 
			VCP_write(&leftTrackText, strlen(&leftTrackText));
			sprintf(&torsText, "Torso Forward:\t\t%d, %d\r\n", torsoForward, torsoPower); 
			VCP_write(&torsText, strlen(&torsText));
			sprintf(&armsText, "ArmsOpen:\t\t%d, %d\r\n", armsOpen, armsPower); 
			VCP_write(&armsText, strlen(&armsText));
			sprintf(&chestText, "ChestCW:\t\t%d, %d\r\n", chestCW, chestPower); 
			VCP_write(&chestText, strlen(&chestText));
		}		
		else if ((size > 0 && size < 6) || size > 6)
		{
			VCP_write("Unrecongnized command, expecting 6 bytes.\r\n", 43);
			VCP_write("\r\nYou typed: \t", 14);
			VCP_write(&byte, size);
			VCP_write("\r\n", 2);
		}
	}
}