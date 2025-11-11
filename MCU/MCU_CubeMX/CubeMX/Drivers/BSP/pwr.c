#include "main.h"
#include "pwr.h"

void pwr_enter_sleep(void)
{
    HAL_SuspendTick();
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
}

void pwr_enter_stop(void)
{
    __HAL_RCC_PWR_CLK_ENABLE(); /* 使能电源时钟*/

    HAL_SuspendTick();
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

void pwr_enter_standby(void)
{
    __HAL_RCC_PWR_CLK_ENABLE(); /* 使能电源时钟*/

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1); /* 使能KEY_UP引脚的唤醒功能*/
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);  /*需要清此标记， 否则将保持唤醒功能*/
    HAL_PWR_EnterSTANDBYMode(); /*进入待机模式*/
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if(GPIO_Pin == WK_UP_Pin)
    {

    }
}
