#include "exit.h"
#include "main.h"

uint16_t g_GPIO_Pin;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	
//	HAL_Delay(20);
	
	switch(GPIO_Pin)
	{
		case WK_UP_Pin:
			if(HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin))
			  {
				HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_RESET);//ùù
				HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);				  
			  }
			  else
			  {
				  HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);//ùù
				  HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);//ùù
			  }
			 break;
		  
		case KEY1_Pin:
			  if(HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == GPIO_PIN_RESET)
			  {
				  HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_RESET);//ùù
			  }
			  else
			  {
				  HAL_GPIO_WritePin(Green_LED_GPIO_Port, Green_LED_Pin, GPIO_PIN_SET);//ùù
			  }
			 
			  break;
		  
		case KEY0_Pin:
			  if(HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin) == GPIO_PIN_RESET)
			  {
				  HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_RESET);
			   }
			  else
			  {
				  HAL_GPIO_WritePin(Red_LED_GPIO_Port, Red_LED_Pin, GPIO_PIN_SET);//ùù
			  }
			 break;
			   
		default:
			break;
	}
}

