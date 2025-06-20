#include "Entry.h"
#include "oled.h"
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

    OLED_Init();
    BSP_CAN_init();

    OLED_Clear();

    displayF16x32_dig(0,0,6);

    uint8_t data[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    while (1){

        HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
        HAL_Delay(200);

    }

}
