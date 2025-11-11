#ifndef BSP_LED_H
#define BSP_LED_H

#include "main.h"
#include "../SYSTEM/sys/sys.h"
#include "../SYSTEM/delay/delay.h"

#define ON    GPIO_PIN_RESET
#define OFF   GPIO_PIN_SET

#define Green_LED_ON()    HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_RESET);  
#define Green_LED_OFF()   HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
// #define Green_LED_Toggle()  do{ HAL_GPIO_TogglePin(Green_LED_GPIO_Port, Green_LED_Pin); }while(0) /* 翻转 LED0 */

#define Red_LED_ON()      HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
#define Red_LED_OFF()     HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
// #define Red_LED_Toggle()  do{ HAL_GPIO_TogglePin(Red_LED_GPIO_Port, Red_LED_Pin); }while(0) /* 翻转 LED0 */

void Green_LED_Toggle();
void Red_LED_Toggle();

#endif /* BSP_LED_H */
