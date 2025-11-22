#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdio.h>
#include "tft.h"
#include "max30102.h"
#include "uart.h"
#define STACK_SIZE 256
#define PRIORITY_SENSOR 1
#define PRIORITY_TFT 2



char buffer[50];
uint32_t ir_buffer[BUFFER_SIZE];
uint32_t red_buffer[BUFFER_SIZE];
int buffer_index = 0;

float global_heart_rate = 0.0f;
float global_spo2 = 0.0f;

// Device active flag
int device_active = 1;
int zero_count = 0;

#define HR_HISTORY_SIZE 25
float hr_history[HR_HISTORY_SIZE];
int hr_index = 0;

void I2C_ScanAddresses(void);
uint8_t I2C_CheckAddress(uint8_t address);

void vTaskReadSensor(void *pvParameters);
void vTaskDisplayTFT(void *pvParameters);
void vTaskButton(void *pvParameters);
int main(void){
		SystemInit();
		RCC_DeInit(); 
		RCC_HSEConfig(RCC_HSE_OFF); 
		RCC_HSICmd(ENABLE); 
		while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY) == RESET); 
		TFT_Init();
		GPIO_Config();
		
		UART_Init();
		I2C_Config();
		MAX30102_Init();
		UART_SendString("Start \r\n");
		TFT_FillScreen(BLACK);
		UART_SendString("Scanning I2C addresses...\r\n");
        TFT_ClearScreen();
        TFT_FillScreen(BLACK);
    I2C_ScanAddresses();

    UART_SendString("MAX30102 initialized.\r\n");
		if (xTaskCreate(vTaskReadSensor, "MAX30102", STACK_SIZE, NULL, PRIORITY_SENSOR, NULL) != pdPASS)
		{
			UART_SendString("Failed to read sensor task \r\n");
		}
		
		if (xTaskCreate(vTaskDisplayTFT, "TFT Display", STACK_SIZE, NULL, PRIORITY_TFT, NULL) != pdPASS)
		{
			UART_SendString("Failed to create TFT display task \r\n");
		}
		
		if (xTaskCreate(vTaskButton, "Button", STACK_SIZE, NULL, PRIORITY_TFT + 1, NULL) != pdPASS)
		{
			UART_SendString("Failed to create button task \r\n");
		}
		
		vTaskStartScheduler();
		while(1)
		{
			
			
		}

}

uint8_t I2C_CheckAddress(uint8_t address)
{
    uint32_t timeout = 10000;
    uint8_t success = 0;
    
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && timeout)
    {
        timeout--;
    }
    
    if (timeout == 0) return 0; 
    
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = 10000;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && timeout)
    {
        timeout--;
    }
    
    if (timeout == 0) {
        I2C_GenerateSTOP(I2C1, ENABLE);
        return 0; 
    }

    I2C_Send7bitAddress(I2C1, (uint8_t)(address << 1), I2C_Direction_Transmitter);

    timeout = 10000;
    while(timeout--)
    {
        if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
        {
            success = 1;
            break;
        }

        if(I2C_GetFlagStatus(I2C1, I2C_FLAG_AF))
        {
            I2C_ClearFlag(I2C1, I2C_FLAG_AF);
            break;
        }
    }
    I2C_GenerateSTOP(I2C1, ENABLE);
    delay_ms(1); 
    
    return success;
}

void I2C_ScanAddresses(void)
{
    uint8_t address;
    uint8_t foundDevices = 0;

    for(address = 1; address < 128; address++)
    {
        if(I2C_CheckAddress(address))
        {
            sprintf(buffer, "Device found at address: 0x%02X (8-bit: 0x%02X)\r\n", address, address << 1);
            UART_SendString(buffer);
            
            if(address == 0x57) {
                UART_SendString("  This is MAX30102!\r\n");
            }
            
            foundDevices++;
        }
    }
    
    if(foundDevices == 0)
    {
        UART_SendString("No I2C devices found. Check connections.\r\n");
    }
}


