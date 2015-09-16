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
	MX_I2S3_Init();
	MX_I2C1_Init();

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
		
		//while (I2C_GetFlagStatus(I2C1, I2C_ISR_BUSY) != RESET)
			byte = 0;
		
		int size = VCPRxBuffer.Size - VCPRxBuffer.Position;
		if (size == 8)
		{
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
			VCP_write(&leftTrackText, strlen((char*)rightTrackText));
			sprintf((char*)rightTrackText, "Left Track Forward:\t\t%d, %d\r\n", rightTrackForward, rightTrackPower); 
			VCP_write(&rightTrackText, strlen((char*)rightTrackText));
			sprintf((char*)torsText, "Torso Forward:\t\t%d, %d\r\n", torsoForward, torsoPower); 
			VCP_write(&torsText, strlen((char*)torsText));
			sprintf((char*)armsText, "ArmsOpen:\t\t%d, %d\r\n", armsOpen, armsPower); 
			VCP_write(&armsText, strlen((char*)armsText));
			sprintf((char*)chestText, "ChestCW:\t\t%d, %d\r\n", chestCW, chestPower); 
			VCP_write(&chestText, strlen((char*)chestText));
			sprintf((char*)durationText, "Duration:\t\t\t\%dms\r\n", activeDuration * 50); 
			VCP_write(&durationText, strlen((char*)durationText));
			VCP_write("**********", 10);
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