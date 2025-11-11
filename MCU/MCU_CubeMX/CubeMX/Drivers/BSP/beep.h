#ifndef BSP_BEEP_H
#define BSP_BEEP_H

#include "main.h"

#define Beep_ON()     HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_SET);
#define Beep_OFF()    HAL_GPIO_WritePin(Beep_GPIO_Port, Beep_Pin, GPIO_PIN_RESET);


#endif /* BSP_BEEP_H */
