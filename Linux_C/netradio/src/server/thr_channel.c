#include "thr_channel.h"




#include "../include/proto.h"
#include <pthread.h>


//每一个线程负责一个频道
struct thr_channel_ent_st
{
    chnid_t chnid;
    pthread_t tid;
}

struct thr_channel_ent_st thr_channel[CHNNR];
static int tid_nextpos = 0;


//对应频道的线程的处理函数
static void *thr_channel_snder(void *ptr)
{
    
}

//创建每个频道的发送线程
/*传入一个媒体库结构体指针，然后将指向的数据缓存到thr_channel数组中

*/
int thr_channel_create(struect melib_listentry_st *ptr)
{
    int err;
    if(tid_nextpos >= CHNNR)
    {
        return -1;
    }

    err = pthread_create(&thr_channel[tid_nextpos].tid, thr_channel_snder, ptr);
    if(err)
    {
        return -1;
    }

    thr_channel[tid_nextpos].chnid = ptr->chnid;
    tid_nextpos ++;

    return 0;
}

//销毁某一个频道线程
int thr_channel_destory(struct mlib_listentry_st *ptr)
{
    int i;

    for(i = 0; i < CHNNR; i ++)
    {
        if(thr_channel[i].chnid == ptr->chnid)
        {
            if(pthread_cannel(thr_channel[i].tid) < 0)
            {
                return -1;
            }

            pthread_join(thr_channel[i].tid, NULL);
            thr_channel[i].chnid = -1;

            return 0;
        }
    }

    return -1;
}

//销毁所有频道线程
int thr_channel_destroyall(void)
{
    int i;

    for(i = 0; i < CHNNR; i ++)
    {
        if(thr_channel[i].chnid > 0)
        {
            if(thr_channel[i].tid < 0)
            {
                return -1;
            }

            pthread_join(thr_channel[i].tid, NULL);
            thr_channel[i].chnid = -1;
        }
    }

    return 0;
}
