#ifndef __MAIN_H_
#define __MAIN_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <pthread.h>
#include <sys/socket.h>
#include<netinet/in.h>


#define TCP_TIMEOUT  120 //2min
struct screen_buffer_list_node
{
	char buffer[128];
	int  attr;
        int timeout;
        pthread_mutex_t mutex;
        struct sockaddr_in addr;
        int sockfd;
        struct screen_buffer_list_node *ppre;
	struct screen_buffer_list_node *pnext;
};

#ifdef __cplusplus
}
#endif
#endif /*__MAIN_H_*/