#pragma once

#include "stm32f4xx_hal_uart.h"

void USART_puts(UART_HandleTypeDef *usartx, uint8_t *s);