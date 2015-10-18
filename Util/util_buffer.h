#pragma once
#include <stm32f4xx_hal.h>
#include <string.h>

#define USARTBUFFSIZE 100
#define COMMANDBUFFSIZE 50

typedef struct {
	uint8_t in;
	uint8_t out;
	uint8_t count;
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
	char *command[COMMANDBUFFSIZE];
}FIFO_CommandTypeDef;

void CommandBufferInit(__IO FIFO_CommandTypeDef*);
ErrorStatus CommandBufferPut(__IO FIFO_CommandTypeDef*, char*, uint8_t);
char *CommandBufferGet(__IO FIFO_CommandTypeDef*, char*);
ErrorStatus CommandBufferIsEmpty(__IO FIFO_CommandTypeDef buffer);