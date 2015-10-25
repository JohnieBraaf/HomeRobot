#include <util_buffer.h>

void BufferInit(__IO FIFO_TypeDef *buffer)
{
	buffer->count = 0;//0 bytes in buffer
	buffer->in = 0;//index points to start
	buffer->out = 0;//index points to start
}

extern __IO FIFO_RXResponseTypeDef Response2Rx;
ErrorStatus BufferPut(__IO FIFO_TypeDef *buffer, uint8_t ch)
{
	if (buffer->count == USARTBUFFSIZE)
		return ERROR;//buffer full
	
	if (strncmp(&ch, "\n", 1) == 0)
	{
		int size = buffer->in - buffer->out;
		if (buffer->in < buffer->out) size = buffer->out - buffer->in;
		char comm[size + 2];
		for (int i = 0; i < size; i++)
		{
			comm[i] = BufferGet(buffer);
		}
		comm[size] = '\n';
		comm[size + 1] = '\0';
		RXResponseBufferPut(&Response2Rx, comm);
		
		buffer->buff[buffer->in++] = ch;
		if (buffer->in == USARTBUFFSIZE - 1)
			buffer->in = 0;//start from beginning	
		buffer->out = buffer->in;
	}
	else
	{
		buffer->buff[buffer->in++] = ch;
		buffer->count++;
		if (buffer->in == USARTBUFFSIZE - 1)
			buffer->in = 0;//start from beginning
	}	
	return SUCCESS;
}

uint8_t BufferGet(__IO FIFO_TypeDef *buffer)
{
	if (buffer->count == 0)
		return (char *)ERROR;//buffer empty
	
	buffer->count--;
	
	if (buffer->out++ == USARTBUFFSIZE -2)
		buffer->out = 0;//start from beginning
	
	if (buffer->out == 0) return buffer->buff[USARTBUFFSIZE-2];
	else return buffer->buff[buffer->out-1];
	
}

ErrorStatus BufferIsEmpty(__IO FIFO_TypeDef buffer)
{
	if (buffer.count == 0)
		return SUCCESS;//buffer full
	return ERROR;
}

void RXResponseBufferInit(__IO FIFO_RXResponseTypeDef *buffer)
{
	buffer->count = 0;//0 bytes in buffer
	buffer->in = 0;//index points to start
	buffer->out = 0;//index points to start
}

ErrorStatus RXResponseBufferPut(__IO FIFO_RXResponseTypeDef *buffer, char *comm)
{
	if (buffer->count == RXRESPONSEBUFFSIZE)
		return ERROR;//buffer full
	buffer->RXResponse[buffer->in++] = strdup(comm);
	buffer->count++;
	if (buffer->in == RXRESPONSEBUFFSIZE)
		buffer->in = 0;//start from beginning
	return SUCCESS;
}

char *RXResponseBufferGet(__IO FIFO_RXResponseTypeDef *buffer, char *comm)
{
	if (buffer->count == 0)
		return "";
	comm = strdup(buffer->RXResponse[buffer->out++]);
	buffer->count--;
	if (buffer->out == RXRESPONSEBUFFSIZE)
		buffer->out = 0;//start from beginning
	return comm;
}

ErrorStatus RXResponseBufferIsEmpty(__IO FIFO_RXResponseTypeDef buffer)
{
	if (buffer.count == 0)
		return SUCCESS;//buffer full
	return ERROR;
}

void TXCommandBufferInit(__IO FIFO_TXCommandTypeDef *buffer)
{
	buffer->count = 0;//0 bytes in buffer
	buffer->in = 0;//index points to start
	buffer->out = 0;//index points to start
}

ErrorStatus TXCommandBufferPut(__IO FIFO_TXCommandTypeDef *buffer, char *comm, char bits)
{
	if (buffer->count == TXCOMMANDBUFFSIZE)
		return ERROR;//buffer full
	buffer->bits[buffer->in] = bits;
	buffer->TXCommand[buffer->in++] = strdup(comm);
	buffer->count++;
	if (buffer->in == TXCOMMANDBUFFSIZE)
		buffer->in = 0;//start from beginning
	return SUCCESS;
}

char *TXCommandBufferGet(__IO FIFO_TXCommandTypeDef *buffer, char *comm)
{
	if (buffer->count == 0)
		return "";
	comm = strdup(buffer->TXCommand[buffer->out++]);
	buffer->count--;
	if (buffer->out == TXCOMMANDBUFFSIZE)
		buffer->out = 0;//start from beginning
	return comm;
}

ErrorStatus TXCommandBufferIsEmpty(__IO FIFO_TXCommandTypeDef buffer)
{
	if (buffer.count == 0)
		return SUCCESS;//buffer full
	return ERROR;
}