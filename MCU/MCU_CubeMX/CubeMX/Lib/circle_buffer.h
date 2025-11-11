#ifndef _CIRCLE_BUFFER_H
#define _CIRCLE_BUFFER_H

#include <stdint.h>

typedef struct circle_buffer
{
    uint32_t r;
    uint32_t w;
    uint32_t len;
    uint8_t *buf;
}circle_buf, *p_circle_buf;

void circle_buffer_init(p_circle_buf pCircleBuf, uint32_t len, uint8_t *buf);
int circle_buffer_read(p_circle_buf pCircleBuf, uint8_t *pval);
int circle_buffer_write(p_circle_buf pCircleBuf, uint8_t val);

#endif /*_CIRCLE_BUFFER_H*/
