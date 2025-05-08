#include <stdio.h>

#include <sys/types.h> //socket
#include <sys/socket.h>
#include <netinet/in.h>//ip,ipv4,创建TCP、UDP相关的socket
#include <netinet/ip.h>


#include "../include/proto.h"
#include "server_conf.h"

/**
 * -M 指定多播组
 * -P 指定接受端口
 * -F 前台运行
 * -D 指定媒体库位置
 * -I 指定网络设备
 * -H 显示帮助
 */

static server_conf_st server_conf = 
{
    .rcvport = DEFAULT_REVPORT,
    .mgroup = DEFAULT_MGROUP,
    .media_dir = DEFAULT_MEDIADIR,
    .runmode = RUN_DAEMON,
    .cntlport = DEFAULT_CNTLPORT_SERVER,
    .ifname = DEFAULT_IF
};

int serversd;
struct sockaddr_in sndaddr;
/*struct sockaddr_in 
{
    sa_family_t    sin_family; // address family: AF_INET /
    in_port_t      sin_port;   // port in network byte order /
    struct in_addr sin_addr;   // internet address/
};
*/
struct mlib_listentry_st *list;

static pthread_t tid_msgcntl;

void print_help()
{
    printf("-M 指定多播组\n");
    printf("-P 指定接收端口\n");
    printf("-F 前台运行\n");
    printf("-D 指定媒体库位置\n");
    printf("-I 指定网络设备\n");
    printf("-H 显示帮助\n");
} 

/*创建一个后台守护进程*/
static int daemonize(void)
{
    pid_t pid;
    pid = fork();
    if(pid < 0) //出错
    {
        return -1;
    }

    if(pid > 0)
    {
        exit(0);
    }

    //打开一个空设备
    int fd = open("dev/null", O_RDWR);
    if(fd < 0)//打开失败
    {
        return -2;
    }
    else
    {
        dup2(fd, STDERR_FILENO);//将文件描述符fd指向的文件设置为程序的标准错误输出(stderr)。这意味着任何写入到stderr的数据实际上会写入到fd所指向的文件中。
        dup2(fd, STDIN_FILENO);//将文件描述符fd指向的文件设置为程序的标准输入(stdin)。这意味着任何从stdin读取的数据实际上是从fd所指向的文件中读取的
        dup2(fd, STDOUT_FILENO);//将文件描述符fd指向的文件设置为程序的标准输出(stdout)。这意味着任何写入到stdout的数据实际上会写入到fd所指向的文件中

        if(fd > STDOUT_FILENO)//关闭所有
        {
            close(fd);
        }
    }
    setsid();//

    chdir("/");
    umask(0);

    return 0;
}

/*后台守护进程退出*/
static void daemon_exit(int s)
{
    

}

/*初始化一个UDP socket，特别是为多播（Multicast）通信做准备*/
static void socket_init()
{
    struct ip_mreqn mreq;
    /* man 7 ip
        struct ip_mreqn 
        {
            struct in_addr imr_multiaddr; // IP multicast group  address
            struct in_addr imr_address;   //IP address of local  interface
            int            imr_ifindex;   // interface index 
        };
    */

    //创建Socket：首先创建了一个用于UDP通信的socket。
    serversd = socket(AF_INET, SOCK_DGRAM, 0);
    if(serversd < 0)
    {
        syslog(LOG_ERR, "socket(): %s", strerror(errno));
        exit(1);
    }

    /*设置多播网络接口：通过设置socket选项来指定多播组和网络接口*/
    //1、设置多播相关参数
    inet_pton(AF_INET, server_conf.mgroup, &mreq.imr_multiaddr);//多播组
    inet_pton(AF_INET, "0.0.0.0", &mreq.imr_address);//多播端口
    mreq.imr_ifindex = if_nametoindex(server_conf.ifname);

    //2、配置Socket以使用指定的多播接口
    int ret = setsocket(serversd, IPPROTO_IP, IP_MULTICAST_IT, &mreq, sizeof(mreq))
    if(ret < 0)
    {
        syslog(LOG_ERR, "setsockopt(IP_MULTICAST_IF): %s", strerror(errno));
        exit(1);
    }

    //初始化发送地址结构：设置了发送数据时的目标地址信息。
    sndaddr.sin_family = AF_INET;
    sndaddr.sin_port = htons(atoi(server_conf.rcvport));
    inet_pton(AF_INET, server_conf.mgroup, &sndaddr.sin_addr);
}

int main(int argc, char *argv[])
{


    /*命令行分析*/
    int c;

    while(1)
    {
        c = getopt(argc, argv, "M:P:FD:I:H");
        if(c < 0)
            break;
        
        switch (c)
        {
            case 'M':
                server_conf.mgroup = optarg;
                break;
            case 'P':
                server_conf.rcvpor = optarg;
                break;
            case 'F':
                server_conf.runmode = RUN_FOREGROUD;
                break;
            case 'D':
                server_conf.media_dir = optarg;
                break;
            case 'I':
                server_conf.ifname = optarg;
                break;
            case 'H':
                printf_help();
                exit(1);
                break;
            
            default:
                abort();
                break;
        }
    }

    /*守护进程的实现*/
    if(server_conf.runmode == RUN_DAEMON)//如果命令行输入选择的是后台运行
    {
        if(daemonize() != 0)
        {
            exit(1);
        }
    }
    else if(server_conf.runmode == RUN_FOREGROUD)//选择的前台运行
    {
        //do nothing
    }
    else 
    {
        syslog(LOG_ERR,"EINVAL server_conf.runmode");/*写入系统日志*/
        exit(1);
    }

    /*socket初始化*/
    socket_init();
    sdlocal_cntl_init();

    /*获取频道信息:
    struct mlib_listentry_st *list;
    int list_size;
    int err;
    */
    int list_size;
    int err;
    err = mlib_getchnlist(&list, &list_size);
    if(err)
    {
        syslog(LOG_ERR, "mlib_getchnlist() : %s.", strerror(errno));
        exit(1);
    }
    syslog(LOG_DEBUG, "channel size = %d", list_size);

    /*创建节目单线程*/
    err = thr_list_create(list, list_size);
    if(err)
    {
        exit(1);
    }

    /*创建频道线程: 一个频道创建一个线程*/
    for(i = 0; i < list_size; i ++)
    {
        err = thr_channel_create(list + i);

        if(err)
        {
            sysslog(LOG_ERR, "thr_channel_create(): %s\n", strerror(errno));
            exit(-1);
        }
    }

    syslog(LOG_DEBUG, "%d channel threads create.", i);

    //启动控制端口
    err = pthread_create(&tid_msgcntl, 0, msg_cntl_listen, NULL);
    if(err < 0)
    {
        printf("pthread_create err\n");
        exit(1);
    }

    while(1)
        pause();


    //关闭、销毁资源
    close(serversd);
    close(sdlocal_cntl);

    pthread_mutex_destroy(&sd_lock);
    pthread_mutex_destroy(&channel_lock);

    for(i = 0; i < CHNNR; i ++)
    {
        pthread_cond_destroy(channel_cond + i);
        pthread_cond_destroy(sock_cond + i);
    }

    thr_list_destroy();
    thr_channel_destroyall();

    mlib_freechnlist(list);

    closelog();
    exit(0);
}