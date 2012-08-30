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

extern char hostip[64];
extern int hostport;
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
    char  buffer[128];
    int num=0;
    phead=(struct screen_buffer_list_node *)para;
  
    printf("\033[2J");
    while(1)
    {  
        plist=phead;

        time(&now);
        ptime_now=localtime(&now);
        printf("\033[H\033[l");
       /*
        * update the screen
       */
        sprintf(buffer,"****IP:%s    PORT:%5d    TIME:%02d:%02d:%02d****\n",
                hostip,
                hostport,
                ptime_now->tm_hour,
                ptime_now->tm_min, 
                ptime_now->tm_sec);

        printf("%s",buffer);   
        num=0;
        while(plist!=NULL)
        {
            if(num==0)
            {
                printf("%s",plist->buffer);
            }
            else 
            {
                if(plist->buffer!=NULL)
                {
                    printf("NO:%2d%s",num,plist->buffer);
                }
            }
                plist=plist->pnext;
                num++;     
        }
        fflush(stdout); 
         sleep(1);
    }
    pthread_exit(NULL);
}
