#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include "circle_buffer.h"
#include <string.h>

#define USART_RX_SIZE           2048
#define USART_RX_MAX  			(256) 	//定义最大接收字节数 200
#define NUM                     10
#define EN_USART1_RX 			1		//使能（1）/禁止（0）串口1接收

/*DMA接收缓冲区*/
extern u8 usart_rx_buf[USART_RX_SIZE];     //接收缓冲,最大USART_REC_LEN个字节.末字节为换行符 
extern u8 usart_rx_flag;       //接收状态标记
extern u8 usart_rx_len;      //接收的数据长度
extern u8 usart_send_flag;	//发送状态标记


/*缓冲区数组结构二*/
typedef struct
{
	uint8_t *start;
	uint8_t *end;
}rx_buffer_t;

typedef struct
{
	uint16_t rx_count;
    rx_buffer_t rx_data[NUM];
    rx_buffer_t *rx_data_in;
    rx_buffer_t *rx_data_out;
    rx_buffer_t *rx_data_end;
}rx_circle_buf_t;


extern rx_circle_buf_t rx_circle_buf;


void Usart_Init(u32 bound);

void Usart_DMA_Init(void);
void Usart_DMA_Clear(void);
void Usart_DMA_Send(u8 *buf, u16 len);

void Usart_Printf(char *fmt, ...);

void rx_buff_init(void);

#endif


