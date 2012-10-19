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
#include "udps.h"
#define SERVER_PORT 50000
#define MAX_LISTEN  5

/*global variable*/
int g_hostport;
char g_hostip[32];
int g_host_max_listen;
struct screen_buffer_list_node *g_phead;

pthread_mutex_t  g_file_mutex;

static int pthread_make(void *(*fn)(void *),void * arg)
{
    int err;
    pthread_t tid;
    pthread_attr_t attr;
    err=pthread_attr_init(&attr);
    if (err!=0)
    {
        return err;
    }
    err=pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
    if (err==0)
    {
        err=pthread_create(&tid,&attr,fn,arg);
    }
    pthread_attr_destroy(&attr);
    return err;
}
/*
 * @brief find the tail of the list
 * 
 * @author renyong (8/30/2012)
 * 
 * @param phead 
 * 
 * @return struct screen_buffer_list_node* 
 */
static struct screen_buffer_list_node * list_find_tail(struct screen_buffer_list_node *phead)
{
    struct screen_buffer_list_node *plist;
    plist=phead;
     while(plist->pnext!=NULL)
     {
         plist=plist->pnext;
     }
     return plist;
}
int main(int argc,char *argv[])
{
 
    struct screen_buffer_list_node *pnow;
    struct screen_buffer_list_node *plist;
    struct screen_buffer_list_node *pcleanlist;

    int err;

    /*
     * the variable about the socket
     */
    int sockfd;
    socklen_t length;
    /*
     * the variable about the thread
     */

    if ((argc>1)&&(argv[1]!=NULL))
    {
        g_hostport=atoi(argv[1]);
    }
    else
    {
        g_hostport=SERVER_PORT;
    }
    if ((argc==2)&&(argv[2]!=NULL))
    {
        g_host_max_listen=atoi(argv[2]);
    }
    else
    {
        g_host_max_listen=MAX_LISTEN;
    }
    /* *
     * get the local ip
     * */
    getlocalhostip(g_hostip);
    /*
     * start tcp server
     */
    sockfd=  tcp_server_init(g_hostport,g_host_max_listen);


    g_phead=(struct screen_buffer_list_node *)malloc(sizeof(struct screen_buffer_list_node));
    memset(g_phead,0,sizeof(struct screen_buffer_list_node));   
    pnow=g_phead;
    pthread_mutex_init(&(g_phead->mutex),NULL);

    pthread_mutex_init(&g_file_mutex,NULL);

    sprintf(g_phead->buffer,"author:rensuiyi\n");

    /*
     * creat the screen thread
     */
#if 1
    err=pthread_make(screen_thread,g_phead);
    if (err!=0)
    {
        printf("can't creat the thread :%s\n",strerror(err));
        exit(1);
    } 
    /*
     * operation not finished ,should sleep for moment or the buffer cannot be refreshed. 
     */
    sleep(1);
#endif

#if 0
    err=pthread_make(udp_server_thread,NULL);
    if(err!=0)
    {
        printf("can't create the thread:%s\n",strerror(err));
        exit(1);
    }
#endif
 
   length=sizeof(struct sockaddr_in);

#if 0
    plist=(struct screen_buffer_list_node *)malloc(sizeof(struct screen_buffer_list_node));
    memset(plist,0,sizeof(struct screen_buffer_list_node));   
    pthread_mutex_init(&(plist->mutex),NULL);

    pnow->pnext=plist;
    plist->ppre=pnow;
    pnow=plist;

    //err=pthread_make(subthread,plist);
    sleep(1);
#endif 

    while (1)
    {
        /*
         * get the memory 
         */
        plist=(struct screen_buffer_list_node *)malloc(sizeof(struct screen_buffer_list_node));
        memset(plist,0,sizeof(struct screen_buffer_list_node));   
        plist->sockfd=accept(sockfd,(struct sockaddr *)(&plist->addr),&length);
        if (plist->sockfd==-1)
        {
            free(plist);
        }
        else
        {
            /*
             *  add the plist to the list
             */

            /*get the list lock*/
            pthread_mutex_lock(&g_phead->mutex);

            pthread_mutex_init(&(plist->mutex),NULL);
            plist->timeout=TCP_TIMEOUT;
            time(&(plist->time_start));
            //sprintf()
            pnow=list_find_tail(g_phead);

            pnow->pnext=plist;
            plist->ppre=pnow;
            //pnow=plist;

            /*
             * collect the list_node of the disconnect member 
             * the fd<0 =-1 
             */
#if 0
            pcleanlist=phead;
            while(pcleanlist!=NULL)
            {
                pnow=pcleanlist->pnext;
                if(pcleanlist->sockfd==-1)
                {
                    if(pcleanlist->pnext!=NULL );
                    {
                        pcleanlist->ppre->pnext=pcleanlist->pnext;
                    }
                    if(pcleanlist->ppre!=NULL)
                    {
                        pcleanlist->pnext->ppre=pcleanlist->ppre;
                    }
                    free(pcleanlist);
                }
             pcleanlist=pnow;
            }
#endif  
            /*release the list lock*/
            pthread_mutex_unlock(&g_phead->mutex);

            err=pthread_make(subthread,plist);
            if(err!=0)
            {
                exit(0);
            }
          //  sleep(2);
        
        }

    }
    return 0;

}

