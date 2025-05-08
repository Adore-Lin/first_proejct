#ifndef SERVER_CONF_H__
#define SERVER_CONF_H__

#define DEFAULT_MEDIADIR     "/home/chh/media"  //默认媒体库
#define DEFAULT_IF           "eth0"      //默认网卡

enum 
{
    RUN_DAEMON = 1,
    RUN_FOREGROUD = 2
};

struct server_conf_st
{
    char *rcvport;
    char *mgroup;
    char *media_dir;
    char runmode;
    char *ifname;
    char *cntlport;
};

extern struct server_conf_st server_conf;



#endif /*SERVER_CONF_H__*/