#pragma once
#include <stm32f4xx_hal.h>
#include <string.h>

#define USARTBUFFSIZE 1024
#define RXRESPONSEBUFFSIZE 50

typedef struct {
	uint16_t in;
	uint16_t out;
	uint16_t count;
	uint8_t buff[USARTBUFFSIZE];
}FIFO_TypeDef;

void BufferInit(__IO FIFO_TypeDef *buffer);
ErrorStatus BufferPut(__IO FIFO_TypeDef*, uint8_t);
uint8_t BufferGet(__IO FIFO_TypeDef*);
ErrorStatus BufferIsEmpty(__IO FIFO_TypeDef);

typedef struct {
	uint8_t in;
	uint8_t out;
	uint8_t count;
	char *RXResponse[RXRESPONSEBUFFSIZE];
}FIFO_RXResponseTypeDef;

void RXResponseBufferInit(__IO FIFO_RXResponseTypeDef*);
ErrorStatus RXResponseBufferPut(__IO FIFO_RXResponseTypeDef*, char*, uint8_t);
char *RXResponseBufferGet(__IO FIFO_RXResponseTypeDef*, char*);
ErrorStatus RXResponseBufferIsEmpty(__IO FIFO_RXResponseTypeDef buffer);