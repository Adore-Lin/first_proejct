#ifndef __DRIVER_KEY_H
#define __DRIVER_KEY_H

#include "stm32f1xx_hal.h"

//引脚宏定义：引脚、端口
#define KEY0_GPIO_PIN      GPIO_PIN_4
#define KEY0_GPIO_PORT	   GPIOE
#define KEY0_GPIO_CLK_EN()  __HAL_RCC_GPIOE_CLK_ENABLE()

#define KEY1_GPIO_PIN      GPIO_PIN_3
#define KEY1_GPIO_PORT	   GPIOE
#define KEY1_GPIO_CLK_EN()  __HAL_RCC_GPIOE_CLK_ENABLE()

//引脚状态宏定义：
#define PUSH_DOWN  GPIO_PIN_RESET
#define SPRING_UP  GPIO_PIN_SET

/*按键状态读取函数宏定义*/
#define KEY0  HAL_GPIO_ReadPin(KEY0_GPIO_PORT, KEY0_GPIO_PIN)
#define KEY1  HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN)

void KeyInit(void);
void KeyTest(void);

#endif /*__DRIVER_KEY_H */
