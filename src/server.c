#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include "main.h"

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
	/*
	 * read the words and send it back
	 */
	sprintf(sock_list->buffer,"hello\n");
	while(1)
	{

         sleep(1);
	}


	sleep(1);
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
    if((serv_sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		printf("Creat socket failed!\n");
		exit(1);
	}
	if((bind(serv_sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr)))<0)
	{
		printf("bind port %d failed!\n",port);
		exit(1);
	}
	if((listen(serv_sockfd,queuelen))<0)
	{
		printf("server listen failed!\n");
		exit(1);
	}
	return serv_sockfd;
}
