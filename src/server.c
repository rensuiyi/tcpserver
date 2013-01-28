#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <time.h>
#include <fcntl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/select.h>
#include "main.h"


extern pthread_mutex_t g_file_mutex;
extern struct screen_buffer_list_node * g_phead;

#define  RECIEVE_BUFFER_SIZE  3096 
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
    char  data_buffer[RECIEVE_BUFFER_SIZE];
    char  log_buffer[1024];
    int logfile_fd;
    struct tm *sock_time;
    time_t  timenow;
     fd_set read_fds;
    struct timeval timeout={TCP_TIMEOUT,0};

    /* 
     *  get the tid
     */
    sock_list->tid=pthread_self();
    /*
     * write the log to the log file
     */
    time(&sock_list->time_start);
    sock_time= localtime(&sock_list->time_start);
    pthread_mutex_lock(&g_file_mutex);
    logfile_fd=open("./tcplog.txt",O_WRONLY|O_APPEND|O_CREAT,0755);

    if (logfile_fd>=0)
    {
        sprintf(log_buffer,"\n%02d-%02d  %02d:%02d:%02d\n",
                sock_time->tm_mon+1,
                sock_time->tm_mday,
                sock_time->tm_hour,
                sock_time->tm_min, 
                sock_time->tm_sec);
        write(logfile_fd,log_buffer,strlen(log_buffer));
        sprintf(log_buffer,"%s [%d]  connect \n",(char *)inet_ntoa( sock_list->addr.sin_addr),sock_list->addr.sin_port);
        write(logfile_fd,log_buffer,strlen(log_buffer));
        close(logfile_fd);
    }
    pthread_mutex_unlock(&g_file_mutex);

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

        pthread_mutex_unlock(&sock_list->mutex);
        timeout.tv_sec=TCP_TIMEOUT;
        FD_ZERO(&read_fds);//clear the set
        FD_SET(sock_list->sockfd,&read_fds);
        switch (select(sock_list->sockfd+1,&read_fds,NULL,NULL,&timeout))
        {
        case -1:goto out;
        case 0 :goto out;
        default:
            if (!FD_ISSET(sock_list->sockfd,&read_fds) )
            {
                goto out;
            }
        }
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
                sock_list->timeout=TCP_TIMEOUT; 
                /*
                 * write the log file 
                 */
                time(&timenow);
                sock_time= localtime(&timenow);
                pthread_mutex_lock(&g_file_mutex);
                logfile_fd=open("./tcplog.txt",O_WRONLY|O_APPEND|O_CREAT,0755);

                if (logfile_fd>=0)
                {
                    sprintf(log_buffer,"\n%02d-%02d  %02d:%02d:%02d\n",
                            sock_time->tm_mon,
                            sock_time->tm_mday+1,
                            sock_time->tm_hour,
                            sock_time->tm_min, 
                            sock_time->tm_sec);
                    write(logfile_fd,log_buffer,strlen(log_buffer));
                    sprintf(log_buffer,"%s [%d] data:\n",(char *)inet_ntoa( sock_list->addr.sin_addr),sock_list->addr.sin_port);
                    write(logfile_fd,log_buffer,strlen(log_buffer));
                    data_buffer[len]='\n';
                    data_buffer[len+1]='\0';
                    write(logfile_fd,data_buffer,len+1);

                    close(logfile_fd);
                }
                pthread_mutex_unlock(&g_file_mutex);
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
   close(sock_list->sockfd);
    //sock_list->sockfd=-1;
     time(&timenow);
    sock_time= localtime(&timenow);
    pthread_mutex_lock(&g_file_mutex);
    logfile_fd=open("./tcplog.txt",O_WRONLY|O_APPEND|O_CREAT,0755);

    if (logfile_fd>=0)
    {
        sprintf(log_buffer,"\n%02d-%02d  %02d:%02d:%02d\n",
                sock_time->tm_mon+1,
                sock_time->tm_mday,
                sock_time->tm_hour,
                sock_time->tm_min, 
                sock_time->tm_sec);
        write(logfile_fd,log_buffer,strlen(log_buffer));
        sprintf(log_buffer,"%s [%d]  disconnect \n",(char *)inet_ntoa( sock_list->addr.sin_addr),sock_list->addr.sin_port);
        write(logfile_fd,log_buffer,strlen(log_buffer));
        close(logfile_fd);
    }
    pthread_mutex_unlock(&g_file_mutex);
    /*
     * release the list node
     */
    pthread_mutex_lock(&g_phead->mutex);
    if (sock_list->pnext!=NULL)
    {
        sock_list->pnext->ppre=sock_list->ppre;
    }
    if (sock_list->ppre!=NULL)
    {
        sock_list->ppre->pnext=sock_list->pnext;
    }
    free(sock_list);
    pthread_mutex_unlock(&g_phead->mutex);
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
int tcp_server_init(int port,int queuelen){
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

