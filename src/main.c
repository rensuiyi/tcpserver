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

#define SERVER_PORT 5000
#define MAX_LISTEN  5
int main(int argc,char *argv[])
{
	printf("tcp server start\n");
	int port;
	int max_listen;
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
   
   printf("======PORT:%d     MAX LISTEN:%d=====\n",port,max_listen);
	return 0;
	
}

