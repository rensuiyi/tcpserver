#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include "udps.h"


#define UDP_BUFFER_SIZE  128
static int get_time_str(char *strbuffer,int len)
{
    struct tm *mytime;
    time_t time_now;

    if (len<24)
    {
        return -1;
    }
    time(&time_now);
    mytime= localtime(&time_now);
    sprintf(strbuffer,"%04d-%02d-%02d %02d:%02d:%02d\n",
            mytime->tm_year+1900,
            mytime->tm_mon+1,
            mytime->tm_mday,
            mytime->tm_hour,
            mytime->tm_min,
            mytime->tm_sec);
    return 0;
}

/**
 * @brief the entry of the udp server
 * 
 * @author renyong (9/20/2012)
 * 
 * @param port 
 * 
 * @return int 
 */
void *udp_server_thread(void *para)
{

    int sock;
//sendto中使用的对方地址
    struct sockaddr_in toAddr;
//在recvfrom中使用的对方主机地址
    struct sockaddr_in fromAddr;
    int recvLen;
    unsigned int addrLen;
    char recvBuffer[128];
    sock = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
    if (sock < 0)
    {
        printf("创建套接字失败了.\r\n");
        exit(0);
    }
    memset(&fromAddr,0,sizeof(fromAddr));
    fromAddr.sin_family=AF_INET;
    fromAddr.sin_addr.s_addr=htonl(INADDR_ANY);
    fromAddr.sin_port = htons(10000);
    if (bind(sock,(struct sockaddr*)&fromAddr,sizeof(fromAddr))<0)
    {
        printf("bind() 函数使用失败了.\r\n");
        close(sock);
        exit(1);
    }
    while (1)
    {
        addrLen = sizeof(toAddr);
        if ((recvLen = recvfrom(sock,recvBuffer,128,0,(struct sockaddr*)&toAddr,&addrLen))<0)
        {
            printf("()recvfrom()函数使用失败了.\r\n");
            close(sock);
            exit(1);
        }
        if (sendto(sock,recvBuffer,recvLen,0,(struct sockaddr*)&toAddr,sizeof(toAddr))!=recvLen)
        {
            printf("sendto fail\r\n");
            close(sock);
            exit(0);
        }

    }
    return 0;
}


