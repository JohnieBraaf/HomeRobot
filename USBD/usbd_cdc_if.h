#pragma once
#include "usbd_cdc.h"

struct s_RxBuffer
{
	uint8_t Buffer[CDC_DATA_HS_OUT_PACKET_SIZE];
	int Position, Size;
	char ReadDone;
};

extern USBD_CDC_ItfTypeDef  USBD_CDC_HomeRobot_fops;