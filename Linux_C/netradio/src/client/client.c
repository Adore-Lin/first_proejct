#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <syslog.h>
#include <signal.h>      
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <net/if.h>

// #include "../include/proto.h"
// #include "server_conf.h"
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<getopt.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>
#include<arpa/inet.h>
#include<net/if.h>
#include<sys/file.h>
#include<fcntl.h>
#include<linux/input.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>
#include<sys/types.h>
#include<pthread.h>
#include<sys/wait.h>
#include<linux/input.h>
#include <sys/types.h>
#include<time.h>

#include "client.h"
//#include "proto.h"
#include "../include/proto.h"

/* mp3 缓冲区定义
*/
#define BUF_MP3   "/home/chh/temp/buf_mp3"
#define BUF_MP3_LEN  (1024 * 1024 * 16)
#define PIPEBLOCK   512 

static int pd[2];
static pid_t pid;
static pthread_t tid_read;//读文件缓冲线程
static int buf_fd;
static off_t pos_write = SEEK_SET;//缓冲文件的写位置
static off_t pos_read  = SEEK_SET;//缓冲文件的读位置

static pthread_t tid_player_cntl;//控制信息发送线程
static int sdlocal_cntl;
static struct sockaddr_in addrlocal, addrserver;
static socklen_t addrlocal_len, addrserver_len;
static struct msg_cntl_st msg_cntl;
static int chosenid;
static  int total_list = 3;

//线程并发
static pthread_mutex_t bufmp3_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cont_t cond_readmp3buf = PTHREAD_COND_INITIALIZER;
pthread_cont_t cond_writemp3buf = PTHREAD_COND_INITIALIZER;
static int buf_rdwr = 1;

//默认值
struct client_conf_st client_conf = {
    .rcvport    = DEFAULT_REVPORT,
    .mgroup     = DEFAULT_MGROUP,
    .cntlport   = DEFAULT_CNTLPORT_LOCAL,
    .player_cmd = DEFAULT_PLAYERCMD};



void print_help()
{
    printf("-M --multigroup 指定多播组 \n");
    printf("-P --port       指定接收端口\n");
    printf("-p --player     指定播放器\n");
    printf("-H --help       显示帮助\n");
    printf("KEY_UP          上一首\n");
    printf("KEY_DOWN        下一首\n");
    printf("KEY_LEFT        回退\n");
    printf("KEY_RIGHT       快进\n");
    printf("KEY_SPACE       暂停\n");
    printf("KEY_F           一个频道\n");
    printf("KEY_N           下一个频道\n");
    printf("KEY_H           打印帮助 \n");
    printf("KEY_Q           退出 \n");
    printf("KEY_ENTER       播放 \n");
}


int main(int argc, char **argv)
{ 
    int c;
    int index = 0;
    struct option argarr[] = {{"port",   1, NULL, 'P'},
                              {"mgroup", 1, NULL, 'M'},
                              {"player", 1, NULL, 'p'},
                              {"help",   1, NULL, 'H'},
                              {"NULL",   0, NULL,  0 }};

    int sd;
    struct ip_mreqn mreq;
    struct sockaddr_in laddr;
    int val;

    int pd[2];//管道有两端：读端和写端


/*1、从命令行上获取输入的指令*/

    /*初始化级别： 默认值， 配置文件，环境变量，命令行参数*/
    while(1)
    {
        c = getopt_long(argc, argv, "P:M:p:H", argarr, &index);

        if(c < 0)
            break;

        switch (c)
        {
            case 'P':
                client_conf.rcvport = optarg;
                break;

            case 'M':
                client_conf.mgroup = optarg;
                break;

            case 'p':
                client_conf.player_cmd = optarg;
                break;

            case 'H':
                print_help();
                break;
            
            default:
                abort();
                break;
        }
    }

/*2、搭建socket网络通信，使用UDP，从网络上接收数据*/
    //数据收发套接字
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0)
    {
        perror("socket()");
        exit(1);
    }

    /*对结构体mreq 成员进行赋值*/
    //inet_pton(int __af, const char *restrict __cp, void *restrict __buf)
    inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
    /*if error*/

    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);
    /*if error*/

    mreq.imr_ifindex = if_nametoindex("eth0");
    /*if error*/

    //添加多播地址，设置socket属性
    if(setsockopt(sd,IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
    {
        perror("setsockopt()");
        exit(1);
    }

    val = 1;
    if(setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &val, sizeof(val)))
    {
        perror("setsockopt()");
        exit(0);
    }

    //绑定本地地址
    laddr.sin_family = AF_INET;
    laddr.sin_port = htons(atoi(client_conf.rcvport));
    inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr.s_addr);
    if(bind(sd, (void *)&laddr, sizeof(laddr))  < 0)
    {
        perror("bind()");
        exit(1);
    }

