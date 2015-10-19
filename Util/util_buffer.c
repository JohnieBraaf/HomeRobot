#include <util_buffer.h>

void BufferInit(__IO FIFO_TypeDef *buffer)
{
	buffer->count = 0;//0 bytes in buffer
	buffer->in = 0;//index points to start
	buffer->out = 0;//index points to start
}

extern __IO FIFO_CommandTypeDef Command2Rx;
ErrorStatus BufferPut(__IO FIFO_TypeDef *buffer, uint8_t ch)
{
	if (buffer->count == USARTBUFFSIZE)
		return ERROR;//buffer full
	
	if (strncmp(&ch, "\n", 1) == 0)
	{
		int size = buffer->in - buffer->out;
		if (buffer->in < buffer->out) size = buffer->out - buffer->in + 1;
		char comm[size + 2];
		for (int i = 0; i < size; i++)
		{
			comm[i] = BufferGet(buffer);
		}
		comm[size] = '\n';
		comm[size + 1] = '\0';
		CommandBufferPut(&Command2Rx, &comm, strlen(&comm));
	}
	else
	{
		buffer->buff[buffer->in++] = ch;
		buffer->count++;
		if (buffer->in == USARTBUFFSIZE)
			buffer->in = 0;//start from beginning
	}	
	return SUCCESS;
}

uint8_t BufferGet(__IO FIFO_TypeDef *buffer)
{
	if (buffer->count == 0)
		return (char *)ERROR;//buffer empty
	
	buffer->count--;
	
	if (buffer->out++ == USARTBUFFSIZE -1)
		buffer->out = 0;//start from beginning
	
	if (buffer->out == 0) return buffer->buff[USARTBUFFSIZE-1];
	else return buffer->buff[buffer->out-1];
	
}

ErrorStatus BufferIsEmpty(__IO FIFO_TypeDef buffer)
{
	if (buffer.count == 0)
		return SUCCESS;//buffer full
	return ERROR;
}

void CommandBufferInit(__IO FIFO_CommandTypeDef *buffer)
{
	buffer->count = 0;//0 bytes in buffer
	buffer->in = 0;//index points to start
	buffer->out = 0;//index points to start
}

ErrorStatus CommandBufferPut(__IO FIFO_CommandTypeDef *buffer, char *comm, uint8_t size)
{
	if (buffer->count == COMMANDBUFFSIZE)
		return ERROR;//buffer full
	buffer->command[buffer->in++] = strdup(comm);
	buffer->count++;
	if (buffer->in == COMMANDBUFFSIZE)
		buffer->in = 0;//start from beginning
	return SUCCESS;
}

char *CommandBufferGet(__IO FIFO_CommandTypeDef *buffer, char *comm)
{
	if (buffer->count == 0)
		return "";
	comm = strdup(buffer->command[buffer->out++]);
	buffer->count--;
	if (buffer->out == COMMANDBUFFSIZE)
		buffer->out = 0;//start from beginning
	return comm;
}

ErrorStatus CommandBufferIsEmpty(__IO FIFO_CommandTypeDef buffer)
{
	if (buffer.count == 0)
		return SUCCESS;//buffer full
	return ERROR;
}