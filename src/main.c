#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "server.h"
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include "main.h"
#include "screen.h"
#include "ip.h"
#define SERVER_PORT 50000
#define MAX_LISTEN  5

int hostport;
char hostip[32];
int host_max_listen;

int main(int argc,char *argv[])
{
    struct screen_buffer_list_node *phead;
    struct screen_buffer_list_node *pnow;
    struct screen_buffer_list_node *plist;

    int err;

    /*
     * the variable about the socket
     */
    int sockfd;
    socklen_t length;
    /*
     * the variable about the thread
     */
    pthread_t thread_id;
    pthread_attr_t  attr;

    if ((argc>1)&&(argv[1]!=NULL))
    {
        hostport=atoi(argv[1]);
    }
    else
    {
        hostport=SERVER_PORT;
    }
    if ((argc==2)&&(argv[2]!=NULL))
    {
        host_max_listen=atoi(argv[2]);
    }
    else
    {
        host_max_listen=MAX_LISTEN;
    }
    /* *
     * get the local ip
     * */
    getlocalhostip(hostip);
    /*
     * start tcp server
     */
    sockfd=  tcp_server_init(hostport,host_max_listen);

    if ((err=pthread_attr_init(&attr))!=0)
    {
        printf("the attribut init failed :%s\n",strerror(err));
        exit(1);
    }
    err=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if (err!=0)
    {
        printf("the attribut detach error:%s\n",strerror(err));
        exit(1);
    }
    phead=(struct screen_buffer_list_node *)malloc(sizeof(struct screen_buffer_list_node));
    memset(phead,0,sizeof(struct screen_buffer_list_node));   
    pnow=phead;

    sprintf(phead->buffer,"author:\nrensuiyi\n");
    
    /*
     * creat the screen thread
     */
    err=pthread_create(&thread_id,&attr,screen_thread,phead);
    if (err!=0)
    {
        printf("can't creat the thread :%s\n",strerror(err));
        exit(1);
    }
    /*
     * operation not finished ,should sleep for moment or the buffer cannot be refreshed. 
     */
   sleep(1);
    length=sizeof(struct sockaddr_in);
    while (1)
    {
        /*
         * get the memory 
         */
        plist=(struct screen_buffer_list_node *)malloc(sizeof(struct screen_buffer_list_node));
        memset(phead,0,sizeof(struct screen_buffer_list_node));   
        plist->sockfd=accept(sockfd,(struct sockaddr *)(&plist->addr),&length);
        if(plist->sockfd==-1)
        {
            free(plist);
        }
        else
        {
            /*
             *  add the plist to the list
             */
            pthread_mutex_init(&(plist->mutex),NULL);
            //sprintf()
           
            pnow->pnext=plist;
            plist->ppre=pnow;

            err=pthread_create(&thread_id,&attr,subthread,plist);
            sleep(1);
            /*
             * collect the list_node of the disconnect member 
             * the fd<0 =-1 
             */


        }

    }
    return 0;

}

