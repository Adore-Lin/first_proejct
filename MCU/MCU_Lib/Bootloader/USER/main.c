#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"

#include "usart.h"
#include "24cxx.h"
#include "w25qxx.h"

/************************************************
ALIENTEK 精英STM32F103开发板

驱动：
2个LED		已验证
2个按键key 	已验证
8: 按键输入实验：将 STM32F1 的 IO 口作为外部中断输入，以中断的方式利用板载的 3 个按键， 来控制板载的两个 LED 的亮灭

************************************************/

/*串口通信测试：实现串口助手自发自收*/
void Usart_Test(void)
{
	if (usart_rx_flag == 1)
	{
		usart_rx_flag = 0;
		Usart_DMA_Clear(); //清除DMA的接收数量寄存器，为下一次接收做准备
		Usart_DMA_Send(usart_rx_buf, usart_rx_len); //将接收到的数据通过DMA发送回去
		memset(usart_rx_buf, 0, sizeof(usart_rx_buf)); //清空接收缓冲区
	}	
	
}

void Hardware_Init(void)
{
	LED_Init();
	KEY_Init();

	Uart_Init(115200);
	AT24CXX_Init();
	W25QXX_Init();
}

int main(void)
{	
	//变量初始化
	// u16 led0_pwm = 0;
	// u8 dir = 1;
	
	//函数初始化
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
	//驱动初始化
	Hardware_Init();

	 
	while(1)
	{
		Usart_Test();
	}
}

