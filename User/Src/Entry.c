#include "Entry.h"
#include "OLED_DMA.h"
#include "BSP_CAN.h"

void set_motor_current(float current_f)
{

    int16_t current = 16384*current_f*.05f;

    uint8_t data[8];

    data[0] = current >> 8;
    data[1] = current | 0xff00;
    data[2] = current >> 8;
    data[3] = current | 0xff00;
    data[4] = current >> 8;
    data[5] = current | 0xff00;
    data[6] = current >> 8;
    data[7] = current | 0xff00;

    BSP_CAN_Send_Data(data, 0x200);

}


void BSP_CAN_Callback(uint8_t* pdata, CAN_RxHeaderTypeDef* header_rx){

    set_motor_current(0.5);
}


_Noreturn void Entry(void){

    OLED_DMA_Init();
    BSP_CAN_init();

    OLED_DMA_Clear();

    OLED_DMA_Display_DIG(0,0,8);

    uint8_t data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};


    uint64_t lastf = OLED_DMA_F_Count;

    while (1){

        uint32_t f = OLED_DMA_F_Count-lastf;
        lastf = OLED_DMA_F_Count;

        OLED_DMA_ShowNum(100,0,f,3,12);

        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
        HAL_Delay(1000);

    }

}
