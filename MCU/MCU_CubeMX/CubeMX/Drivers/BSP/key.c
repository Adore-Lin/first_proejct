#include "key.h"
#include "../SYSTEM/delay/delay.h"

/**
* @brief 按键扫描函数
* @note 该函数有响应优先级(同时按下多个按键): WK_UP > KEY1 > KEY0!!
* @param mode:0 / 1, 具体含义如下:
* @arg 0, 不支持连续按(当按键按下不放时, 只有第一次调用会返回键值,
*       必须松开以后, 再次按下才会返回其他键值)
* @arg 1, 支持连续按(当按键按下不放时, 每次调用该函数都会返回键值)
* @retval 键值, 定义如下:
* KEY0_PRES, 1, KEY0 按下
* KEY1_PRES, 2, KEY1 按下
* WKUP_PRES, 3, WKUP 按下
*/
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1;
    uint8_t keyval = 0;

    if(mode == 1)
    {
        key_up = 1;
    }

    if(key_up && (KEY0 == 0 || KEY1 == 0 || WK_UP == 1))
    {
        delay_ms(10);
        key_up = 0;

        if(KEY0 == 0)
            keyval = KEY0_PRESS;
        
        if(KEY1 == 0)
            keyval = KEY1_PRESS;

        if(WK_UP == 1)
            keyval = WKUP_PRESS;
    }
    else if(KEY0 == 1 && KEY1 == 1 && WK_UP == 0)
    {
        key_up = 1;
    }

    return keyval;
}
