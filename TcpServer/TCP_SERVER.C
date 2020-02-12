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
**********************MCU���� STM32F407Ӧ�ÿ�����(�����)*************************
**********************************************************************************
* �ļ�����: TCP_SERVER.c                                                         *
* �ļ�������TCP����������                                                        *
* �������ڣ�2015.10.08                                                           *
* ��    ����V1.0                                                                 *
* ��    �ߣ�Clever                                                               *
* ˵    ����                                                                     * 
* ��    ���������̴��������ѧϰ�ο�                                             *
**********************************************************************************
*********************************************************************************/


/****************************************************************************
* ��    ��: tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* ��    �ܣ�TCP���ݽ��պͷ���
* ��ڲ����� 
* ���ز����� 
* ˵    ��������һ���ص���������һ��TCP�ε����������ʱ�ᱻ����       
****************************************************************************/	
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	struct pbuf *p_temp = p;
	
	if(p_temp != NULL)
	{	
		tcp_recved(pcb, p_temp->tot_len);    //��ȡ���ݳ��� tot_len��tcp���ݿ�ĳ���
		while(p_temp != NULL)	
		{				
			tcp_write(pcb,p_temp->payload,p_temp->len,TCP_WRITE_FLAG_COPY); 	// payloadΪTCP���ݿ����ʼλ��       
			tcp_output(pcb);
			p_temp = p_temp->next;
		}		
	}
	else
	{
		tcp_close(pcb); 	// ��ΪTCP��������Ӧ�����ر��������
	}
	pbuf_free(p); 		 // �ͷŸ�TCP��  
	err = ERR_OK;
	return err;
}

/****************************************************************************
* ��    ��: tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* ��    �ܣ��ص�����
* ��ڲ����� 
* ���ز����� 
* ˵    ��������һ���ص���������һ�������Ѿ�����ʱ�ᱻ����
****************************************************************************/	
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
	tcp_setprio(pcb, TCP_PRIO_NORMAL);//TCP_PRIO_MIN); 		/* ���ûص��������ȼ��������ڼ�������ʱ�ر���Ҫ,�˺����������*/
	tcp_recv(pcb,tcp_server_recv); 				/* ����TCP�ε�ʱ�Ļص����� */
	err = ERR_OK;
	return err;
}

/****************************************************************************
* ��    ��: TCP_server_init(void)
* ��    �ܣ����TCP�������ĳ�ʼ������Ҫ��ʹ��TCPͨѶ��������״̬
* ��ڲ����� 
* ���ز����� 
* ˵    ����
****************************************************************************/	
void TCP_server_init(void)
{
	struct tcp_pcb *pcb;

	pcb = tcp_new(); 								              // ����ͨ�ŵ�TCP���ƿ�(pcb) 
	tcp_bind(pcb,IP_ADDR_ANY,TCP_Server_PORT); 	  // �󶨱���IP��ַ�Ͷ˿ںţ���Ϊtcp��������  
	pcb = tcp_listen(pcb); 							          // �������״̬  
	tcp_accept(pcb,tcp_server_accept); 			      // ��������������ʱ�Ļص�����  
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