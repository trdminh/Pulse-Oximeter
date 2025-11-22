#include "max30102.h"

void I2C_Config(void)
{
    I2C_InitTypeDef I2C_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE); 
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x00; 
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000; 

    I2C_Init(I2C1, &I2C_InitStructure);
    I2C_Cmd(I2C1, ENABLE);
}
void MAX30102_WriteReg(uint8_t reg, uint8_t value)
{
uint32_t timeout = 10000;
    

    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && timeout--);
    

    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && timeout--);
    

    I2C_Send7bitAddress(I2C1, MAX30102_I2C_ADDR << 1, I2C_Direction_Transmitter);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && timeout--);

    I2C_SendData(I2C1, reg);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && timeout--);
    

    I2C_SendData(I2C1, value);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && timeout--);
    

    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t MAX30102_ReadReg(uint8_t reg)
{
	uint8_t value;
    uint32_t timeout = 10000;
    

    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && timeout--);
    

    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && timeout--);
    

    I2C_Send7bitAddress(I2C1, MAX30102_I2C_ADDR << 1, I2C_Direction_Transmitter);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && timeout--);
    

    I2C_SendData(I2C1, reg);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && timeout--);
    

    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && timeout--);
    

    I2C_Send7bitAddress(I2C1, MAX30102_I2C_ADDR << 1, I2C_Direction_Receiver);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && timeout--);
    

    I2C_AcknowledgeConfig(I2C1, DISABLE);
    

    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) && timeout--);
    

    value = I2C_ReceiveData(I2C1);
    

    I2C_GenerateSTOP(I2C1, ENABLE);
    
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    
    return value;
}

void MAX30102_ReadFIFO(uint32_t *red, uint32_t *ir)
{
    uint8_t data[6];
    uint32_t timeout = 10000;
    int i;

    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && timeout--);
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && timeout--);
    I2C_Send7bitAddress(I2C1, MAX30102_I2C_ADDR << 1, I2C_Direction_Transmitter);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && timeout--);
    I2C_SendData(I2C1, REG_FIFO_DATA);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && timeout--);


    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && timeout--);
    I2C_Send7bitAddress(I2C1, MAX30102_I2C_ADDR << 1, I2C_Direction_Receiver);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && timeout--);

    for(i = 0; i < 6; i++)
    {
        if(i == 5)
            I2C_AcknowledgeConfig(I2C1, DISABLE); 
        timeout = 10000;
        while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) && timeout--);
        data[i] = I2C_ReceiveData(I2C1);
    }
    I2C_GenerateSTOP(I2C1, ENABLE);
    I2C_AcknowledgeConfig(I2C1, ENABLE);


    *red = ((uint32_t)data[0] << 16) | ((uint32_t)data[1] << 8) | data[2];
    *ir = ((uint32_t)data[3] << 16) | ((uint32_t)data[4] << 8) | data[5];
}

void MAX30102_Init(void)
{

    MAX30102_WriteReg(REG_MODE_CONFIG, 0x40);
    delay_ms(10);

    MAX30102_WriteReg(REG_INTR_ENABLE_1, 0x40);
		
		MAX30102_WriteReg(REG_INTR_ENABLE_2, 0x00);
	
		MAX30102_WriteReg(REG_FIFO_WR_PTR, 0x00);
	
		MAX30102_WriteReg(REG_OVF_COUNTER, 0x00);
	
		MAX30102_WriteReg(REG_FIFO_RD_PTR, 0x00);
	
		MAX30102_WriteReg(REG_FIFO_CONFIG, 0x0f);
	
		MAX30102_WriteReg(REG_MODE_CONFIG, 0x03);
	
		MAX30102_WriteReg(REG_SPO2_CONFIG, 0x27);
	
		MAX30102_WriteReg(REG_LED1_PA, 0x24);
	
		MAX30102_WriteReg(REG_LED2_PA, 0x24);
	
		MAX30102_WriteReg(REG_PILOT_PA, 0x7f);

}

float calculate_spo2(uint32_t *red_data, uint32_t *ir_data)
{
    uint32_t red_dc = 0, ir_dc = 0;
    uint32_t red_ac = 0, ir_ac = 0;
    int i;

    for(i = 0; i < BUFFER_SIZE; i++)
    {
        red_dc += red_data[i];
        ir_dc += ir_data[i];
    }
    red_dc /= BUFFER_SIZE;
    ir_dc /= BUFFER_SIZE;
    

    uint32_t red_min = red_data[0], red_max = red_data[0];
    uint32_t ir_min = ir_data[0], ir_max = ir_data[0];
    for(i = 1; i < BUFFER_SIZE; i++)
    {
        if(red_data[i] < red_min) red_min = red_data[i];
        if(red_data[i] > red_max) red_max = red_data[i];
        if(ir_data[i] < ir_min) ir_min = ir_data[i];
        if(ir_data[i] > ir_max) ir_max = ir_data[i];
    }
    red_ac = red_max - red_min;
    ir_ac = ir_max - ir_min;
    

    float R = (float)red_ac / red_dc / ((float)ir_ac / ir_dc);
    

    float spo2 = 110 - 25 * R;
    if(spo2 > 100) spo2 = 100;
    if(spo2 < 0) spo2 = 0;
    return spo2;
}

int detect_peaks(uint32_t *data, int size, int *peaks) {
    int count = 0;
    for (int i = 1; i < size - 1; i++) {
        if (data[i] > data[i - 1] && data[i] > data[i + 1]) {
            peaks[count++] = i;
            if (count >= MAX_PEAKS) break;
        }
    }
    return count;
}

float calculate_heart_rate(uint32_t *ir_buffer, int buffer_size, float fs) {
    int peaks[MAX_PEAKS];
    int peak_count = detect_peaks(ir_buffer, buffer_size, peaks);

    if (peak_count < 2) {
        return -1; 
    }

    float total_interval = 0.0f;
    for (int i = 1; i < peak_count; i++) {
        total_interval += (peaks[i] - peaks[i - 1]);
    }

    float avg_interval = total_interval / (peak_count - 1);

    float heart_rate = 60.0f * fs / avg_interval;

    return heart_rate/3.25;
}

void delay_ms(uint32_t ms)
{
    volatile uint32_t i;
    for(; ms > 0; ms--)
    {
        for(i = 0; i < 12000; i++); 
    }
}