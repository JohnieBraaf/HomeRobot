#include "serial.h"
#include "string.h"
#include "stm32f4xx_hal_uart.h"

extern UART_HandleTypeDef     UartHandle;
extern uint8_t aEncryptedtext[64];

void USART_puts(UART_HandleTypeDef *usartx, uint8_t *s)
{
	while (*s) {
		HAL_UART_Transmit(usartx, s, 1, 5); 
		s++;
	}
}

char buffer[1024] = { 0 };
uint16_t data_length = 0;

void USART3_IRQHandler(void)
{
	
	HAL_UART_IRQHandler(& UartHandle);
	
	// check if the USART2 receive interrupt flag was set
	if (UartHandle.ErrorCode == HAL_UART_ERROR_NONE) {
		char* aEncryptedtext;
		char t = *UartHandle.pRxBuffPtr;//USART1->DR; // the character from the USART2 data register is saved in t

		if (t == '\r') {
			// Just ignore
		}
		else if (data_length >= (sizeof(buffer) - 3)) {
			// Potential overflow...
			data_length = 0;
		}
		else if (t == '\n') {
			buffer[data_length++] = t;
			buffer[data_length++] = '\0';
			// Send the complete received message to ...
			strncpy((char *)aEncryptedtext, buffer, sizeof(aEncryptedtext));
			data_length = 0;
		}
		else {
			// Handle a normal character
			buffer[data_length++] = t;
		}
	}
}
