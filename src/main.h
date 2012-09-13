#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <pthread.h>
#include <sys/socket.h>
#include<netinet/in.h>
#include <time.h>

#define TCP_TIMEOUT  120 //2min
struct screen_buffer_list_node
{
        pthread_t tid;
	char buffer[128];                  /*screen buffer*/
	int  attr;
        int timeout;                          /*time out value*/
        time_t time_start;                /*the start time of the socket*/
        pthread_mutex_t mutex;     /*the thread mutex*/
        struct sockaddr_in addr;      /*the addr of the client*/
        int sockfd;                            /*the socket fd*/
        struct screen_buffer_list_node *ppre;
	struct screen_buffer_list_node *pnext;
};

#ifdef __cplusplus
}
#endif
#endif /*__MAIN_H_*/