/*3、管道，父子进程的操作*/

    //开辟一个管道``
    if(pipe(pd) < 0)
    {
        perror("pipe()");
        exit(1);
    }

    //创建缓冲文件
    oldmask = umask(0);
    buf_fd = open(BUF_MP3, O_RDWR | O_CREAT, 0777);
    umask(oldmask);

    //创建子进程
    pid = fork();
    if(pid < 0)
    {
        perror("fork()");
        exit(1);
    }
    else if(pid == 0)
    {
        //子进程：调用解码器
        close(sd);//关闭socket
        close(pd[1]);//关闭写端
        dup2(pd[0], STDIN_FILENO);
        if(pd[0] > STDIN_FILENO)
        {
            close(pd[0]);
        }

        printf("开始调用MPG123\n");
        execl("/bin/sh", "sh", "-c", client_conf.player_cmd, NULL);
        perror("execl()");
        exit(1);
    }

    //父进程：从网络上收包，发送给子进程
    //收节目单--选择频道--收频道包，发送给子进程
    struct msg_list_st *msg_list;

    msg_list = malloc(MSG_LIST_MAX);
    if(msg_list == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    addrserver_len = sizeof(struct sockaddr_in);
    while(1)
    {
        recv_len = recvfrom(sd, &total_list, sizeof(total_list), 0, (void *)&addrserver, &addrserver_len);
        if(recv_len < sizeof(int))
        {
            fprintf(stderr, "message is too small.\n");
            continue;
        }

        recv_len = recvfrom(sd, msg_list, MSG_LIST_MAX, 0, (void *)&addrserver, &addrserver_len);
        if(recv_len < sizeof(struct msg_list_st))
        {
            fprintf(stderr, "message is too small.\n");
            continue;
        }

        if(msg_list->chnid != LISTCHNID)
        {
            fprintf(stderr, "chnid is not match.\n");
            continue;
        }

        break;
    }

    //初始化控制端套接字
    sdlocal_cntl_init(addrserver);

    //打印节目单并选择频道
    struct msg_listentry_st *pos;
    printf("the number of channel is %d\n", total_list);

    for(pos = msg_list->entry; (char *)pos < ((char *)msg_list) + recv_len; pos = (void *)(((char *)pos) + ntohs(pos->len)))
    {
        printf("channel %d: %s\n", pos->chnid, pos->desc);
    }
    free(msg_list);

    while(scanf("%d", &chosenid) != 1)
    {
        exit(1);
    }
    /*视频最后一节：从这里开始*/
    msg_cntl.types = KEY_ENTER;
    msg_cntl.chnid = chosenid;
    msg_cntl_send(msg_cntl);

    //打开键盘监控
    if((err = pthread_create(&tid_player_cntl, NULL, player_cntl, NULL)) < 0)
    {
        fprintf(stderr, "thread of player_cntl create failed\n");
    }

    //收频道包，发送给子进程
    struct msg_channel_st *msg_channel;
    msg_channel = malloc(MAG_CHANNEL_MAX);
    if(msg_channel == NULL)
    {
        perror("malloc()");
        exit(1);
    }

    remoteaddr_len = sizeof(struct sockaddr_in);
    if((err = pthread_create(&tid_read, NULL, bufmp3_read, pd)) < 0)
    {
        fprintf(stderr, "pthread_create():%s.\n", strerror(err));
    }

    while(1)
    {
        recv_len = recvfrom(sd, msg_channel, MSG_CHANNEL_MAX, 0, (coid *)&remoteaddr, &remoteaddr_len);
        if(recv_len == 0 || remoteaddr.sin_addr.s_addr != addrserver.sin_addr.s_addr 
            || remoteaddr.sin_port.port != addrserver.sin_port.port)
        {
            //当读到不符合的数据包时recvfrom返回0，msgchannel 不发生改变
            fprintf(stderr, "ignore:address or data not match,\n");
            continue;
        }

        pthread_mutex_lock(&bufmp3_lock);
        while(buf_rdwr == 0)
        {
            pthread_cond_wait(&cond_writemp3buf, &bufmp3_lock);
        }
        buf_rdwr = 0;
        if(recv_len < sizeof(struct msg_channel_st))
        {
            if(recv_len < 0)
            {
                fprintf(stderr, "read from socket failed\n");
            }

            fprintf(stderr, "ignore: message is too small\n");
            pthread_cond_signal(&cond_readmp3buf);
            pthread_mutex_unlock(&bufmp3_lock);
            continue;
        }

        if(msg_channel->chnid == chosenid && recv_len - sizeof(chnid_t) > 0)
        {

            if(pos_write >= pos_read?pos_write - pos_read : pos_write + BUF_MP3_LEN - pos_read 
                < BUF_MP3_LEN / 16 *15)
            {
                //printf
                if(write(buf_fd, msg_channel->data, recv_len - sizeof(chnid_t) < 0))
                {
                    exit(1);
                }
                //printf
            }
            else
            {
                msg_cntl.type = KEY_SPACE;
                msg_cntl.chnid = chosenid;
                msg_cntl_send(msg_cntl);
            }

            buf_rdwr = 1;
            pthread_cond_signal(&cond_readmp3buf);
            pthread_mutex_unlock(&bufmp3_lock);
        }

        free(msg_channel);
        free(msg_list);
        close(buf_fd);
        close(sd);
        close(sdlocal_cntl);

        pthread_mutex_destory(&bufmp3_lock);
        pthread_cond_destory(&cond_readmp3buf);
        pthread_cond_signal(&cond_writemp3buf);

        if(-remove(BUF_MP3))
        {
            perror("remove():");
        }

        kill(pid, SIGKILL);
        wait(NULL);
        printf("已经杀死了子进程\n");
        exit(0);
    }







 








    exit(0);
}