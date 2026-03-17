#include <stdio.h>
#include "circle_buffer.h"

/*初始化：
* r: 下一个要读的位置； w: 下一个要写的位置
* r、w 初始化都为0
*/
void circle_buffer_init(p_circle_buf pCircleBuf, uint32_t len, uint8_t *buf)
{
   pCircleBuf->r = pCircleBuf->w = 0;
   pCircleBuf->len = len;
   pCircleBuf->buf = buf;
}

/*读数据：
* 从buffer[r]读出数据，然后调整 r 指向下一个位置；当 r 越界后要从0开始
* 判断 buffer 为空，r = w 表示空
*/
int circle_buffer_read(p_circle_buf pCircleBuf, uint8_t *pval)
{
    if(pCircleBuf->r != pCircleBuf->w)
    {
        *pval = pCircleBuf->buf[pCircleBuf->r];
        pCircleBuf->r++;

        if(pCircleBuf->r == pCircleBuf->len)
        {
            pCircleBuf->r = 0;
        }

        return 0;
    }
    else
    {
        return -1;
    }

}

// 批量读取环形缓冲区数据
int circle_buffer_read_multi(p_circle_buf pCircleBuf, uint8_t *pbuf, uint32_t len)
{
    uint32_t i = 0;
    while (i < len && pCircleBuf->r != pCircleBuf->w)
    {
        pbuf[i] = pCircleBuf->buf[pCircleBuf->r];
        pCircleBuf->r++;
        
        if(pCircleBuf->r == pCircleBuf->len)
        {
            pCircleBuf->r = 0;
        }
        
        i++;
    }
    return i; // 返回实际读取的字节数
}


/*写入数据： 
* 把数据写入buffer[w]，然后调整 w 指向下一个位置；当 w 越界后要从0开始
* 判断 buffer 为满，“下一个写入的位置” 等于当前读位置
*/
int circle_buffer_write(p_circle_buf pCircleBuf, uint8_t val)
{
    uint32_t next_w;

    next_w = pCircleBuf->w + 1;
    if(next_w == pCircleBuf->len)
    {
        next_w = 0;
    }

    if(next_w != pCircleBuf->r)
    {
        pCircleBuf->buf[pCircleBuf->w] = val;
        pCircleBuf->w = next_w;

        return 0;
    }
    else
    {
        return -1;
    }
}

