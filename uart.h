#ifndef __UART_H_
#define __UART_H_

#include "stdio.h"
#include "stm32f10x.h"

void GPIO_Config(void);
void UART_Init(void);
void UART_SendString(char *str);

#endif
