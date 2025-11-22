#include "tft.h"
#include "uart.h"
#include <stdlib.h>
#include <stdio.h>

// Private functions
static void TFT_SPI_Init(void);
static void TFT_GPIO_Init(void);
static void TFT_Reset(void);
static void TFT_SendInitCommands(void);

static void TFT_SPI_Init(void) {
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;


    RCC_APB2PeriphClockCmd(TFT_SPI_CLK | RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Pin = TFT_SCK_PIN | TFT_MOSI_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(TFT_SCK_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = TFT_MISO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(TFT_MISO_PORT, &GPIO_InitStructure);

    // SPI configuration
    SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(TFT_SPI, &SPI_InitStructure);

    SPI_Cmd(TFT_SPI, ENABLE);
}

static void TFT_GPIO_Init(void) {
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(TFT_CS_CLK | TFT_DC_CLK | TFT_RST_CLK | TFT_BL_CLK, ENABLE);

    GPIO_InitStructure.GPIO_Pin = TFT_CS_PIN | TFT_DC_PIN | TFT_RST_PIN | TFT_BL_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(TFT_CS_PORT, &GPIO_InitStructure);

    GPIO_SetBits(TFT_CS_PORT, TFT_CS_PIN); 
    GPIO_SetBits(TFT_DC_PORT, TFT_DC_PIN); 
    GPIO_SetBits(TFT_RST_PORT, TFT_RST_PIN); 
    GPIO_SetBits(TFT_BL_PORT, TFT_BL_PIN); 
}

// Reset the TFT
static void TFT_Reset(void) {
    GPIO_ResetBits(TFT_RST_PORT, TFT_RST_PIN);
    for(volatile int i = 0; i < 10000; i++);
    GPIO_SetBits(TFT_RST_PORT, TFT_RST_PIN);
    for(volatile int i = 0; i < 10000; i++);
}


static void TFT_SendInitCommands(void) {
    TFT_WriteCommand(0x01); 
    for(volatile int i = 0; i < 150000; i++); 
    TFT_WriteCommand(0x11); 
    for(volatile int i = 0; i < 50000; i++); 

    TFT_WriteCommand(0xB1); 
    TFT_WriteData(0x01);
    TFT_WriteData(0x2C);
    TFT_WriteData(0x2D);

    TFT_WriteCommand(0xB2); 
    TFT_WriteData(0x01);
    TFT_WriteData(0x2C);
    TFT_WriteData(0x2D);

    TFT_WriteCommand(0xB3);
    TFT_WriteData(0x01);
    TFT_WriteData(0x2C);
    TFT_WriteData(0x2D);
    TFT_WriteData(0x01);
    TFT_WriteData(0x2C);
    TFT_WriteData(0x2D);

    TFT_WriteCommand(0xB4); 
    TFT_WriteData(0x07);

    TFT_WriteCommand(0xC0); 
    TFT_WriteData(0xA2);
    TFT_WriteData(0x02);
    TFT_WriteData(0x84);

    TFT_WriteCommand(0xC1);
    TFT_WriteData(0xC5);

    TFT_WriteCommand(0xC2);
    TFT_WriteData(0x0A);
    TFT_WriteData(0x00);

    TFT_WriteCommand(0xC3); 
    TFT_WriteData(0x8A);
    TFT_WriteData(0x2A);

    TFT_WriteCommand(0xC4);
    TFT_WriteData(0x8A);
    TFT_WriteData(0xEE);

    TFT_WriteCommand(0xC5);
    TFT_WriteData(0x0E);



    TFT_WriteCommand(0xE0); 
    TFT_WriteData(0x0F);
    TFT_WriteData(0x1A);
    TFT_WriteData(0x0F);
    TFT_WriteData(0x18);
    TFT_WriteData(0x2F);
    TFT_WriteData(0x28);
    TFT_WriteData(0x20);
    TFT_WriteData(0x22);
    TFT_WriteData(0x1F);
    TFT_WriteData(0x1B);
    TFT_WriteData(0x23);
    TFT_WriteData(0x37);
    TFT_WriteData(0x00);
    TFT_WriteData(0x07);
    TFT_WriteData(0x02);
    TFT_WriteData(0x10);

    TFT_WriteCommand(0xE1); 
    TFT_WriteData(0x0F);
    TFT_WriteData(0x1B);
    TFT_WriteData(0x0F);
    TFT_WriteData(0x17);
    TFT_WriteData(0x33);
    TFT_WriteData(0x2C);
    TFT_WriteData(0x29);
    TFT_WriteData(0x2E);
    TFT_WriteData(0x30);
    TFT_WriteData(0x30);
    TFT_WriteData(0x39);
    TFT_WriteData(0x3F);
    TFT_WriteData(0x00);
    TFT_WriteData(0x07);
    TFT_WriteData(0x03);
    TFT_WriteData(0x10);

    TFT_WriteCommand(0x2A);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x9F); 

    TFT_WriteCommand(0x2B);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x00);
    TFT_WriteData(0x7F); 
    TFT_WriteCommand(0x3A); 
    TFT_WriteData(0x05); 

    TFT_WriteCommand(0x29); 
}

