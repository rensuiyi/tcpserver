/*
 * =====================================================================================
 *
 *       Filename:  server.h
 *
 *    Description:  part of tcp server
 *
 *        Version:  1.0
 *        Created:  2012年08月20日 21时47分26秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  rensuiyi 
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef __TCPSERVER_H_
#define __TCPSERVER_H_
#ifdef  __cpluscplus
extern "C"
{
#endif /*__cpluscplus*/

struct screen_buffer_list_node
{
	char buffer[128];
	int  attr;
	struct screen_buffer_list_node *pnext;
};


#ifdef __cpluscplus
}
#endif /*__cpluscplus*/
#endif /*__TCPSERVER_H_*/
