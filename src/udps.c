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
extern void *udp_server_thread(void *para)
{
    /*
     *  init the socket
     */
    extern int g_hostport;
    int port=g_hostport;
    int sockfd;
    int udp_file_fd;
    char buffer[128];
    char time_buffer[32];
    struct sockaddr_in servaddr, cliaddr;
    unsigned int len;
    /* 
     * create a socket
     */
    sockfd = socket(AF_INET, SOCK_DGRAM, 0); 

    /* 
     * init servaddr
     */
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port );
    /*
     *  bind the socket
     */    
    if (bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
    {
        perror("bind error\n");
        pthread_exit(NULL);
    }
    udp_file_fd=open("./udplog.txt",O_WRONLY|O_APPEND|O_CREAT,0755);
    if (udp_file_fd<0)
    {
        perror("open file failed\n");
        goto out;
    }
    else
    {
        get_time_str(time_buffer,32);
        write(udp_file_fd,time_buffer,strlen(time_buffer));
        sprintf(buffer,"udp server start port:%6d \n",port);
        write(udp_file_fd,buffer,strlen(buffer));
        close(udp_file_fd);
    }
    /*
     * read the data and sendback the words 
     */
    len=sizeof(cliaddr);
    while(1 );
    {
        int n;
        /* waiting for receive data */
        n = recvfrom(sockfd,buffer,UDP_BUFFER_SIZE, 0, (struct sockaddr *)&cliaddr, &len);
        if(n>0)
        {
            if ((udp_file_fd=open("./udplog.txt",O_WRONLY|O_APPEND|O_CREAT,0755))>=0)
            {
                get_time_str(time_buffer,32);
                write(udp_file_fd,time_buffer,strlen(time_buffer));
                write(udp_file_fd,buffer,strlen(buffer));
                write(udp_file_fd,"\n",1);
                close(udp_file_fd);
            }
        }
        /* sent data back to client */
        sendto(sockfd, buffer, n, 0, (struct sockaddr *)&cliaddr, len);
    }
    out:
    close(sockfd);
    pthread_exit(NULL);
}


