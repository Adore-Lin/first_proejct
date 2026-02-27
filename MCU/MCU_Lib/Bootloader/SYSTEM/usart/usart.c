#include "sys.h"
#include "usart.h"	 
////////////////////////////////////////////////////////////////////////////////// 	 
//如果使用ucos,则包括下面的头文件即可.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos 使用	  
#endif

//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//重定义fputc函数 
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//循环发送,直到发送完毕   
    USART1->DR = (u8) ch;      
	return ch;
}
#endif 

/*使用microLib的方法*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/
 
//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
u8 usart_rx_buf[USART_REC_LEN] = {0};     //接收缓冲,最大USART_REC_LEN个字节.
u8 usart_rx_flag = 0;       //接收状态标记
u8 usart_rx_len = 0;      //接收的数据长度

u8 usart_send_buf[USART_REC_LEN] = {0};    //发送缓冲,最大USART_REC_LEN个字节.
u8 usart_send_flag = 0;		//发送状态标记
u8 usart_send_len = 0;     //发送的数据长度
  
void uart_init(u32 bound)
{
  	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
	
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9

	//USART1_RX	  GPIOA.10初始化
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

	//Usart1 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

	USART_Init(USART1, &USART_InitStructure); //初始化串口1
	USART_Cmd(USART1, ENABLE);                    //使能串口1 

	USART_ITConfig(USART1,USART_IT_TC,ENABLE);//开启串口传输完成中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//开启串口空闲中断

	//配置DMA传输
	DMA_DeInit(DMA1_Channel4);
	DMA_DeInit(DMA1_Channel5);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA1时钟

	//DMA1_Channel5 USART1_RX
	DMA_InitStructure.DMA_BufferSize = sizeof(usart_rx_buf);//接收缓冲区的大小
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;//外设作为数据传的源
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//禁止内存到内存传输
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart_rx_buf;//内存地址
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//内存数据宽度为8位
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址自动增加
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;//工作在正常模式
	DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&USART1->DR;//外设地址
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据宽度为8位
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址自动增加
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高优先级
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);//根据DMA_InitStruct中指定的参数初始化DMA外设

	//DMA1_Channel4 USART1_TX
	DMA_InitStructure.DMA_BufferSize = 0;//发送缓冲区的大小，初始化为0不发送
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;//外设作为数据传输的目的
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)usart_send_buf;//内存地址，初始化为0不发送
	DMA_Init(DMA1_Channel4, &DMA_InitStructure);//根据DMA_InitStruct中指定的参数初始化DMA外设
	
	DMA_Cmd(DMA1_Channel5, ENABLE);
	DMA_Cmd(DMA1_Channel4, ENABLE);

	USART_DMACmd(USART1, USART_DMAReq_Tx | USART_DMAReq_Rx, ENABLE);
}

/*清除DMA的接收数量寄存器*/
void usart_dma_clear(void)
{
	DMA_Cmd(DMA1_Channel5, DISABLE);
	DMA_SetCurrDataCounter(DMA1_Channel5, sizeof(usart_rx_buf));
	DMA_Cmd(DMA1_Channel5, ENABLE);
}

/*串口1发送数据*/
void usart_dma_send(u8 *buf, u16 len)
{ 
	u8 send_len;
	
	if(len == 0 || buf == NULL)
	{
		return;
	}

	send_len = (len > sizeof(usart_send_buf)) ? sizeof(usart_send_buf) : len; //限制发送长度不超过缓冲区大小

	while(DMA_GetCurrDataCounter(DMA1_Channel4));//等待DMA传输完成

	memcpy(usart_send_buf, buf, send_len); //将数据复制到发送缓冲区

	DMA_Cmd(DMA1_Channel4, DISABLE); //先禁止DMA传输
	DMA_SetCurrDataCounter(DMA1_Channel4, send_len);//设置DMA传输数量
	DMA_Cmd(DMA1_Channel4, ENABLE); //使能DMA传输
}

//串口1中断服务程序
void USART1_IRQHandler(void)                	
{
	u8 clear;

	if (USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		clear = USART1->SR;	//读取SR寄存器清除中断标志
		clear = USART1->DR;	//读取DR寄存器清除中断标志

		usart_rx_flag = 1; //设置接收完成标志
		usart_rx_len = sizeof(usart_rx_buf) - DMA_GetCurrDataCounter(DMA1_Channel5); //计算接收的数据长度
    } 

	if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)  //发送完成中断
	{
		USART_ClearITPendingBit(USART1, USART_IT_TC); //清除发送完成中断标志
		DMA_Cmd(DMA1_Channel4, DISABLE); //禁止DMA传输
		usart_send_flag = 1;
	}
} 
/*
*clear 变量的作用，为什么需要读取 SR 和 DR？
在 STM32 的 USART 外设中，当发生空闲中断（IDLE interrupt）时，硬件会自动将接收缓冲区中的数据通过 DMA 传输到内存中。
所以为了清除这个中断标志，必须按照 STM32 的要求执行以下操作：
	读取状态寄存器 (SR)：这是为了确认中断的发生，并准备清除中断标志。
	读取数据寄存器 (DR)：这是关键步骤，读取 DR 寄存器会清除 IDLE 中断标志。
	如果不执行这两个读取操作，中断标志不会被清除，可能会导致中断持续触发或无法正确处理后续的中断。
*/
	



