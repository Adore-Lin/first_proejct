/*抽象数据结构体：
* 同时包含操作的函数指针
*  定义一个这样类型的结构体，然后初始化
*/

//结构体
struct soft_time
{
    uint32_t timeout;
    void *args;
    void (*func)(void *);
};

void key_timerout_func(void *args);

struct soft_time key_timer = {~0, NULL, key_timerout_func};

void key_timerout_func(void *args)
{

}