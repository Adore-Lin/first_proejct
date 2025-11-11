#include "led.h"

uint8_t g_green_flag = 0;
uint8_t g_red_flag = 0;


//注：翻转太快，效果难以观察
void Green_LED_Toggle()
{
    delay_ms(5000);
	g_green_flag = HAL_GPIO_ReadPin(Green_LED_GPIO_Port, Green_LED_Pin);
    if(g_green_flag == GPIO_PIN_RESET)
    {
        HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);
//        delay_ms(500);
    }
    else
    {
        HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_RESET);
//        delay_ms(500);
    }
}

void Red_LED_Toggle()
{
     delay_ms(5000);
	g_red_flag = HAL_GPIO_ReadPin(Red_LED_GPIO_Port, Red_LED_Pin);
     if(g_red_flag == GPIO_PIN_RESET)
     {
         HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);
 //        delay_ms(5000);
     }
     else
     {
         HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
 //        delay_ms(5000);
     }

//    do
//    { 
//        HAL_GPIO_TogglePin(Red_LED_GPIO_Port, Red_LED_Pin); 
//    }while(0);
}
