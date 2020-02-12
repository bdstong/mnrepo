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
#include "api.h"


#include "tcp.h"
#include "tcp_thread_svr.h"
#include "sockets.h"
//#include "app.h"
char Dbuf[1024];
#include "tcp_raw_server.h"
extern osThreadId ethTaskHandle;
extern osMessageQId ethMsgQueueHandle;
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
static void tcp_rcv_thread(void *arg)
{
  struct netconn *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
  void *data;
  u16_t len;
  err_t recv_err;
	printf("start tcp \r\n");
     
  //LWIP_UNUSED_ARG(arg);
if(arg!=NULL)
{//    osDelay(1000);
	newconn=arg;
	printf("netconn_recv %d %d\r\n",newconn->state,newconn->pcb.tcp->remote_port);
	

	//start new thread
          recv_err = netconn_recv(newconn, &buf);
					while ( recv_err == ERR_OK) 
          {
            do 
            {
              netbuf_data(buf, &data, &len);
              netconn_write(newconn, data, len, NETCONN_COPY);
          
            } 
            while (netbuf_next(buf) >= 0);
          
            netbuf_delete(buf);
						recv_err = netconn_recv(newconn, &buf);
          }
	printf("netconn_delete %d %d\r\n",newconn->state,newconn->pcb.tcp->remote_port);
        
          // Close connection and discard connection identifier.
          netconn_close(newconn);
          netconn_delete(newconn);
				printf("netconn closed\r\n");

}
else
{
	printf("nothing is done\r\n");
}
vTaskDelete(NULL);
//taskYIELD();
}

static void tcp_listen_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err, accept_err;
  struct netbuf *buf;
  void *data;
  u16_t len;
  err_t recv_err;
 	ETH_MSG eth_msg;
	BaseType_t xReturn;

	printf("start tcp \r\n");
     
  LWIP_UNUSED_ARG(arg);

  /* Create a new connection identifier. */
  conn = netconn_new(NETCONN_TCP);

	printf("start connecting \r\n");
  
  if (conn!=NULL)
  {  
    /* Bind connection to well known port number 7. */
    err = netconn_bind(conn, NULL, 8080);
printf("start binding \r\n");
    
    if (err == ERR_OK)
    {
      /* Tell connection to go into listening mode. */
      netconn_listen(conn);
printf("start listening \r\n");
    
      while (1) 
      {
        /* Grab new connection. */
         accept_err = netconn_accept(conn, &newconn);
    
        /* Process the new connection. */
        if (accept_err == ERR_OK) 
        {
 //printf("ACCEPT \r\n");
//uint32_t tp=xTaskCreate(tcp_rcv_thread, (int8_t *) "t1", configMINIMAL_STACK_SIZE,newconn, TCPIP_THREAD_PRIO, NULL);
//  sys_thread_t tp=sys_thread_new("tcpecho_thread", tcp_rcv_thread, newconn, 128, TCPIP_THREAD_PRIO);//TCPECHO_THREAD_PRIO);
//					printf("xTaskCreate1 %d \r\n",tp);
//					if(tp==NULL)
//					{
	//					netconn_close(newconn);
		//				netconn_delete(newconn);
			//			printf("netconn closed\r\n");
				//	}
//tp=xTaskCreate(tcp_rcv_thread, (int8_t *) "t2", configMINIMAL_STACK_SIZE,NULL, TCPIP_THREAD_PRIO, NULL);
  //sys_thread_new("tcpecho_thread", tcp_rcv_thread, newconn, 512, TCPIP_THREAD_PRIO);//TCPECHO_THREAD_PRIO);
	//				printf("xTaskCreate2 %d \r\n",tp);

					printf("accept %d \r\n",newconn->pcb.tcp->remote_port);
					
					//start new thread
          recv_err = netconn_recv(newconn, &buf);
					while ( recv_err == ERR_OK) 
          {
            do 
            {
              netbuf_data(buf, &data, &len);
              //netconn_write(newconn, data, len, NETCONN_COPY);
 		eth_msg.len=len;
		xReturn=xQueueSendToBack(ethMsgQueueHandle,&eth_msg,0);
         
            } 
            while (netbuf_next(buf) >= 0);
          
            netbuf_delete(buf);
						recv_err = netconn_recv(newconn, &buf);
          }
        
          // Close connection and discard connection identifier.
          netconn_close(newconn);
          netconn_delete(newconn);
				printf("netconn closed\r\n");

}
				else
				{
      printf("accept_err=%d\r\n",accept_err);
				}
      }
    }
    else
    {
      netconn_delete(newconn);
      printf(" can not bind TCP netconn %d\r\n",err);
    }
  }
  else
  {
    printf("can not create TCP netconn conn=%d\r\n",conn);
  }
}
void tcp_thread_init(void)//
{
  sys_thread_new("tcpecho_thread", tcp_listen_thread, NULL, 512, TCPIP_THREAD_PRIO);//TCPECHO_THREAD_PRIO);

}