// Public functions

void TFT_Init(void) {
    TFT_GPIO_Init();
    TFT_SPI_Init();
    TFT_Reset();
    TFT_SendInitCommands();
    TFT_SetRotation(1);
}

void TFT_WriteCommand(uint8_t cmd) {
    GPIO_ResetBits(TFT_CS_PORT, TFT_CS_PIN);
    GPIO_ResetBits(TFT_DC_PORT, TFT_DC_PIN);
    SPI_I2S_SendData(TFT_SPI, cmd);
    while(SPI_I2S_GetFlagStatus(TFT_SPI, SPI_I2S_FLAG_TXE) == RESET);
    GPIO_SetBits(TFT_CS_PORT, TFT_CS_PIN);
}

void TFT_WriteData(uint8_t data) {
    GPIO_ResetBits(TFT_CS_PORT, TFT_CS_PIN);
    GPIO_SetBits(TFT_DC_PORT, TFT_DC_PIN);
    SPI_I2S_SendData(TFT_SPI, data);
    while(SPI_I2S_GetFlagStatus(TFT_SPI, SPI_I2S_FLAG_TXE) == RESET);
    GPIO_SetBits(TFT_CS_PORT, TFT_CS_PIN);
}

void TFT_WriteData16(uint16_t data) {
    TFT_WriteData(data >> 8);
    TFT_WriteData(data & 0xFF);
}

void TFT_SetAddress(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    TFT_WriteCommand(0x2A); 
    TFT_WriteData16(x1);
    TFT_WriteData16(x2);

    TFT_WriteCommand(0x2B); 
    TFT_WriteData16(y1);
    TFT_WriteData16(y2);

    TFT_WriteCommand(0x2C); 
}

static uint16_t current_width = TFT_WIDTH;
static uint16_t current_height = TFT_HEIGHT;

void TFT_FillScreen(uint16_t color) {
    TFT_FillRect(0, 0, current_width - 1, current_height - 1, color);
}

void TFT_ClearScreen(void) {
    TFT_FillScreen(BLACK);
}

uint16_t TFT_GetWidth(void) {
    return current_width;
}

uint16_t TFT_GetHeight(void) {
    return current_height;
}

// Rotation helper: rotation = 0..3
// 0 = portrait, 1 = landscape, 2 = portrait inverted, 3 = landscape inverted
void TFT_SetRotation(uint8_t rotation) {
    uint8_t madctl = 0x00;
    switch(rotation & 0x03) {
        case 0: 
            madctl = 0x00; 
            current_width = 128;
            current_height = 160;
            break;
        case 1: 
            madctl = 0x60; 
            current_width = 160;
            current_height = 128;
            break;
        case 2: 
            madctl = 0xC0; 
            current_width = 128;
            current_height = 160;
            break;
        case 3:
            madctl = 0xA0; 
            current_width = 160;
            current_height = 128;
            break;
    }
    TFT_WriteCommand(0x36);
    TFT_WriteData(madctl);
}


static void tft_delay(volatile uint32_t cnt) {
    while(cnt--) { __asm("nop"); }
}


void TFT_TestOrientations(void) {
    const uint8_t rot_values[4] = {0x00, 0x60, 0xC0, 0xA0};
    char uart_buf[64];
    for (int bgr = 0; bgr <= 1; bgr++) {
        for (int r = 0; r < 4; r++) {
            uint8_t val = rot_values[r] | (bgr ? 0x08 : 0x00);
            TFT_WriteCommand(0x36);
            TFT_WriteData(val);


            if ((rot_values[r] & 0x60) == 0x60) { current_width = 160; current_height = 128; }
            else { current_width = 128; current_height = 160; }

            TFT_ClearScreen();
            TFT_DrawString(4, 8, "MADCTL Test", WHITE, BLACK);
            sprintf(uart_buf, "MADCTL=0x%02X (rot=%d bgr=%d)\r\n", val, r, bgr);
            UART_SendString(uart_buf);
            TFT_DrawString(4, 28, "0123456789", WHITE, BLACK);
            TFT_DrawString(4, 46, "AaBbCcDdEe", WHITE, BLACK);
            TFT_DrawString(4, 64, "Hello, ST7735!", WHITE, BLACK);

            tft_delay(1800000);
        }
    }
    UART_SendString("MADCTL test finished\r\n");
}

