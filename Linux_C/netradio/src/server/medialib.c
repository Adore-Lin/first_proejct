#include "medialib.h"
#include "server_conf.h"

#define PATHSIZE         1024
#define LINEBUFSIZE      1024

struct channel_context_st channel[MAXCHNID + 1];


/*初始化媒体库的信息
* 作用：作用是扫描指定目录下的媒体文件，
        并将这些媒体文件的信息存储到一个动态分配的数组中返回给调用者。
*/
int mlib_getchnlist(struct mlib_listentry_st **result, int *resnum)
{
    //1、初始化频道ID
    int i;

    for(i = 0; i < MAXCHNID + 1; i ++)
    {
        channel[i].chnid = -1;  //将所有可能的频道ID设置为 -1，表示这些频道初始时未被使用
    }

    //2、构建搜索路径并执行搜索
    char path[PATHSIZE];
    glob_t globres;

    snprintf(path, PATHSIZE, "%s/*", server_conf.media_dir);
    if(glob(path, 0, NULL, &globres) != 0)
    {
        return -1;
    }

    //3、分配内存以存储结果
    struct mlib_listentry_st *ptr;

    ptr = malloc(sizeof(struct mlib_listentry_st) * globres.gl_pathc);
    if(ptr == NULL)
    {
        syslog(LOG_ERR, "malloc failed.");
        exit(1);
    }

    //4、处理每个找到的文件
    int num = 0;
    struct channel_context_st *res;

    for(i = 0; i < globres.gl_pathc; i++)
    {
        //对于每一个找到的文件，通过 path2entry() 函数将其转换为频道上下文信息
        res = path2entry(globres.gl_pathv[i]); 
        if(res != NULL)
        {
            //如果成功，则复制相关信息到 channel 数组和 ptr 数组中，并增加计数器。
            memcpy(channel + res->chnid, res, sizeof(*res));
            ptr[num].chnid = res->chnid;
            ptr[num].desc  = strdup(res->desc);
            num ++;
        }
    }
    //5、调整分配的内存大小并返回结果
    *result = realloc(ptr, sizeof(struct mlib_listentry_st) * num);
    if(result == NULL)
    {
        syslog(LOG_ERR, "realloc failed,");
    }
    *resnum = num;

    return 0;
}