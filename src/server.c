#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include "main.h"

extern g_file_mutex;

#define  RECIEVE_BUFFER_SIZE   1024
/* 
 * @brief the entry of the connect thread 
 *    write the data to the file 
 *    update the list buffer
 * 
 * @author renyong (8/30/2012)
 * 
 * @param para 
 * 
 * @return void* 
 */
void * subthread(void *para)
{
    struct screen_buffer_list_node  *sock_list;
    sock_list=(struct screen_buffer_list_node *)para;
    int recive_data_len=0;
    int send_data_len=0;
    int len;
    char  data_buffer[1024];
    /*
     * read the words and send it back
     */
    while (1)
    {
    
        pthread_mutex_lock(&sock_list->mutex); 
        /*
         * get the lock and modify the buffer 
        */
        sprintf(sock_list->buffer,"%16s %7d   %8s %10d %10d",
                (char *)inet_ntoa( sock_list->addr.sin_addr),
                sock_list->addr.sin_port,
                "connect",
                recive_data_len,
                send_data_len
               );
        //  printf("%s\n%08x",sock_list->buffer,sock_list);
          pthread_mutex_unlock(&sock_list->mutex);
        //  fflush(stdout);
         /*
          * recive the data from the socket 
          */   
#if 1
        len=read(sock_list->sockfd,data_buffer,RECIEVE_BUFFER_SIZE-1);
        if (len>0)
        {
            recive_data_len+=len;
            len=write(sock_list->sockfd,data_buffer,len);
            if (len>0)
            {
                send_data_len+=len;
            }
            else
            {
                goto out;
            }

        }
        else
        {
            goto out;
        }
#endif 
        usleep(1000);
    }
out:
    sock_list->sockfd=-1;
    pthread_exit(NULL);
}
/**
 * @brief init the tcp server which and listen the port 
 * 
 * @author renyong (8/30/2012)
 * 
 * @param port 
 * @param queuelen 
 * 
 * @return int  :the sock fd
 */
int tcp_server_init(int port,int queuelen)
{
    int serv_sockfd;
    struct sockaddr_in server_addr;
    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htons(INADDR_ANY);
    server_addr.sin_port=htons(port);
    if ((serv_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("creat socket failed!\n");
        exit(1);
    }
    if ((bind(serv_sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)))<0)
    {
        printf("bind port %d failed!\n",port);
        exit(1);
    }
    if ((listen(serv_sockfd,queuelen))<0)
    {
        printf("server listen failed!\n");
        exit(1);
    }
    return serv_sockfd;
}
