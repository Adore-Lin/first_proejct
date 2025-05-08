#include "thr_msgcntl.h"
#include <pthread.h>

//全局变量
int sdlocal_cntl;//控制端口变量
struct sockaddr_in addrlocl, addrclient;
socklen_t addrlocal_len, addrclient_len;
int lisentsd;







//控制端口初始化
void sdlocal_cntl_init()
{
    sdlocal_cntl = 0;

    /*使用 socket() 创建一个新的套接字，
    AF_INET:表示这是一个IPV4协议族的套接字，
    SOCK_STREAM: 指定了使用TCP协议提供可靠、面向连接的服务
    0： 最后一个参数通常设为0，表示使用默认协议，在这个情况下是TCP */
    lisentsd = socket(AF_INET, SOCK_STREAM, 0);
    if(lisentsd < 0)
    {
        perror("socket(AF_INET, SOCK_STREAM, 0):");
    }

    /*设置本地地址结构
    1、设置地址族：AF_INET（IPV4）
    2、使用 inet_pton() 将IP地址 "0.0.0.0" 转换为二进制形式并存储在结构体中
    （这表示服务器将监听所有网络接口上的连接）
    3、将配置文件中定义的端口号转换为网络字节序并赋值给 sin_port 字段。*/
    addrlocl.sin_family = AF_INET;
    inet_pton(AF_INET, "0.0.0.0", &addrlocl.sin_addr.s_addr);
    addrlocl.sin_port = htons(atoi(server_conf.cntlport));
    addrlocl_len = sizeof(addrlocl);

    /*绑定套接字到地址和端口
    1、bind() 函数将之前创建的套接字与特定的IP地址和端口关联起来*/
    if(bind(lisentsd, (struct sockaddr *)&addrlocl, addrlocl_len) < 0)
    {
        perror("bind(sd, (struct sockaddr *)&addrlocl, addrlocl_len):");
    }

    /*开始监听连接请求
    1、listen() 函数使服务器开始监听该套接字上的连接请求
    2、SOMAXCONN 是系统允许的最大连接队列长度*/
    if(listen(listensd, SOMAXCONN) < 0)
    {
        perror("listen(sd, SOMAXCONN):");
    }
}

void *msg_cntl_listen(void *ptr)//接受控制的信息
{
    sdlocal_cntl = 0;
    sdlocal_cntl = accept(listentsd, (struct sockaddr *)&addrclient, &addrclient_len);

    char address[30];
    inet_ntop(AF_INET, &addrclient.sin_addr.s_addr, address,  30);
    printf("addrclinet: %s sdlocal_cntl:%d\n", address, sdlocal_cntl);
    if(sdlocal_cntl < 0)
    {
        perror("accept():");
    }

    printf("addrclient :%d, sdlocal_cntl:%d \n", ntohs(addrclient.sin_addr.s_addr), sdlocal_cntl);

    if(sdlocal_cntl)
    {
        printf("clent and client connect successd\n");
    }

    int recv_len = 0;
    struct msg_cntl_st msg_cntl;
    while (1)
    {
        printf("recv_len in msg_cntl()\n");
        recv_len = recvfrom(sdloacl_cntl, &msg_cntl, sizeof(struct msg_cntl_st), MSG_WAITALL, NULL, NULL);

        if(recv_len < 0)
        {
            perror("recv_len in msg_cntl()");
            break;
        }

        if(recv_len ==  sizeof(struct msg_cntl_st))
        {
            printf("recieved message is : KEY_CODE %d CHNID %d\n", msg_cntl.type, msg_cntl.chnid);
            msg_cntl_handle(msg_cntl);
        }

    }
    
    pthread_exit(NULL);
}