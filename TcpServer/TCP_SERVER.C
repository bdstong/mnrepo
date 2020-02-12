//#include "stm32f4xx_hal.h"
//#include "lwip/opt.h"

//#include "lwip/timeouts.h"
//#include "netif/ethernet.h"
//#include "netif/etharp.h"
//#include "lwip/ethip6.h"
//#include "ethernetif.h"
#include <string.h>
#include "cmsis_os.h"


#include "tcp.h"
#include "TCP_SERVER.h"
#include "sockets.h"
//#include "app.h"
char buf[1024];

/*********************************************************************************
**********************MCU启明 STM32F407应用开发板(高配版)*************************
**********************************************************************************
* 文件名称: TCP_SERVER.c                                                         *
* 文件简述：TCP服务器运用                                                        *
* 创建日期：2015.10.08                                                           *
* 版    本：V1.0                                                                 *
* 作    者：Clever                                                               *
* 说    明：                                                                     * 
* 声    明：本例程代码仅用于学习参考                                             *
**********************************************************************************
*********************************************************************************/


/****************************************************************************
* 名    称: tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* 功    能：TCP数据接收和发送
* 入口参数： 
* 返回参数： 
* 说    明：这是一个回调函数，当一个TCP段到达这个连接时会被调用       
****************************************************************************/	
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	struct pbuf *p_temp = p;
	
	if(p_temp != NULL)
	{	
		tcp_recved(pcb, p_temp->tot_len);    //获取数据长度 tot_len：tcp数据块的长度
		while(p_temp != NULL)	
		{				
			tcp_write(pcb,p_temp->payload,p_temp->len,TCP_WRITE_FLAG_COPY); 	// payload为TCP数据块的起始位置       
			tcp_output(pcb);
			p_temp = p_temp->next;
		}		
	}
	else
	{
		tcp_close(pcb); 	// 作为TCP服务器不应主动关闭这个连接
	}
	pbuf_free(p); 		 // 释放该TCP段  
	err = ERR_OK;
	return err;
}

/****************************************************************************
* 名    称: tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* 功    能：回调函数
* 入口参数： 
* 返回参数： 
* 说    明：这是一个回调函数，当一个连接已经接受时会被调用
****************************************************************************/	
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_NORMAL);//TCP_PRIO_MIN); 		/* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	tcp_recv(pcb,tcp_server_recv); 				/* 设置TCP段到时的回调函数 */
	err = ERR_OK;
	return err;
}

/****************************************************************************
* 名    称: TCP_server_init(void)
* 功    能：完成TCP服务器的初始化，主要是使得TCP通讯快进入监听状态
* 入口参数： 
* 返回参数： 
* 说    明：
****************************************************************************/	
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;

	pcb = tcp_new(); 								              // 建立通信的TCP控制块(pcb) 
	tcp_bind(pcb,IP_ADDR_ANY,TCP_Server_PORT); 	  // 绑定本地IP地址和端口号（作为tcp服务器）  
	pcb = tcp_listen(pcb); 							          // 进入监听状态  
	tcp_accept(pcb,tcp_server_accept); 			      // 设置有连接请求时的回调函数  
}
/*
struct sockaddr {
  u8_t        sa_len;
  sa_family_t sa_family;
  char        sa_data[14];
};
struct sockaddr_in {
  u8_t            sin_len;
  sa_family_t     sin_family;
  in_port_t       sin_port;
  struct in_addr  sin_addr;
#define SIN_ZERO_LEN 8
  char            sin_zero[SIN_ZERO_LEN];
};

*/
void tcp_svr_init(void)//
{
  sys_thread_new("tcpecho_thread", vLWIPSendTask, NULL, 512, TCPIP_THREAD_PRIO);//TCPECHO_THREAD_PRIO);

}
//recive one client
void vLWIPSendTask11( void *pvParameters )
{
   int listenfd;
    int remotefd;
    int len;
    struct sockaddr_in local_addr,remote_addr; 
        fd_set readset;
        fd_set writeset;
        struct timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;
 
 
    //struct lwip_socket* sock;
 
    listenfd = socket(AF_INET,SOCK_STREAM,0);
 
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(8080);
    local_addr.sin_len = sizeof(local_addr);
    local_addr.sin_addr.s_addr = INADDR_ANY;
	
	//bzero(&(local_addr.sin_zero),8);
   
    if (bind(listenfd, (struct sockaddr *) &local_addr, sizeof(local_addr)) < 0)
    {
        return ;
    }
 
    if (listen(listenfd, 1) == -1)
    {
        return;
    }
 
    len = sizeof(remote_addr);
        while(1)
        {
      //??????,lwip??????listen??,??accept

					remotefd = accept(listenfd, (struct sockaddr *)&remote_addr, &len);
						printf("accepted fd=%d\r\n",remotefd);
          //close(listenfd);
          //listenfd = -1;
      //getpeername(remotefd, (struct sockaddr *)&remote_addr, &len);
          if(remotefd != -1)
          {
            //send(remotefd,"start to work!\r\n",16,0);
            for(;;)
            {
              FD_ZERO(&readset);
              FD_ZERO(&writeset);
              FD_SET(remotefd, &readset);
              FD_SET(remotefd, &writeset);
							
 
             //int ret=select(remotefd+1, &readset, &writeset, 0, &timeout);//=0.timeout
             int ret=select(remotefd+1, &readset, 0, 0, &timeout);//=0.timeout
                    
							//printf("readset=%d  writeset=%d\r\n",readset.fd_bits[0],writeset.fd_bits[0]);
							//printf("ret=%d\r\n",ret);
              if(ret > 0)
              {
                if (FD_ISSET(remotefd, &readset))
                {

									//printf("readset\r\n");

                  memset(buf,0,50);
                  if(recv(remotefd,buf,50,0) <= 0)
                  {
										printf("recv<=0\r\n");
                    close(remotefd);
                    remotefd = -1;
                    break;
                  }
                  else
                  {
										//printf("recv>0\r\n");
                    int i = strlen(buf);
                    send(remotefd,buf,i ,0);
                  }
                }
                
                else if(FD_ISSET(remotefd, &writeset))
                {
									printf("writeset\r\n");
                   //send(remotefd,"this is time to send!\r\n",25,0);
                }
                
              }
              else if(ret < 0)
              {
                close(remotefd);
                remotefd = -1;
                break;
              } 
             
            }
          }
        }
       
        vTaskDelete( NULL );
}

