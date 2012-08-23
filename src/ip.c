/*
 * =====================================================================================
 *
 *       Filename:  ip.c
 *
 *    Description:  get the ip and mac address
 *
 *        Version:  1.0
 *        Created:  2012年08月23日 23时58分56秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  rensuiyi 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdio.h>    
#include <stdlib.h>    
#include <sys/types.h>    
#include <sys/socket.h>    
#include <netinet/in.h>    
#include <netdb.h>    
#include <arpa/inet.h>    
#include <signal.h>    
   
#include <sys/ioctl.h>    
#include <linux/if.h>    
long getlocalhostip(char *ipbuf)
{   
  int  MAXINTERFACES=16;   
    long ip;   
    int fd, intrface, retn = 0;   
    struct ifreq buf[MAXINTERFACES]; ///if.h    
    struct ifconf ifc; ///if.h    
    ip = -1;   
    if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) >= 0) //socket.h    
    {   
        ifc.ifc_len = sizeof buf;   
        ifc.ifc_buf = (caddr_t) buf;   
        if (!ioctl (fd, SIOCGIFCONF, (char *) &ifc)) //ioctl.h    
        {   
            intrface = ifc.ifc_len / sizeof (struct ifreq);    
            while (intrface-- > 0)   
            {   
                if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))   
                {   
                    ip=inet_addr( inet_ntoa( ((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr) );//types    
                    break;   
                }   
                       
            }   
               
        }   
        close (fd);   
    }
	sprintf(ipbuf,"%03u.%03u.%03u.%03u",(uint8_t)(ip),(uint8_t)(ip>>8),(uint8_t)(ip>>16),(uint8_t)(ip>>24));   
    return ip;   
	
}   
  
#if 0   
int main(int argc, char **argv){   
   char  ip[32];   
    getlocalhostip(ip);      
    printf("local ip:%s",ip);   
    return 0;   
} 
#endif















