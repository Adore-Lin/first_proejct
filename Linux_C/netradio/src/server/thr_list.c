

#include <pthread.h>
#include <errno.h>
#include <syslog.h>


#include "thr_list.h"
#include "medialib.h"
#include "server_conf.h"

static pthread_t tid_list;

//节目单包含的节目数量
static int nr_list_ent;
//节目单信息数组， 每一条存储一个节目频道信息
static struct mlib_listentry_st *list_ent;


//节目单 线程的创建
int thr_list_create(struct mlib_listentry *listp, int nr_ent)
{
    int err;
    list_ent = listp;
    nr_list_ent = nr_ent;

    err = pthread_create(&tid_list, NULL, thr_list, NULL);
    if(err)
    {
        syslog(LOG_ERR, "pthread_create():%s ", strerror(errno));
        return -1;
    }

    return 0;
}

int thr_list_destory(void)
{
    pthread_cancel(tid_list);
    pthread_join(tid_list, NULL);

    return 0;
}