#ifndef _MEDIALIB__H
#define _MEDIALIB__H

#include "../include/proto.h"
#include <glob.h>
#include <unistd.h>

struct mlib_listentry_st
{
   chnid_t chnid;
   char *desc;
};

struct channel_context_st
{
    chnid_t chnid;
    char *desc;
    glob_t mp3glob;
    int pos;
    int fd;
    off_t offset;
    mytbf_t *tbf;
};



int mlib_getchnlist(struct mlib_listentry_st **result, int *resnum);




#endif /*_MEDIALIB__H*/