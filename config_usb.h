#pragma once

void USB_CDC_Confg(void);
void OTG_FS_IRQHandler(void);
void OTG_HS_IRQHandler(void);

int VCP_read(void *pBuffer, int size);
int VCP_write(const void *pBuffer, int size);