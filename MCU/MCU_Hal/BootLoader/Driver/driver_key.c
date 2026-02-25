#include "driver_key.h"
#include "driver_led.h"

void KeyInit(void)
{
	//定义GPIO的结构体变量
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	//使能按键的GPIO 对应的时钟
	KEY0_GPIO_CLK_EN();
	KEY1_GPIO_CLK_EN();
	
	GPIO_InitStruct.Pin   = KEY0_GPIO_PIN | KEY1_GPIO_PIN; // 选择按键的引脚
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;// 设置为推挽输出模式
	GPIO_InitStruct.Pull  = GPIO_PULLUP; // 默认上拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;// 引脚反转速度设置为快
	
	//初始化引脚配置
	HAL_GPIO_Init(KEY0_GPIO_PORT, &GPIO_InitStruct);
}

/*按键测试：按键轮询扫描，按下则灯亮*/
void KeyTest(void)
{
	if(KEY0 == PUSH_DOWN)
	{
		HAL_Delay(8);
		if(KEY0 == PUSH_DOWN)
		{
			R_LED(ON);
		}
	}
	else if(KEY0 == SPRING_UP)
	{
		HAL_Delay(8);
		if(KEY0 == SPRING_UP)
		{
			R_LED(OFF);
		}
	}
		
	if(KEY1 == PUSH_DOWN)
	{
		HAL_Delay(8);
		if(KEY1 == PUSH_DOWN)
		{
			G_LED(ON);
		}
	}
	else if(KEY1 == SPRING_UP)
	{
		HAL_Delay(8);
		if(KEY1 == SPRING_UP)
		{
			G_LED(OFF);
		}
	}
}
