#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"

#include "usart.h"


/************************************************
ALIENTEK 精英STM32F103开发板

驱动：
2个LED		已验证
2个按键key 	已验证
8: 按键输入实验：将 STM32F1 的 IO 口作为外部中断输入，以中断的方式利用板载的 3 个按键， 来控制板载的两个 LED 的亮灭

13：定时器中断 实验：使用 TIM3 的定时器中断来控制 DS1 的翻转，在主函数中用 DS0 的翻转来提示程序正在运行
14：定时器输出 PWM实验：使用 TIM3 的通道2，把通道2重映射到PB5，产生PWM来控制DS0的亮度
15: 定时器输入 捕获实验：使用 TIM5 的通道1（PA0） 来做输入捕获， 捕获 PA0 上高电平的脉宽（用 WK_UP 按键输入高电平），通过串口打印高电平脉宽时间，

************************************************/

/*串口通信测试：实现串口助手自发自收*/
void usart_test(void)
{
	if (usart_rx_flag == 1)
	{
		usart_rx_flag = 0;
		usart_dma_clear(); //清除DMA的接收数量寄存器，为下一次接收做准备
		usart_dma_send(usart_rx_buf, usart_rx_len); //将接收到的数据通过DMA发送回去
		memset(usart_rx_buf, 0, sizeof(usart_rx_buf)); //清空接收缓冲区
	}	
	
}

void Hardware_Init(void)
{
	LED_Init();
	KEY_Init();

	uart_init(115200);
	
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
		usart_test();
	}
}

