#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "circle_buffer.h"
#include <string.h>

#define USART_REC_LEN  			(200) 	//定义最大接收字节数 200
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收
	  	
extern u8 usart_rx_buf[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8 usart_rx_flag;       //接收状态标记
extern u8 usart_rx_len;      //接收的数据长度
extern u8 usart_send_flag;	//发送状态标记
extern circle_buf usart_rx_circle_buf;

void Usart_Init(u32 bound);

void Usart_DMA_Init(void);
void Usart_DMA_Clear(void);
void Usart_DMA_Send(u8 *buf, u16 len);

void Usart_Printf(char *fmt, ...);
#endif


