#ifndef CLIENT_H__
#define CLIENT_H__

//暂时这么写，本地不一定有这个mpg123
#define DEFAULT_PLAYERCMD  "usr/bin/mpg123 -> /dev/null"

struct client_conf_st
{
    char *rcvport;
    char *mgroup;
    char *player_cmd;
    char *cntlport;
};

extern struct client_conf_st client_conf;

#endif // !CLIENT_H__

