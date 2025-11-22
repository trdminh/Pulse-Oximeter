#ifndef __MAX_30102_H_
#define __MAX_30102_H_

#include <stdio.h>
#include "stm32f10x.h"

#define MAX30102_I2C_ADDR 0x57
#define BUFFER_SIZE 25
#define MAX_PEAKS 5

//register addresses
#define REG_INTR_STATUS_1 0x00
#define REG_INTR_STATUS_2 0x01
#define REG_INTR_ENABLE_1 0x02
#define REG_INTR_ENABLE_2 0x03
#define REG_FIFO_WR_PTR 0x04
#define REG_OVF_COUNTER 0x05
#define REG_FIFO_RD_PTR 0x06
#define REG_FIFO_DATA 0x07
#define REG_FIFO_CONFIG 0x08
#define REG_MODE_CONFIG 0x09
#define REG_SPO2_CONFIG 0x0A
#define REG_LED1_PA 0x0C
#define REG_LED2_PA 0x0D
#define REG_PILOT_PA 0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 0x1F
#define REG_TEMP_FRAC 0x20
#define REG_TEMP_CONFIG 0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 0xFE
#define REG_PART_ID 0xFF
void I2C_Config(void);
void delay_ms(uint32_t ms);
void MAX30102_ReadFIFO(uint32_t *red, uint32_t *ir);
void MAX30102_Init(void);
void MAX30102_WriteReg(uint8_t reg, uint8_t value);
uint8_t MAX30102_ReadReg(uint8_t reg);
float calculate_spo2(uint32_t *red_data, uint32_t *ir_data);
int detect_peaks(uint32_t *data, int size, int *peaks);
float calculate_heart_rate(uint32_t *ir_buffer, int buffer_size, float fs);

#endif