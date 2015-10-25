#pragma once
#include <HomeRobot.h>

struct ESPNetwork
{
	char *apIP;
	char *apMAC;
	char *stationIP;
	char *stationMAC;
};

void UpdateESP(void);
void InitESP(void);