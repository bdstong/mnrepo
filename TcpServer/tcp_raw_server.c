#include <string.h>
#include "cmsis_os.h"
#include "tcp.h"
#include "sockets.h"
//#include "app.h"

#include "tcp_raw_server.h"
extern osThreadId ethTaskHandle;
extern osMessageQId ethMsgQueueHandle;
//TCP Server接收数据缓冲区
#define TCP_RX_BUFSIZE 2000
uint8_t tcp_server_recvbuf[TCP_RX_BUFSIZE];	

/****************************************************************************
* 名    称: tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* 功    能：TCP数据接收和发送
* 入口参数： 
* 返回参数： 
* 说    明：这是一个回调函数，当一个TCP段到达这个连接时会被调用       
static err_t tcp_server_recv2(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	BaseType_t xReturn;
	ETH_MSG eth_msg;
	struct pbuf *p_temp = p;
	struct pbuf *q;
	err_t errt;
	uint32_t data_len = 0;
	
	if(p_temp != NULL)
	{	
		eth_msg.len=p_temp->tot_len;
		
		//tcp_recved(pcb, p_temp->tot_len);    //获取数据长度 tot_len：tcp数据块的长度
		//printf("recived----%d\r\n",p_temp->tot_len);

		memset(tcp_server_recvbuf,0,TCP_RX_BUFSIZE);  //数据接收缓冲区清零
		while(p_temp != NULL)	
		{				
			//errt=tcp_write(pcb,p_temp->payload,p_temp->len,TCP_WRITE_FLAG_COPY);//TCP_WRITE_FLAG_COPY);//|TCP_WRITE_FLAG_MORE); 	// payload为TCP数据块的起始位置       
			//if(err!=ERR_OK) printf("-------------write--%d\r\n",errt);
			//errt=tcp_output(pcb);
			//if(err!=ERR_OK) printf("-------------output--%d\r\n",errt);
				if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//拷贝数据
				else memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
				data_len += q->len;  	
				if(data_len > TCP_SERVER_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	

			p_temp = p_temp->next;
		}		
		tcp_recved(pcb, eth_msg.len);
		pbuf_free(p); 		 // 释放该TCP段  
		//xQueueSendToBackFromISR(ethTaskHandle,&eth_msg,&xHigherPriorityTaskWoken );
		//portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		xReturn=xQueueSendToBack(ethMsgQueueHandle,&eth_msg,0);
		//uint32_t rt=xTaskNotifyGive(ethTaskHandle);
		//printf("queue %d",xReturn);
		
	}
	else
	{
	printf("CLOSE-----------%d\r\n",pcb->remote_port);
		tcp_close(pcb); 	// 作为TCP服务器不应主动关闭这个连接
	}
	err = ERR_OK;
	return err;
}
****************************************************************************/	
static err_t tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
{
	err_t ret_err;
	uint32_t data_len;
	struct pbuf *q;
 	ETH_MSG eth_msg;
	BaseType_t xReturn;
 	
	//struct tcp_server_struct *es;
	//LWIP_ASSERT("arg != NULL",arg != NULL);
	//es=(struct tcp_server_struct *)arg;
	if(p!=NULL)  //当处于连接状态并且接收到的数据不为空时将其打印出来
	{
		data_len=0;
		memset(tcp_server_recvbuf,0,TCP_RX_BUFSIZE);  //数据接收缓冲区清零

		for(q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
		{
			//tcp_write(pcb,q->payload,q->len,TCP_WRITE_FLAG_COPY); 	//test
			//tcp_output(pcb);
			//判断要拷贝到TCP_SERVER_RX_BUFSIZE中的数据是否大于TCP_SERVER_RX_BUFSIZE的剩余空间，如果大于
			//的话就只拷贝TCP_SERVER_RX_BUFSIZE中剩余长度的数据，否则的话就拷贝所有的数据
			if(q->len > (TCP_RX_BUFSIZE-data_len)) memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_RX_BUFSIZE-data_len));//拷贝数据
			else memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
			data_len += q->len;  	
			if(data_len > TCP_RX_BUFSIZE) break; //超出TCP客户端接收数组,跳出	
		}

		//tcp_server_flag|=1<<6;	//标记接收到数据了
		//lwipdev.remoteip[0]=tpcb->remote_ip.addr&0xff; 		//IADDR4
		//lwipdev.remoteip[1]=(tpcb->remote_ip.addr>>8)&0xff; //IADDR3
		//lwipdev.remoteip[2]=(tpcb->remote_ip.addr>>16)&0xff;//IADDR2
		//lwipdev.remoteip[3]=(tpcb->remote_ip.addr>>24)&0xff;//IADDR1 
		eth_msg.len=p->tot_len;
		eth_msg.buf=tcp_server_recvbuf;
		eth_msg.pcb=pcb;
		tcp_recved(pcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
		pbuf_free(p);  	//释放内存
		xReturn=xQueueSendToBack(ethMsgQueueHandle,&eth_msg,0);
		ret_err=ERR_OK;
	}
	else
	{
		//tcp_recved(pcb,p->tot_len);//用于获取接收数据,通知LWIP可以获取更多数据
		//es->p=NULL;
		if(p)pbuf_free(p); //释放内存
		tcp_close(pcb);
		ret_err=ERR_OK;
	}
	return ret_err;
}
static err_t tcp_server_sent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	printf("sent-----------%d\r\n",len);
	return ERR_OK;
}
static err_t Server_conn_err(void *arg, err_t err)
{
	printf("err-----------%d\r\n",err);
	return ERR_OK;
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
  struct tcp_pcb_listen *lpcb = (struct tcp_pcb_listen*)arg;
  /* Decrease the listen backlog counter */
  //tcp_accepted(lpcb);  //减小监听累计计数器
  /* Set priority */
	tcp_setprio(pcb, TCP_PRIO_MIN); 		/* 设置回调函数优先级，当存在几个连接时特别重要,此函数必须调用*/
	
	tcp_recv(pcb,tcp_server_recv); 				/* 设置TCP段到时的回调函数 */
	tcp_sent(pcb, tcp_server_sent);
	//tcp_err(pcb, Server_conn_err);
	printf("ACCEPT-----------%d\r\n",pcb->remote_port);
	err = ERR_OK;
	return err;
}
/*
tcp_arg(pcb, arg);//将pcb的状态传递给回调函数
tcp_err(pcb, Server_conn_err); // ????????
tcp_recv(pcb, Server_recv); // ???????????
tcp_sent(pcb, Server_sent); // tcp_write????????????
tcp_poll(pcb, Tcp_Server_Poll, 0); //轮训的回调函数
gRemoteIp = pcb->remote_ip; // ??????????
return ERR_OK;
*/
/****************************************************************************
* 名    称: TCP_server_init(void)
* 功    能：完成TCP服务器的初始化，主要是使得TCP通讯快进入监听状态
* 入口参数： 
* 返回参数： 
* 说    明：
****************************************************************************/	
void TCP_server_init(void)
{
	err_t err;  
	struct tcp_pcb *tcppcbnew;  	//定义一个TCP服务器控制块
	struct tcp_pcb *tcppcbconn;  	//定义一个TCP服务器控制块
	
//	uint8_t *tbuf;
	uint8_t res=0;		
	
//	tbuf=mymalloc(SRAMIN,200);	//申请内存
//	if(tbuf==NULL)return ;		//内存申请失败了,直接退出
	//sprintf((char*)tbuf,"Server Port:%d",TCP_Server_PORT);//服务器端口号
	tcppcbnew=tcp_new();	//创建一个新的pcb
	if(tcppcbnew)			//创建成功
	{ 
		err=tcp_bind(tcppcbnew,IP_ADDR_ANY,TCP_Server_PORT);	//将本地IP与指定的端口号绑定在一起,IP_ADDR_ANY为绑定本地所有的IP地址
		if(err==ERR_OK)	//绑定完成
		{
			tcppcbconn=tcp_listen(tcppcbnew); 			//设置tcppcb进入监听状态
			tcp_accept(tcppcbconn,tcp_server_accept); 	//初始化LWIP的tcp_accept的回调函数
		}else res=1;  
	}else res=1;

}
void tcp_send_msg(ETH_MSG *msg)
{
int i;
	struct pbuf *p,*q;
	err_t wr_err=ERR_OK;

	//tcp_write(msg->pcb,msg->buf,msg->len,TCP_WRITE_FLAG_COPY);
	//		tcp_output(msg->pcb);
	//return;
	
	p=pbuf_alloc(PBUF_TRANSPORT,msg->len,PBUF_POOL);//申请内存
	pbuf_take(p,msg->buf,msg->len);
		//printf("\r\nlen=%d\r\n",msg->len);
		//printf("num=%d\r\n",pbuf_clen(p));
		for(i=0,q=p;q!=NULL;q=q->next)  //遍历完整个pbuf链表
		{
			printf("want to send %d\r\n",q->len);
			while(q->len>tcp_sndbuf(msg->pcb))osDelay(1);
			wr_err=tcp_write(msg->pcb,q->payload,q->len,TCP_WRITE_FLAG_COPY); 	//test
			tcp_output(msg->pcb);
			//printf("i=%d\r\n",++i);
			//printf("p=%d ref=%d tot_len=%d len=%d next=%d\r\n",q,q->ref,q->tot_len,q->len,q->next);
			tcp_recved(msg->pcb,q->len); 		//更新tcp窗口大小
    //osDelay(100);
			
		}
			
		pbuf_free(p); 	//释放内存	

//		q=p;
	//	printf("after free\r\np=%d ref=%d tot_len=%d len=%d flags=%d next=%d\r\n",q,q->ref,q->tot_len,q->len,q->flags,q->next);
		//if(p) pbuf_free(p); 	//释放内存	
	

			//tcp_server_senddata(tpcb,es); 		//轮询的时候发送要发送的数据
	
}
