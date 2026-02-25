#ifndef __IAP_H
#define __IAP_H

#include "../../SYSTEM/sys/sys.h"

/*定义一个函数类型的参数*/
typedef void (*iap_fun)(void);

/*内部FLASH划分：
    BootLoader 0x0800000~0x800FFF(36KB)
    App1地址从 0x08010000开始
*/
#define FALSH_APP1_ADDR  0x08010000

/*跳转到APP程序执行*/
void iap_load_app(uint32_t app_addr);
/*在指定地址开始，写入bin*/
void iap_write_appbin(uint32_t app_addr, uint8_t *app_buf, uint32_t app_len);


#endif /* __IAP_H */