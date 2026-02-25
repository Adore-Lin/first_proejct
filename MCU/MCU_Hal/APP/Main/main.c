/*  2025-01-01：裸机MCU
 * 
 * 文件名称：main.c
 * 摘要：
 *  
 *  驱动：通过修改韦东山HAL库教程代码，集成项目
 *  业务：模仿50
 *--------------------------------------------------
*/
#include "main.h"
#include "driver_led.h"
#include "driver_key.h"


uint32_t sys_time = 0;

/*硬件初始化
* 1、系统时钟
* 2、按键、LED
* 3、串口调试
* 4、IAP升级

* 3、显示屏LCD
* 4、FLASH、EEPROM
* 5、定时器
* 6、RTC
* 7、看门狗
*/
void HardwareInit(void)
{
	LedInit();
    KeyInit();
}

/*驱动测试*/
void DriverTest(void)
{
	//LedTest();
    KeyTest();
//    UsartInit(115200);
}

int main(void)
{    
    //uint32_t sys_clk = 0;
    // 初始化HAL库函数必须要调用此函数
    HAL_Init();

    /*
     * 系统时钟即AHB/APB时钟配置
     * 当使用内部高速时钟HSE（8MHz）配置系统时钟时，使用PLL前可以选择不分频或者二分频，我们要配置到最大72MHz的系统频率此处当然是不分频
     * 然后经过一个放大器，最大放大倍数为16，我们要想得到72MHz，此处选择9倍放大系数，即8*9=72MHz即可达到目标
    */
    SystemClock_Config();
    
	//硬件初始化
	HardwareInit();
	
	SystemClock_Config();
	sys_time = HAL_RCC_GetSysClockFreq();

	
    while(1)
    {
		DriverTest();
	}
}

/*
 *  函数名：void Error_Handler(void)
 *  输入参数：无
 *  输出参数：无
 *  返回值：无
 *  函数作用：程序错误处理函数，此处暂时设为死循环，不做任何动作
*/
void Error_Handler(void)
{
    while(1)
    {
    }
}
