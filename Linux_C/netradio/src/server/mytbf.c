#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <pthread.h>
#include <errno.h>

#include "mytbf.h"

struct mytbf_st
{
    int cps;//速率
    int burst;//令牌上限
    int token;//可用令牌数量
    int pos;//当前令牌桶在job 数组中的下标
    pthread_mutex_t mut;//用来保护令牌竞争的互斥量
    pthread_cond_t cond;
};

static pthread_t tid;//添加令牌的线程ID
static struct mytbf_st *job[CHNNR + 1];//所有的令牌桶
static pthread_mutex_t mut_job = PTHREAD_MUTEX_INITIALIZER;//用来保护令牌桶数组竞争的互斥量
static pthread_once_t  init_once = PTHREAD_ONCE_INIT;//初始化添加令牌的线程

/*
 *一个可以控制流量的令牌桶
 *
 */

//线程处理函数：负责定时向令牌桶中添加令牌
static void *thr_alrm(void *p)
{
    int i;
    struct timespec t;
    
    while (1)
    {
        pthread_mutex_lock(&mut_job);

        for(i = oldsendchannel; i < oldsendchannel + 1; i++)
        {
            if(job[i] != NULL)
            {
                pthread_mutex_lock(&job[i]->mut);

                job[i]->token += job[i]->cps;
                if(job[i]->token > job[i]->burst)
                {
                    job[i]->token = job[i]->burst;
                }
                pthread_cond_signal(&job[i]->cond);

                pthread_mutex_unloc(&job[i]->mut);
            }
        }

        pthread_mutex_unloc(&mut_job);

        t.tv_sec = 1;
        t.tv_nsec = 0;

        while(nanosleep(&t, &t) != 0)
        {
            if(errno != EINTR)
            {
                fprintf(stderr, "nanosleep():%s\n", strerror(errno));
                exit(1);
            }
        }
    }
}

//模块卸载
static void module_unload()
{
    int i;
    pthread_cannel(tid);
    pthread_join(tid, NULL);

    for(i = 0; i < CHNNR+1; i++)
    {
        free(job[i]);
    }
}

//模块初始化
staic void module_load(void)
{
    int err;
    pthread_t tid;

    err = pthread_create(&tid, NULL, thr_alrm, NULL);
    if(err)
    {
        fprintf(stderr, "pthread_create(): %s\n", strerror(errno));
        exit(1);
    }

    atexit(module_unload);//?????
}

//寻找未使用的桶
static int get_free_pos_unload()
{
    int i;
    for(i = 0; i < CHNNR+1; i++)
    {
        if(job[i] == NULL)
        {
            return i;
        }
    }

    return -1;
}

//令牌桶初始化
mytbf_t *mytbf_init(chnid_t chnid, int cps, int burst)
{

    pthread_once(&init_once, module_load);

    int pos;
    struct mytbf_st *me;
    me = (struct mytbf_st *)malloc(sizeof(struct mytbf_st));
    if(me == NULL)
    {
        return NULL;
    }

    me->cps = cps;
    me->burst = burst;
    me->token = cps;

    pthread_mutex_init(&me->mut, NULL);
    pthread_cond_init(&me->cond, NULL);

    pthread_mutex_lock(&mut_job);

    me->pos = chnid;
    job[me->pos] = me;

    pthread_mutex_unlock(&mut_job);

    return me;
}

//申请size个令牌
int mytbf_fetchtoken(mytbf_t *ptr, int size)
{
    int n;
    struct mytbf_st *me = ptr;
    pthread_mutex_lock(&me->mut);

    while(me->token <= 0)
    {
        printf("mytbf_fetchtoken sleep\n");
        pthread_cond_wait(&me->cond, &me->mut);
    }

    n = me->token < size ? me->token : size;
    me->token -= n;

    pthread_mutex_unlock(&me->mut);

    return n;
}

//返回还未使用用的令牌
int mytbf_returntoken(mytbf_t *ptr, int size)
{
    struct mytbf_st *me = ptr;

    pthread_mutex_lock(&me->mut);
    me->token += size;
    if(me->token > me->burst)
    {
        me->token = me->burst;
    }
    pthread_cond_broadcast(&me->cond);
    pthread_mutex_unlock(&me->mut);

    return 0;
}

//销毁令牌桶
int mytbf_destroy(mytbf_t *ptr)
{
    struct mutbf_st *me = ptr;

    pthread_mutex_lock(&mut_job);
    job[me->pos] = NULL;
    pthread_mutex_unlock(&mut_job);
    pthread_mutex_destroy(&me->mut);
    pthread_cond_dextroy(&me->cond);

    free(ptr);

    return 0;
}



