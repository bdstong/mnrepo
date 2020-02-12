//#include "stm32f4xx_hal.h"
//#include "lwip/opt.h"

//#include "lwip/timeouts.h"
//#include "netif/ethernet.h"
//#include "netif/etharp.h"
//#include "lwip/ethip6.h"
//#include "ethernetif.h"
#include <string.h>
#include "cmsis_os.h"
#include "sys.h"
#include "tcp.h"
#include "sockets.h"
#include "app.h"

#define TCP_RXBUF_SIZE 2048
uint8_t tcp_RxBuf[TCP_RXBUF_SIZE];

int state[FD_SETSIZE];  
extern osMessageQId ethMsgQueueHandle;

int  maxfd;  

void psd()
{

	for (int i = 0; i < FD_SETSIZE; i++) printf("state[%d]=%d ",i,state[i]);

	printf("maxfd=%d\r\n",maxfd);
}
//recive muliti clients
void vLWIPSendTask(void *pvParameters )  
{  
	int listener;  
	struct sockaddr_in localAddr;  
	int i;  
	ETH_MSG eth_msg;
	BaseType_t xReturn;

	fd_set readset, writeset, exset;  

	localAddr.sin_family = AF_INET;  
	localAddr.sin_addr.s_addr = INADDR_ANY;  
	localAddr.sin_port = htons(8080);  

	for (i = 0; i < FD_SETSIZE; i++) state[i] = 0;  
  
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
 	printf("start listener=%d\r\n",listener);
 
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

		for (i=0; i < FD_SETSIZE; i++) 
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

			printf("accept %d\r\n",fd);
			if (fd < 0) 
			{  
				//perror("accept");  
			} 
			else if (fd > FD_SETSIZE) 
			{  
				close(fd);  
			} 
			else 
			{  
				//make_nonblocking(fd);  
				ioctl(fd,FIONBIO,&argp);
				state[fd] = 1;//alloc_fd_state();  
				//assert(state[fd]);/*XXX*/  
			}  
		}  
	
		for (i=0; i < maxfd+1; i++) 
		{  
			if (i == listener)  continue;  
			if (FD_ISSET(i, &readset)) 
			{  
				int rcvNum=recv(i,tcp_RxBuf,TCP_RXBUF_SIZE,MSG_DONTWAIT);
				//printf("recived %d rcv num=%d\r\n",i,rcvNum);
				if(rcvNum)
				{
					//send(i,tcp_RxBuf,rcvNum,MSG_DONTWAIT);
					eth_msg.s=i;
					eth_msg.buf=tcp_RxBuf;
					eth_msg.len=rcvNum;
					xReturn=xQueueSendToBack(ethMsgQueueHandle,&eth_msg,0);
					if(xReturn!=pdTRUE)printf("send to queue error\r\n");
				}
				else
				{
					close(i);
					state[i]=0;
					printf("closed %d\r\n",i);
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
void tcp_socket_svr_init(void)//
{
  sys_thread_new("tcpecho_thread", vLWIPSendTask, NULL, DEFAULT_THREAD_STACKSIZE, TCPIP_THREAD_PRIO);//TCPECHO_THREAD_PRIO);
 // sys_thread_new("HTTP", http_server_socket_thread, NULL, DEFAULT_THREAD_STACKSIZE * 2, WEBSERVER_THREAD_PRIO);
}
