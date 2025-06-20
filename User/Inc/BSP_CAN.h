#ifndef __BSP_CAN_H
#define __BSP_CAN_H

#include "main.h"

int BSP_CAN_init(void);

void BSP_CAN_Send_Data(uint8_t* pData, uint32_t StdID);
__weak void BSP_CAN_Callback(uint8_t* pdata, CAN_RxHeaderTypeDef* header_rx);

#endif