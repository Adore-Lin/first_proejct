#ifndef BSP_KEY_H
#define BSP_KEY_H

#include "main.h"

//读引脚
#define KEY0    HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)
#define KEY1    HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)
#define WK_UP   HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin)

//按下对应的值
#define KEY0_PRESS  1
#define KEY1_PRESS  2
#define WKUP_PRESS  3

//按键扫描函数
uint8_t key_scan(uint8_t mode);

#endif /* BSP_KEY_H */
