#pragma once

#include <stm32f4xx_hal.h>

#include <usbd_cdc_if.h>

#include <config_system.h>
#include <config_usb.h>
#include <config_timer.h>
#include <config_gpio.h>
#include <config_i2s.h> 
#include <config_i2c.h> 
#include <config_usart.h>

#include <util_buffer.h>
#include <util_string.h>

#include <movement.h>

#include <string.h>
#include <math.h>

#include <stm32f4_discovery_audio.h>

static void _strdelchr(char*, size_t, size_t*, size_t*);
size_t strdelstr(char*, const char*);
void VCP_Send(const void *pBuffer);
void VCP_Collect(const void *pBuffer);

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern TIM_HandleTypeDef htim5;
extern TIM_HandleTypeDef htim9;