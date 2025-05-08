#ifndef PROTO_H__
#define PROTO_H__

#include "site_type.h"

#define DEFAULT_MGROUP  "224.2.2.2"   //网络多播地址
#define DEFAULT_REVPORT  "1989"       //接收端口
#define DEFAULT_CNTLPORT_LOCAL   "2001"
#define DEFAULT_CNTLPORT_SERVER  "2022"

#define CHNNR       100   //chnnr

#define LISTCHNID   0

#define MINCHNID    1
#define MAXCHNID    (MINCHNID + CHNNR-1)

#define MSG_CHANNEL_MAX     (65536 - 20 -8)
#define MAG_DATA            (MSG_CHANNEL_MAX - sizeof(chnid_t))


//每一个channel数据包的内容
struct msg_channel_st
{
    chnid_t chnid;
    uint8_t data[1];
}__attribute__((packed)); //表示字节不对齐

//描述每一个channel
struct msg_listentry_st
{
    chnid_t chnid;
    uint16_t len;
    char desc[1];
}__attribute__((packed));

//每一个channel信息
struct msg_list_st
{
    chnid_t chnid;
    struct msg_listentry_st entry[1];
}__attribute__((packed));

struct msg_cntl_st
{
    uint16_t type;
    chnid_t chnid;
}__attribute__((packed));



#endif //PROTO_H__
