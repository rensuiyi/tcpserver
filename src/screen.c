/**
 * @brief this file is used to update the screen 
 *  
 * @author rensuiyi 
 *  
 * @date 2012-8-30 
 */

#include "time.h"
#include "main.h"
#include  "errno.h"
#include  "pthread.h"
#include "stdio.h"
#include  "stdlib.h"
#include "sys/types.h"

extern char g_hostip[64];
extern int g_hostport;
/**
 * @brief the thread of the flush the screen
 * 
 * @author renyong (8/30/2012)
 * 
 * @param para 
 */
void *screen_thread(void * para)
{


    time_t now;
    struct tm * ptime_now;
    struct screen_buffer_list_node *plist;
    struct screen_buffer_list_node *phead;
    char  buffer[128*10];
    int num=0;
    phead=(struct screen_buffer_list_node *)para;

    printf("\033[2J");
    while (1)
    {
        

        time(&now);
        ptime_now=localtime(&now);
        printf("\033[J\033[H\033[l");
        /*
         * update the screen
        */
        sprintf(buffer,"****IP:%s    PORT:%5d    TIME:%02d:%02d:%02d****\n",
                g_hostip,
               g_hostport,
                ptime_now->tm_hour,
                ptime_now->tm_min, 
                ptime_now->tm_sec);

        //printf("%s",buffer);   
        sprintf(buffer,"%s%s",buffer,"NO   IP              PORT     STATU      RECIEVE       SEND      TIME(s)\n");
        num=0;
        /* get the list lock*/
        pthread_mutex_lock(&phead->mutex);
        /*
         * print the socket infomation 
         */
        plist=phead->pnext;    
        while (plist!=NULL)
        {
            pthread_mutex_lock(&plist->mutex);
            time(&now);
            if (plist->buffer!=NULL)
            {
                sprintf(buffer,"%s%2d%s   %6d\n",buffer,num,plist->buffer,(int)(now-plist->time_start));            
            }
            if(plist->timeout==0)
            {
               // pthread_cancel(plist->tid);
            }
            else
            {
                plist->timeout--;
            }
            pthread_mutex_unlock(&plist->mutex);
            plist=plist->pnext;
            num++;     
        } 
        pthread_mutex_unlock(&phead->mutex);
        printf("%s",buffer);
        fflush(stdout); 
        sleep(1);
    }
    pthread_exit(NULL);
}
