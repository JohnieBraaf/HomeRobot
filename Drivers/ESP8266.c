#include <ESP8266.h>

char *ipAddress;

extern UART_HandleTypeDef huart2;
extern __IO FIFO_RXResponseTypeDef Response2Rx;
extern __IO FIFO_TXCommandTypeDef Command2Tx;

int rightTrackPower;
int leftTrackPower;
int torsoPower;
int armsPower;
int chestPower;

extern __IO uint32_t LeftTrack_RemainActive;
extern __IO uint32_t RightTrack_RemainActive;
extern __IO uint32_t Torso_RemainActive;
extern __IO uint32_t Arms_RemainActive;
extern __IO uint32_t Chest_RemainActive;

void UpdateESP(void)
{
	while (Response2Rx.count > 0)
	{
		char *response = RXResponseBufferGet(&Response2Rx, response);
		VCP_Send(response);
		
		if (strncmp(response, ">", 1) == 0)
		{
			char** tokens = str_split(response, ';');
			if (tokens)
			{
				int i;
				for (i = 0; *(tokens + i); i++)
				{
					char* val = *(tokens + i);
					switch (i) 
					{
					case 0:
						val = *(tokens + i) + 1; // strip first char
						leftTrackPower = atoi(val);
						LeftTrack_RemainActive = 500;
						break;
					case 1 :
						rightTrackPower = atoi(val);
						RightTrack_RemainActive = 500;
						break;
					case 2 :
						torsoPower = atoi(val);
						Torso_RemainActive = 500;
						break;
					case 3 :
						torsoPower = atoi(val);
						Torso_RemainActive = 500;
						break;
					case 4 :
						armsPower = atoi(val);
						Arms_RemainActive = 500;
						break;
					case 5 :
						chestPower = atoi(val);
						Chest_RemainActive = 500;
						break;
					}
					
					VCP_Send(val);
					VCP_Send("\n");
					//free(val);
					free(*(tokens + i));
				}			
				free(tokens);
			}
		}
	}
	
	if (Command2Tx.count > 0)
	{			
		char *command = TXCommandBufferGet(&Command2Tx, command);
		HAL_UART_Transmit(&huart2, (char *)command, strlen(command), 5);
		
		
	} 
}