void TFT_DrawPixel(uint16_t x, uint16_t y, uint16_t color) {
    TFT_SetAddress(x, y, x, y);
    TFT_WriteData16(color);
}

void TFT_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while(1) {
        TFT_DrawPixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

void TFT_DrawRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    TFT_DrawLine(x1, y1, x2, y1, color);
    TFT_DrawLine(x2, y1, x2, y2, color);
    TFT_DrawLine(x2, y2, x1, y2, color);
    TFT_DrawLine(x1, y2, x1, y1, color);
}

void TFT_FillRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
    uint16_t i, j;
    TFT_SetAddress(x1, y1, x2, y2);
    for(i = y1; i <= y2; i++) {
        for(j = x1; j <= x2; j++) {
            TFT_WriteData16(color);
        }
    }
}

static const uint8_t font5x7[95][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // 32 space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // 33 !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // 34 "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // 35 #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // 36 $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // 37 %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // 38 &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // 39 '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // 40 (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // 41 )
    {0x14, 0x08, 0x3E, 0x08, 0x14}, // 42 *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // 43 +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // 44 ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // 45 -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // 46 .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // 47 /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 48 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 49 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 50 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 51 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 52 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 53 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 54 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 55 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 56 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 57 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // 58 :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // 59 ;
    {0x08, 0x14, 0x22, 0x41, 0x00}, // 60 <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // 61 =
    {0x00, 0x41, 0x22, 0x14, 0x08}, // 62 >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // 63 ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // 64 @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // 65 A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // 66 B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // 67 C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // 68 D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // 69 E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // 70 F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // 71 G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // 72 H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // 73 I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // 74 J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // 75 K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // 76 L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F}, // 77 M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // 78 N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // 79 O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // 80 P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // 81 Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // 82 R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // 83 S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // 84 T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // 85 U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // 86 V
    {0x3F, 0x40, 0x38, 0x40, 0x3F}, // 87 W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // 88 X
    {0x07, 0x08, 0x70, 0x08, 0x07}, // 89 Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // 90 Z
    {0x00, 0x7F, 0x41, 0x41, 0x00}, // 91 [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // 92 \
    {0x00, 0x41, 0x41, 0x7F, 0x00}, // 93 ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // 94 ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // 95 _
    {0x00, 0x01, 0x02, 0x04, 0x00}, // 96 `
    {0x20, 0x54, 0x54, 0x54, 0x78}, // 97 a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // 98 b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // 99 c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // 100 d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // 101 e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // 102 f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // 103 g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // 104 h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // 105 i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // 106 j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // 107 k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // 108 l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // 109 m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // 110 n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // 111 o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // 112 p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // 113 q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // 114 r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // 115 s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // 116 t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // 117 u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // 118 v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // 119 w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // 120 x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // 121 y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // 122 z
    {0x00, 0x08, 0x36, 0x41, 0x00}, // 123 {
    {0x00, 0x00, 0x7F, 0x00, 0x00}, // 124 |
    {0x00, 0x41, 0x36, 0x08, 0x00}, // 125 }
    {0x10, 0x08, 0x08, 0x10, 0x08}, // 126 ~
};

void TFT_DrawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg) {
    uint8_t i, j;
    uint8_t line;
    if (c < 32 || c > 126) return;
    c -= 32;
    TFT_SetAddress(x, y, x+4, y+6);
    for (i = 0; i < 7; i++) {
        line = font5x7[c][i];
        for (j = 0; j < 5; j++) {
            if (line & 0x01) {
                TFT_WriteData16(color);
            } else {
                TFT_WriteData16(bg);
            }
            line >>= 1;
        }
    }
}

void TFT_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color, uint16_t bg) {
    while (*str) {
        TFT_DrawChar(x, y, *str++, color, bg);
        x += 6; 
        if (x > 122) { 
            x = 0;
            y += 8;
        }
    }
}

void TFT_DisplayOff(void) {
    TFT_WriteCommand(0x28); // Display OFF
}

void TFT_DisplayOn(void) {
    TFT_WriteCommand(0x29); // Display ON
}