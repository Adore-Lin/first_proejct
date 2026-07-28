#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"

#include "usart.h"
#include "24cxx.h"
#include "w25qxx.h"
#include "iap.h"
#include "circle_buffer.h"

/************************************************
ALIENTEK 精英STM32F103开发板

驱动：
2个LED		已验证
2个按键key 	已验证
8: 按键输入实验：将 STM32F1 的 IO 口作为外部中断输入，以中断的方式利用板载的 3 个按键， 来控制板载的两个 LED 的亮灭

************************************************/

const u8 AT24C02_Buff[100] = "AT24C02_Buff";
const u8 W25QXX_Buff[100] = "W25QXX_Buff";

u8 usart_send_date[100];

/*串口通信测试：实现串口助手自发自收*/
void Usart_DMA_Test(void)
{
	if (usart_rx_flag == 1)
	{
		usart_rx_flag = 0;

		Usart_DMA_Clear(); //清除DMA的接收数量寄存器，为下一次接收做准备
		Usart_DMA_Send(usart_rx_buf, usart_rx_len); //将接收到的数据通过DMA发送回去
		memset(usart_rx_buf, 0, sizeof(usart_rx_buf)); //清空接收缓冲区
	}	
}

void Usart_Circle_Buf_Test(void)
{
	u16 i;
	u32 len;
	if (rx_circle_buf.rx_data_in != rx_circle_buf.rx_data_out)
	{
		len = rx_circle_buf.rx_data_out->end - rx_circle_buf.rx_data_out->start+1;
		Usart_Printf("rx_data size: %d\r\n", len);
		for(i = 0; i < len; i++)
		{
			Usart_Printf("%c", rx_circle_buf.rx_data_out->start[i]);	
		}

		rx_circle_buf.rx_data_out++;
		if (rx_circle_buf.rx_data_out == rx_circle_buf.rx_data_end)
		{
			rx_circle_buf.rx_data_out = &rx_circle_buf.rx_data[0];
		}
	}
}

/*AT24CXX测试：实现按下按键0写入数据，按下按键1读取数据，并且发送到串口助手, 按下按键时要停顿一会*/
void AT24CXX_Test(void)
{ 
	if (KEY_Scan(0) == KEY0_PRES)
	{
		AT24CXX_Write(0, (u8 *)AT24C02_Buff, sizeof(AT24C02_Buff));
		Usart_Printf("AT24C02_Write OK!\r\n");
	}

	if (KEY_Scan(0) == KEY1_PRES)
	{
		AT24CXX_Read(0, (u8 *)usart_send_date, sizeof(usart_send_date));	
		Usart_Printf("AT24C02_Read: %s\r\n", usart_send_date);
	}

	if (usart_send_flag == 1)
	{
		usart_send_flag = 0;
	}

}

void W25QXX_Test(void)
{ 
}

void Hardware_Init(void)
{
	LED_Init();
	KEY_Init();

	Usart_Init(115200);
	AT24CXX_Init();
	W25QXX_Init();
}

int main(void)
{	
	//变量初始化
	// u16 led0_pwm = 0;
	// u8 dir = 1;
//	SCB->VTOR = FLASH_BASE | 0x10000;
//	sys_nvic_set_vector_table(FLASH_BASE, 0x10000);
	//函数初始化
	delay_init();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
 
	//驱动初始化
	Hardware_Init();
	Usart_Printf("APP LOAD  Test!\r\n");
	
	 
	while(1)
	{
		Usart_DMA_Test();
	
	}
}


