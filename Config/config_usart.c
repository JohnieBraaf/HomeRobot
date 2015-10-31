#include <config_usart.h>

UART_HandleTypeDef huart2;
DMA_HandleTypeDef hdma_usart2_rx;
__IO FIFO_TypeDef U2Rx, U2Tx;
__IO FIFO_RXResponseTypeDef Response2Rx;
__IO FIFO_TXCommandTypeDef Command2Tx;

uint8_t rx2Buffer = '\000';

#define MAXCLISTRING 1024
__IO char rx2String[MAXCLISTRING]; // where we build our string from characters coming in
__IO char uart_passthrough[MAXCLISTRING];
__IO int rx2index = 0;

extern struct ESPNetwork network;

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
	
	BufferInit(&U2Rx);
	BufferInit(&U2Tx);
	
	RXResponseBufferInit(&Response2Rx);
	TXCommandBufferInit(&Command2Tx);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	__HAL_UART_FLUSH_DRREGISTER(&huart2);
	BufferPut(&U2Rx, rx2Buffer);
}