void vTaskReadSensor(void *pvParameters)
{
    UART_SendString("Sensor task started\r\n");
    while(1)
    {
        if (!device_active)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        uint32_t red_value, ir_value;
        MAX30102_ReadFIFO(&red_value, &ir_value);
        if (ir_value >= 7000)
        {
            ir_buffer[buffer_index] = ir_value;
            red_buffer[buffer_index] = red_value;
            buffer_index++;
            zero_count = 0;
            if(buffer_index >= BUFFER_SIZE)
            {
                buffer_index = 0;
                float heart_rate = calculate_heart_rate(ir_buffer, BUFFER_SIZE, 25.0f);
                if (heart_rate < 0) heart_rate = 0;
                float spo2 = calculate_spo2(red_buffer, ir_buffer);
                global_heart_rate = heart_rate;
                global_spo2 = spo2;
                // Check for auto shutdown
                if (heart_rate == 0 && spo2 == 0)
                {
                    zero_count++;
                    if (zero_count >= 120)
                    {
                        device_active = 0;
                        uint8_t mode = MAX30102_ReadReg(REG_MODE_CONFIG);
                        MAX30102_WriteReg(REG_MODE_CONFIG, mode | 0x80);
                        GPIO_WriteBit(TFT_BL_PORT, TFT_BL_PIN, Bit_RESET);
                        UART_SendString("Shutdown\r\n");
                    }
                }
                else
                {
                    zero_count = 0;
                }
                // Store in history
                hr_history[hr_index] = heart_rate;
                hr_index = (hr_index + 1) % HR_HISTORY_SIZE;
                sprintf(buffer, "Heart Rate: %.1f BPM, SpO2: %.1f%%\r\n", heart_rate, spo2);
                UART_SendString(buffer);
            }
        }
        else
        {
            global_heart_rate = 0;
            global_spo2 = 0;
            hr_history[hr_index] = 0;
            hr_index = (hr_index + 1) % HR_HISTORY_SIZE;
            zero_count++;
            if (zero_count >= 120)
            {
                device_active = 0;
                uint8_t mode = MAX30102_ReadReg(REG_MODE_CONFIG);
                MAX30102_WriteReg(REG_MODE_CONFIG, mode | 0x80);
                GPIO_WriteBit(TFT_BL_PORT, TFT_BL_PIN, Bit_RESET);
                UART_SendString("Shut down\r\n");
            }
            sprintf(buffer, "Heart Rate: 0.0 BPM, SpO2: 0.0%%\r\n");
            UART_SendString(buffer);
        }
        vTaskDelay(pdMS_TO_TICKS(100));  
    }
}

void vTaskDisplayTFT(void *pvParameters)
{
    UART_SendString("TFT task started\r\n");
    char display_buffer[50];
    float last_hr = -1.0f, last_spo2 = -1.0f;  
    while(1)
    {
        if (!device_active)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }
        TFT_FillRect(0, 50, 159, 127, BLACK);  
        for(int i = 0; i < HR_HISTORY_SIZE - 1; i++)
        {
            int idx1 = (hr_index + i) % HR_HISTORY_SIZE;
            int idx2 = (hr_index + i + 1) % HR_HISTORY_SIZE;
            float hr1 = hr_history[idx1];
            float hr2 = hr_history[idx2];
            if(hr1 >= 0 && hr2 >= 0)  // Draw if valid (including 0)  
            {
                int x1 = i * 159 / (HR_HISTORY_SIZE - 1);
                int x2 = (i + 1) * 159 / (HR_HISTORY_SIZE - 1);
                int y1 = 101 - (int)((hr1 / 200.0f) * 51);
                int y2 = 101 - (int)((hr2 / 200.0f) * 51);
                TFT_DrawLine(x1, y1, x2, y2, GREEN);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void vTaskButton(void *pvParameters)
{
    UART_SendString("Button task started\r\n");
    while(1)
    {
        if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_RESET) 
        {
            device_active = !device_active;
            if(device_active)
            {
                // Wake up
                zero_count = 0;
                uint8_t mode = MAX30102_ReadReg(REG_MODE_CONFIG);
                MAX30102_WriteReg(REG_MODE_CONFIG, mode & ~0x80);
                GPIO_WriteBit(TFT_BL_PORT, TFT_BL_PIN, Bit_SET); 
                UART_SendString("Device activated\r\n");
            }
            else
            {
                // Shutdown
                uint8_t mode = MAX30102_ReadReg(REG_MODE_CONFIG);
                MAX30102_WriteReg(REG_MODE_CONFIG, mode | 0x80);
                GPIO_WriteBit(TFT_BL_PORT, TFT_BL_PIN, Bit_RESET); 
                TFT_ClearScreen();
                TFT_FillScreen(BLACK); 
                UART_SendString("Device deactivated\r\n");
                
            }
            vTaskDelay(pdMS_TO_TICKS(500)); 
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}