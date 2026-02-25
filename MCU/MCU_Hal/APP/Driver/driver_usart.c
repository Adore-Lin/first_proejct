#include "main.h"
#include "driver_usart.h"
#include <stdio.h>
UART_HandleTypeDef husart;

void UsartInit(uint32_t baudrate)
{
	husart.Instance 		= USART1;
	husart.Init.BaudRate	= baudrate;
	husart.Init.WordLength 	= USART_WORDLENGTH_8B;
	husart.Init.StopBits	= USART_STOPBITS_1;
	husart.Init.Parity 		= USART_PARITY_NONE;
	husart.Init.Mode 		= USART_MODE_TX_RX;
	husart.Init.HwFlowCtl	= UART_HWCONTROL_NONE;

	if(HAL_UART_Init(&husart) != HAL_OK)
	{
		Error_Handler();
	}
	
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef	GPIO_InitStruct = {0};
	
	if(husart.Instance == USARTx)
	{
		USARTx_CLK_EN();
		USARTx_GPIO_CLK_EN();
		
		GPIO_InitStruct.Pin = USARTx_TX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);
		
		GPIO_InitStruct.Pin = USARTx_RX_PIN;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_INPUT;
		HAL_GPIO_Init(USARTx_PORT, &GPIO_InitStruct);
	}
}

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&husart, (uint8_t *)&ch, 1, 10);
	return ch;
}

int fgetc(FILE *f)
{
	uint8_t ch =0;
	HAL_UART_Receive(&husart, (uint8_t *)&ch, 1, 10);
	return (int)ch;
}




