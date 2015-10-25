#include <ESP8266.h>

struct ESPNetwork network;
uint8_t status = 0;
uint8_t waitingForEvent = 0;
uint16_t byteCounter = 0;

extern UART_HandleTypeDef huart2;
extern __IO FIFO_RXResponseTypeDef Response2Rx;
extern __IO FIFO_TXCommandTypeDef Command2Tx;

void InitESP(void)
{
	status = 1;
	
	
}

void UpdateESP(void)
{
	//if (status == 0) InitESP();
	
	while (Response2Rx.count > 0)
	{
		char *response = RXResponseBufferGet(&Response2Rx, response);
		VCP_Send(response);
		
		char *val = substrdelim(response, "\"");
			
		if (waitingForEvent == 2 && byteCounter > 0)
		{
			// decrement number of bytes
			byteCounter = byteCounter - (strlen(response) > byteCounter ? 0 : strlen(response));	
			if (byteCounter == 0)
				waitingForEvent = 0;
		}
		
		char *rightTrackText[30];
		sprintf((char*)rightTrackText, "\r\nvalue:\t\t%d\r\n", byteCounter); 
		VCP_Send(&rightTrackText);

		if (strncmp(response, "OK", 2) == 0 && waitingForEvent == 1) 
			waitingForEvent = 0; 
		else if (strncmp(response, "ERROR\r\n", 7) == 0 && waitingForEvent == 1)
			waitingForEvent = 0;		
		else if (strncmp(response, "+", 1) == 0) 
		{
			if (strncmp(response, "+CIFSR:APIP", 11) == 0)
			{
				network.apIP = substrdelim(response, "\"");
			}
			else if (strncmp(response, "+CIFSR:APMAC", 12) == 0)
			{
				network.apMAC = substrdelim(response, "\"");
			}
			else if (strncmp(response, "+CIFSR:STAIP", 12) == 0)
			{
				network.stationIP = substrdelim(response, "\"");
			}
			else if (strncmp(response, "+CIFSR:STAMAC", 13) == 0)
			{
				network.stationMAC = substrdelim(response, "\"");
			}
			else if (strncmp(response, "+IPD", 4) == 0)
			{
				char *value = substrdelim2(response, ",", ":");
				byteCounter = atoi(value) - strlen(response) + strlen(value) + 6; // 6: "+IPD,:"	
				waitingForEvent = 2;
			}
		}	
	}
	
	if (Command2Tx.count > 0 && waitingForEvent == 0)
	{			
		char *command = TXCommandBufferGet(&Command2Tx, command);
		HAL_UART_Transmit(&huart2, (char *)command, strlen(command), 5);
			
		if (Command2Tx.bits[Command2Tx.out - 1] == 1)
			waitingForEvent = 1;
		if (Command2Tx.bits[Command2Tx.out - 1] == 2)
			waitingForEvent = 2;
	} 
}