//recive muliti clients
void vLWIPSendTask(void *pvParameters )  
{  
	int listener;  
	int state[FD_SETSIZE];  
	struct sockaddr_in localAddr;  
	int i, maxfd;  

	fd_set readset, writeset, exset;  

	localAddr.sin_family = AF_INET;  
	localAddr.sin_addr.s_addr = INADDR_ANY;  
	localAddr.sin_port = htons(8080);  

	for (i = 0; i < FD_SETSIZE; ++i) state[i] = 0;  
  
	listener = socket(AF_INET, SOCK_STREAM, 0);  
  //make_nonblocking(listener);  
	int argp=1;
	ioctl(listener,FIONBIO,&argp) ;
	//printf("argp=%d\r\n",argp);
 
	if (bind(listener, (struct sockaddr*)&localAddr, sizeof(localAddr)) < 0) 
	{  
		perror("bind");  
		return;  
  }  
  
	if (listen(listener, 1)<0) 
	{  
		perror("listen");  
		return;  
	}  
  

	FD_ZERO(&readset);  
	FD_ZERO(&writeset);  
	FD_ZERO(&exset);  
  
	while (1) 
	{  
		maxfd = listener;  

		FD_ZERO(&readset);  
		FD_ZERO(&writeset);  
		FD_ZERO(&exset);  

		FD_SET(listener, &readset);  

		for (i=0; i < FD_SETSIZE; ++i) 
		{  
			if (state[i]) 
			{  
				if (i > maxfd)   maxfd = i;  
				FD_SET(i, &readset);  
				//if (state[i]->writing) 
				//{  
				//	FD_SET(i, &writeset);  
				//}  
			}  
		}  

		if (select(maxfd+1, &readset, &writeset, &exset, NULL) < 0) 
		{  
			perror("select");  
			return;  
		}  
		
		if (FD_ISSET(listener, &readset)) 
		{  
			struct sockaddr_storage ss;  
			socklen_t slen = sizeof(ss);  
			int fd = accept(listener, (struct sockaddr*)&ss, &slen);  
			if (fd < 0) 
			{  
				perror("accept");  
			} 
			else if (fd > FD_SETSIZE) 
			{  
				close(fd);  
			} 
			else 
			{  
				//printf("accept %d\r\n",listener);
				// make_nonblocking(fd);  
				ioctl(fd,FIONBIO,&argp) ;
				state[fd] = 1;//alloc_fd_state();  
				//assert(state[fd]);/*XXX*/  
			}  
		}  
	
		for (i=0; i < maxfd+1; ++i) 
		{  
			int r = 0;  
			if (i == listener)  continue;  
			if (FD_ISSET(i, &readset)) 
			{  
				//r = do_read(i, state[i]);
				int cc=recv(i,buf,50,MSG_DONTWAIT);
				//printf("recived %d rcv num=%d\r\n",i,cc);
				if(cc)
				{
					buf[cc]=0;
					//printf("%s\r\n",buf);
					//write(i,buf,cc);
					send(i,buf,cc,MSG_DONTWAIT);
				}
				else
				{
					close(i);
					state[i]=0;
					//printf("recv<=0 closed\r\n");
				}
			}  
/*
				if (r == 0 && FD_ISSET(i, &writeset)) 
				{  
					//r = do_write(i, state[i]); // ref same as above  
					if (r) 
					{  
						//free_fd_state(state[i]);  
						state[i] = 0;  
						close(i);  
						//printf("close %d\r\n",i);
					}  
        }  
*/
		}//for
	}//while  
}