#include "BSP_CAN.h"
#include "can.h"

CAN_FilterTypeDef canfilter_cfg;
CAN_TxHeaderTypeDef header_tx;
CAN_RxHeaderTypeDef header_rx;
uint32_t mailbox = 0;

int BSP_CAN_init(){
    int OK = 1;

    canfilter_cfg.FilterActivation     = CAN_FILTER_ENABLE;
    canfilter_cfg.FilterFIFOAssignment = CAN_FILTER_FIFO0;
    canfilter_cfg.FilterMode           = CAN_FILTERMODE_IDMASK;
    canfilter_cfg.FilterScale          = CAN_FILTERSCALE_32BIT;
    canfilter_cfg.FilterIdHigh         = 0x00;
    canfilter_cfg.FilterIdLow          = 0x00;
    canfilter_cfg.FilterMaskIdHigh     = 0x00;
    canfilter_cfg.FilterMaskIdLow      = 0x00;
    canfilter_cfg.FilterBank           = 0;
    canfilter_cfg.SlaveStartFilterBank = 0;
    OK*=(HAL_CAN_ConfigFilter(&hcan,&canfilter_cfg)==0);


    header_tx.StdId              = 0x200;
    header_tx.IDE                = CAN_ID_STD;
    header_tx.RTR                = CAN_RTR_DATA;
    header_tx.DLC                = 8;
    header_tx.TransmitGlobalTime = DISABLE;

    header_rx.StdId              = 0x201;
    header_rx.ExtId              = 0;
    header_rx.IDE                = CAN_ID_STD;
    header_rx.RTR                = CAN_RTR_DATA;
    header_rx.DLC                = 8;

    
    OK*=(HAL_CAN_Start(&hcan)==0);
    OK*=(HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING)==0);

    return !OK;
}

void BSP_CAN_Send_Data(uint8_t* pData, uint32_t StdID){
    header_tx.StdId = StdID;
    HAL_CAN_AddTxMessage(&hcan, &header_tx, pData, &mailbox);
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan){
    //return;
    uint8_t data[8];
    HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &header_rx, data);

    extern void BSP_CAN_Callback(uint8_t* pdata, CAN_RxHeaderTypeDef* header_rx);
    BSP_CAN_Callback(data, &header_rx);

    __HAL_CAN_ENABLE_IT(hcan, CAN_IT_RX_FIFO0_MSG_PENDING);
}
