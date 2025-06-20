#include "OLED_DMA.h"
#include "i2c.h"
#include "string.h"
#include "oledfont.h"
#include "oled_dig_font.h"

#define hi2cX hi2c1
#define GRAM_SIZE 1024

uint8_t GRAM[1025] = {0};
uint8_t* GRAM_Fix;
volatile uint64_t OLED_DMA_F_Count;

uint8_t set_cmds[] = {
        0x00,             //CMD Start
        0xAE,             // Display OFF
        0x20, 0x00,       // Memory addressing mode = horizontal
        0xB0,             // Page 0
        0xC8,             // COM scan direction remapped
        0x00,             // Low column address
        0x10,             // High column address
        0x40,             // Start line address
        0x81, 0x7F,       // Contrast
        0xA1,             // Segment remap
        0xA6,             // Normal display
        0xA8, 0x3F,       // Multiplex ratio 1/64
        0xA4,             // Display RAM content
        0xD3, 0x00,       // Display offset
        0xD5, 0x80,       // Display clock divide
        0xD9, 0xF1,       // Pre-charge period
        0xDA, 0x12,       // COM pins config
        0xDB, 0x40,       // VCOM detect
        0x8D, 0x14,       // Charge pump ON
        0xAF              // Display ON
};


inline void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef *hi2c){
    OLED_DMA_F_Count++;
    HAL_I2C_Master_Transmit_DMA(&hi2cX,0x78,GRAM,GRAM_SIZE+1);

}


void OLED_DMA_Init(void){

    GRAM_Fix = GRAM + 1;

//    HAL_Delay(50);
    HAL_I2C_Master_Transmit(&hi2cX, 0x78, set_cmds, sizeof(set_cmds), 0xffff);

    GRAM[0] = 0x40;


    uint8_t tmp_cmds[4] = {
            0x00,                   // 控制字节：命令
            0xB0 | 0,            // 设置页地址 B0-B7
            ((0&0xf0)>>4)|0x10,        // 设置低列地址（低4位）
            0&0x0f,                    // 设置高列地址（高4位）
    };

    HAL_I2C_Master_Transmit(&hi2cX, 0x78, tmp_cmds,4 , 0xff);

    OLED_DMA_F_Count = 0;
    HAL_I2C_Master_Transmit_DMA(&hi2cX,0x78,GRAM,GRAM_SIZE+1);
}

inline void OLED_DMA_Fill_Val(uint8_t v){
    memset(GRAM_Fix, v, GRAM_SIZE);
}
inline void OLED_DMA_Fill_ALL(void){
    OLED_DMA_Fill_Val(0xff);
}
inline void OLED_DMA_Clear(void){
    OLED_DMA_Fill_Val(0);
}



unsigned int OLED_DMA_pow(uint8_t m,uint8_t n)
{
    unsigned int result=1;
    while(n--)result*=m;
    return result;
}

//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~8
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
inline void OLED_DMA_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size)
{
    unsigned char c=0,i=0;
    c=chr-' ';//得到偏移后的值

    if(Char_Size == 16){

        for(i=0;i<8;i++){
            GRAM_T(y,x+i) = F8x16[c*16+i];
            GRAM_T(y+1,x+i) = F8x16[c*16+i+8];
        }

    }
    else{
        for(i=0;i<6;i++)
            GRAM_T(y,x+i) = F6x8[c][i];
    }
}


//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OLED_DMA_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2)
{
    uint8_t t,temp;
    uint8_t enshow=0;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_DMA_pow(10,len-t-1))%10;
        if(enshow==0&&t<(len-1))
        {
            if(temp==0)
            {
                OLED_DMA_ShowChar(x+(size2/2)*t,y,' ',size2);
                continue;
            }else enshow=1;

        }
        OLED_DMA_ShowChar(x+(size2/2)*t,y,temp+'0',size2);
    }
}

//显示一个字符号串
void OLED_DMA_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size)
{
    unsigned char j=0;
    while (chr[j]!='\0')
    {		OLED_DMA_ShowChar(x,y,chr[j],Char_Size);
        x+=8;
        if(x>120){x=0;y+=2;}
        j++;
    }
}


void OLED_DMA_Display_DIG(uint8_t x,uint8_t y,uint8_t v){

    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 16; i++)
            GRAM_T(y+j,x+i) = F16x32_dig[v][i+16*j];

}
