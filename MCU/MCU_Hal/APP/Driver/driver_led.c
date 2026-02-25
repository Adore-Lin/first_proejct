
#include "driver_led.h"

void LedInit(void)
{
	//定义GPIO的结构体变量
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	//使能LED的GPIO 对应的时钟
	R_LED_GPIO_CLK_EN();
	G_LED_GPIO_CLK_EN();
	
	GPIO_InitStruct.Pin = R_LED_GPIO_PIN | G_LED_GPIO_PIN; // 选择 LED 的引脚
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // 设置为推挽输出模式
	GPIO_InitStruct.Pull = GPIO_PULLUP; // 默认上拉
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;// 引脚反转速度设置为快
	
	//初始化引脚配置
	HAL_GPIO_Init(R_LED_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_Init(G_LED_GPIO_PORT, &GPIO_InitStruct);
	
	R_LED(OFF);
	G_LED(OFF);
}

/*Led测试：红灯亮灭，接着绿灯亮灭*/
void LedTest(void)
{
	R_LED(ON);
	HAL_Delay(1000);
	R_LED(OFF);
	
	G_LED(ON);
	HAL_Delay(1000);
	G_LED(OFF);
}
