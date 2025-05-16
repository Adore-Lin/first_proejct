#ifndef __DRIVER_LED_H
#define __DRIVER_LED_H

#include "stm32f1xx_hal.h"

//引脚宏定义：LED引脚、端口
#define R_LED_GPIO_PIN      GPIO_PIN_5 //DS0 红灯
#define R_LED_GPIO_PORT	   	GPIOB
#define R_LED_GPIO_CLK_EN()  __HAL_RCC_GPIOB_CLK_ENABLE()

#define G_LED_GPIO_PIN      GPIO_PIN_5 //DS1 绿灯
#define G_LED_GPIO_PORT	   	GPIOE
#define G_LED_GPIO_CLK_EN()  __HAL_RCC_GPIOE_CLK_ENABLE()


//函数宏定义：LED亮灭函数
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

#define R_LED(flag)  HAL_GPIO_WritePin(R_LED_GPIO_PORT, R_LED_GPIO_PIN, flag);
#define G_LED(flag)  HAL_GPIO_WritePin(G_LED_GPIO_PORT, G_LED_GPIO_PIN, flag);

void LedInit(void);

void LedTest(void);


#endif /*__DRIVER_LED_H */
