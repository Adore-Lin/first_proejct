#include "iap.h"

iap_fun jumpapp;
uint16_t iap_buff[1024];

void iap_write_appbin(uint32_t app_addr, uint8_t *app_buf, uint32_t app_len)
{
    uint16_t t;
    uint16_t i = 0;
    uint16_t temp;
    uint32_t fw_addr = app_addr;
    uint8_t *fw_buf = app_buf;

    for (t = 0; t < app_len; t += 2)
    {
        temp = (uint16_t *)fw_buf[1] << 8;
        temp |= (uint16_t)fw_buf[0];
        fw_buff += 2;
        iap_buff[i++] = temp;

        if (i == 1024)
        {
            i = 0;
            stmflash_write(fw_addr, iap_buff, 1024);
        }
    }

    if (i)
    {
        stmflash_write(fw_addr, iap_buff, i);
    }
}


void iap_load_app(uint32_t app_addr)
{
    /*检查栈顶地址是否合法，可以放在内部SRAM共64KB(0x20000000)*/
    if (((*(volatile uint32_t *)app_addr) & 0x2FFE0000) == 0x20000000)
    {
        /*用户代码区第二字为程序开始地址（复位地址）*/
        jumpapp = (iap_fun)*(volatile uint32_t *)(app_addr + 4);

        /*初始化APP堆栈指针（用户代码区的第一个字用于存放栈顶地址）*/
        sys_msr_msp(*(volatile uint32_t *)app_addr);

        /*跳转到APP*/
        jumpapp();
    }
}

