#include <crypto.h>

UART_HandleTypeDef     UartHandle;

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


int loop(void)
{ 
	/* Configure the COM port */
	UartHandle.Instance        = USART3;
	UartHandle.Init.BaudRate 	 = 9600;
	UartHandle.Init.WordLength = UART_WORDLENGTH_8B;
	UartHandle.Init.StopBits 	 = UART_STOPBITS_1;
	UartHandle.Init.Parity 		 = UART_PARITY_NONE;
	UartHandle.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
	UartHandle.Init.Mode 			 = UART_MODE_TX_RX;
	HAL_UART_Init(&UartHandle);

	while (HAL_UART_GetState(&UartHandle) != HAL_UART_STATE_READY)
	{
			
	} 	 
	//HAL_UART_Transmit_ITHAL_UART_Transmit // compare to HAL_UART_Transmit_IT just add a timeout of 5 as last argument
	if (HAL_UART_Transmit(&UartHandle, (uint8_t*)128, 1, 5) != HAL_OK)
	{

	}		
}




/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
	HAL_UART_Transmit(&UartHandle, (uint8_t *)&ch, 1, 5); 

	return ch;
}