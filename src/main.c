/*
 * =====================================================================================
 *
 *       Filename:  main.c
 *
 *    Description:  part of tcpserver
 *
 *        Version:  1.0
 *        Created:  2012年08月20日 19时09分14秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "server.h"
#include <time.h>

#define SERVER_PORT 5000
#define MAX_LISTEN  5

int main(int argc,char *argv[])
{
	int port;
	struct screen_buffer_list_node *phead;
	struct screen_buffer_list_node *pnow;
	int max_listen;
	/*
	 * the variable about time
	 */
	time_t now;
	struct tm *ptime_now;

	if((argc>1)&&(argv[1]!=NULL))
	{
        port=atoi(argv[1]);
	}
	else
	{
		port=SERVER_PORT;
	}
    if((argc==2)&&(argv[2]!=NULL))
	{
		max_listen=atoi(argv[2]);
	}
	else
	{
		max_listen=MAX_LISTEN;
	}
    phead=(struct screen_buffer_list_node *)malloc(sizeof(struct screen_buffer_list_node));
	memset(phead,0,sizeof(struct screen_buffer_list_node));   
	while(1)
	{
		time(&now);
		ptime_now=localtime(&now);
		printf("\033[2J");
	sprintf(phead->buffer,"****IP:192.168.1.0    PORT:%5d    TIME:%02d:%02d:%02d****\n",port,ptime_now->tm_hour,ptime_now->tm_min,ptime_now->tm_sec);
    
    printf(phead->buffer);
	sleep(1);
	fflush(stdout);
	}
	return 0;
	
}

