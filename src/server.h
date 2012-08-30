/**
 *  @brief this file is about the server system
 *  
 *  @author rensuiyi
 */
#ifndef __TCPSERVER_H_
#define __TCPSERVER_H_
#ifdef  __cpluscplus
extern "C"
{
#endif /*__cpluscplus*/

extern void *subthread(void *para);
extern int tcp_server_init(int port,int len);
#ifdef __cpluscplus
}
#endif /*__cpluscplus*/
#endif /*__TCPSERVER_H_*/
