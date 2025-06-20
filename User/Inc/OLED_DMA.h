#ifndef G474_EDC_DIGPWR_OLED_DMA_H
#define G474_EDC_DIGPWR_OLED_DMA_H

#include "main.h"

#define GRAM_T(p,x) GRAM_Fix[128 * ((p+1)%8) + (x)]
extern uint8_t* GRAM_Fix;
extern volatile uint64_t OLED_DMA_F_Count;

void OLED_DMA_Init(void);

void OLED_DMA_Fill_Val(uint8_t v);
void OLED_DMA_Fill_ALL(void);
void OLED_DMA_Clear(void);

void OLED_DMA_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t Char_Size);
void OLED_DMA_ShowNum(uint8_t x,uint8_t y,unsigned int num,uint8_t len,uint8_t size2);
void OLED_DMA_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t Char_Size);
void OLED_DMA_Display_DIG(uint8_t x,uint8_t y,uint8_t v);

#endif //G474_EDC_DIGPWR_OLED_DMA_H
