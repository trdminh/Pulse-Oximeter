#ifndef TFT_H
#define TFT_H

#include "stm32f10x.h"

// Define pins for TFT
#define TFT_SPI         SPI1
#define TFT_SPI_CLK     RCC_APB2Periph_SPI1

#define TFT_CS_PORT     GPIOB
#define TFT_CS_PIN      GPIO_Pin_10
#define TFT_CS_CLK      RCC_APB2Periph_GPIOB

#define TFT_DC_PORT     GPIOB
#define TFT_DC_PIN      GPIO_Pin_1
#define TFT_DC_CLK      RCC_APB2Periph_GPIOB

#define TFT_RST_PORT    GPIOB
#define TFT_RST_PIN     GPIO_Pin_0
#define TFT_RST_CLK     RCC_APB2Periph_GPIOB

#define TFT_BL_PORT     GPIOA
#define TFT_BL_PIN      GPIO_Pin_1
#define TFT_BL_CLK      RCC_APB2Periph_GPIOA

#define TFT_PWR_PORT    GPIOA
#define TFT_PWR_PIN     GPIO_Pin_2
#define TFT_PWR_CLK     RCC_APB2Periph_GPIOA

#define TFT_SCK_PORT    GPIOA
#define TFT_SCK_PIN     GPIO_Pin_5
#define TFT_MISO_PORT   GPIOA
#define TFT_MISO_PIN    GPIO_Pin_6
#define TFT_MOSI_PORT   GPIOA
#define TFT_MOSI_PIN    GPIO_Pin_7

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


#define TFT_WIDTH  160
#define TFT_HEIGHT 128


void TFT_Init(void);
void TFT_WriteCommand(uint8_t cmd);
void TFT_WriteData(uint8_t data);
void TFT_WriteData16(uint16_t data);
void TFT_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void TFT_FillScreen(uint16_t color);
void TFT_ClearScreen(void);
void TFT_SetRotation(uint8_t rotation);
uint16_t TFT_GetWidth(void);
uint16_t TFT_GetHeight(void);

void TFT_TestOrientations(void);
void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void TFT_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg);
void TFT_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg);
void TFT_DisplayOff(void);
void TFT_DisplayOn(void);
void TFT_PowerOff(void);
void TFT_PowerOn(void);

#endif
