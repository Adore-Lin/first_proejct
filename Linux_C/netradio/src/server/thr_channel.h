#ifndef THR_CHANNEL_H
#define THR_CHANNEL_H

#include "medialib.h"


int thr_channel_create(struect melib_listentry_st *ptr);
int thr_channel_destory(struct mlib_listentry_st *ptr);
int thr_channel_destroyall(void);



#endif /*THR_CHANNEL_H*/