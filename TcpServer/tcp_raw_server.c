#include <string.h>
#include "cmsis_os.h"
#include "tcp.h"
#include "sockets.h"
//#include "app.h"

#include "tcp_raw_server.h"
extern osThreadId ethTaskHandle;
extern osMessageQId ethMsgQueueHandle;
//TCP Server�������ݻ�����
#define TCP_RX_BUFSIZE 2000
uint8_t tcp_server_recvbuf[TCP_RX_BUFSIZE];	

/****************************************************************************
* ��    ��: tcp_server_recv(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* ��    �ܣ�TCP���ݽ��պͷ���
* ��ڲ����� 
* ���ز����� 
* ˵    ��������һ���ص���������һ��TCP�ε����������ʱ�ᱻ����       
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
		
		//tcp_recved(pcb, p_temp->tot_len);    //��ȡ���ݳ��� tot_len��tcp���ݿ�ĳ���
		//printf("recived----%d\r\n",p_temp->tot_len);

		memset(tcp_server_recvbuf,0,TCP_RX_BUFSIZE);  //���ݽ��ջ���������
		while(p_temp != NULL)	
		{				
			//errt=tcp_write(pcb,p_temp->payload,p_temp->len,TCP_WRITE_FLAG_COPY);//TCP_WRITE_FLAG_COPY);//|TCP_WRITE_FLAG_MORE); 	// payloadΪTCP���ݿ����ʼλ��       
			//if(err!=ERR_OK) printf("-------------write--%d\r\n",errt);
			//errt=tcp_output(pcb);
			//if(err!=ERR_OK) printf("-------------output--%d\r\n",errt);
				if(q->len > (TCP_SERVER_RX_BUFSIZE-data_len)) memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_SERVER_RX_BUFSIZE-data_len));//��������
				else memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
				data_len += q->len;  	
				if(data_len > TCP_SERVER_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	

			p_temp = p_temp->next;
		}		
		tcp_recved(pcb, eth_msg.len);
		pbuf_free(p); 		 // �ͷŸ�TCP��  
		//xQueueSendToBackFromISR(ethTaskHandle,&eth_msg,&xHigherPriorityTaskWoken );
		//portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
		xReturn=xQueueSendToBack(ethMsgQueueHandle,&eth_msg,0);
		//uint32_t rt=xTaskNotifyGive(ethTaskHandle);
		//printf("queue %d",xReturn);
		
	}
	else
	{
	printf("CLOSE-----------%d\r\n",pcb->remote_port);
		tcp_close(pcb); 	// ��ΪTCP��������Ӧ�����ر��������
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
	if(p!=NULL)  //����������״̬���ҽ��յ������ݲ�Ϊ��ʱ�����ӡ����
	{
		data_len=0;
		memset(tcp_server_recvbuf,0,TCP_RX_BUFSIZE);  //���ݽ��ջ���������

		for(q=p;q!=NULL;q=q->next)  //����������pbuf����
		{
			//tcp_write(pcb,q->payload,q->len,TCP_WRITE_FLAG_COPY); 	//test
			//tcp_output(pcb);
			//�ж�Ҫ������TCP_SERVER_RX_BUFSIZE�е������Ƿ����TCP_SERVER_RX_BUFSIZE��ʣ��ռ䣬�������
			//�Ļ���ֻ����TCP_SERVER_RX_BUFSIZE��ʣ�೤�ȵ����ݣ�����Ļ��Ϳ������е�����
			if(q->len > (TCP_RX_BUFSIZE-data_len)) memcpy(tcp_server_recvbuf+data_len,q->payload,(TCP_RX_BUFSIZE-data_len));//��������
			else memcpy(tcp_server_recvbuf+data_len,q->payload,q->len);
			data_len += q->len;  	
			if(data_len > TCP_RX_BUFSIZE) break; //����TCP�ͻ��˽�������,����	
		}

		//tcp_server_flag|=1<<6;	//��ǽ��յ�������
		//lwipdev.remoteip[0]=tpcb->remote_ip.addr&0xff; 		//IADDR4
		//lwipdev.remoteip[1]=(tpcb->remote_ip.addr>>8)&0xff; //IADDR3
		//lwipdev.remoteip[2]=(tpcb->remote_ip.addr>>16)&0xff;//IADDR2
		//lwipdev.remoteip[3]=(tpcb->remote_ip.addr>>24)&0xff;//IADDR1 
		eth_msg.len=p->tot_len;
		eth_msg.buf=tcp_server_recvbuf;
		eth_msg.pcb=pcb;
		tcp_recved(pcb,p->tot_len);//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
		pbuf_free(p);  	//�ͷ��ڴ�
		xReturn=xQueueSendToBack(ethMsgQueueHandle,&eth_msg,0);
		ret_err=ERR_OK;
	}
	else
	{
		//tcp_recved(pcb,p->tot_len);//���ڻ�ȡ��������,֪ͨLWIP���Ի�ȡ��������
		//es->p=NULL;
		if(p)pbuf_free(p); //�ͷ��ڴ�
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
* ��    ��: tcp_server_accept(void *arg, struct tcp_pcb *pcb,struct pbuf *p,err_t err)
* ��    �ܣ��ص�����
* ��ڲ����� 
* ���ز����� 
* ˵    ��������һ���ص���������һ�������Ѿ�����ʱ�ᱻ����
****************************************************************************/	
static err_t tcp_server_accept(void *arg,struct tcp_pcb *pcb,err_t err)
{
  struct tcp_pcb_listen *lpcb = (struct tcp_pcb_listen*)arg;
  /* Decrease the listen backlog counter */
  //tcp_accepted(lpcb);  //��С�����ۼƼ�����
  /* Set priority */
	tcp_setprio(pcb, TCP_PRIO_MIN); 		/* ���ûص��������ȼ��������ڼ�������ʱ�ر���Ҫ,�˺����������*/
	
	tcp_recv(pcb,tcp_server_recv); 				/* ����TCP�ε�ʱ�Ļص����� */
	tcp_sent(pcb, tcp_server_sent);
	//tcp_err(pcb, Server_conn_err);
	printf("ACCEPT-----------%d\r\n",pcb->remote_port);
	err = ERR_OK;
	return err;
}
/*
tcp_arg(pcb, arg);//��pcb��״̬���ݸ��ص�����
tcp_err(pcb, Server_conn_err); // ????????
tcp_recv(pcb, Server_recv); // ???????????
tcp_sent(pcb, Server_sent); // tcp_write????????????
tcp_poll(pcb, Tcp_Server_Poll, 0); //��ѵ�Ļص�����
gRemoteIp = pcb->remote_ip; // ??????????
return ERR_OK;
*/
/****************************************************************************
* ��    ��: TCP_server_init(void)
* ��    �ܣ����TCP�������ĳ�ʼ������Ҫ��ʹ��TCPͨѶ��������״̬
* ��ڲ����� 
* ���ز����� 
* ˵    ����
****************************************************************************/	
void TCP_server_init(void)
{
	err_t err;  
	struct tcp_pcb *tcppcbnew;  	//����һ��TCP���������ƿ�
	struct tcp_pcb *tcppcbconn;  	//����һ��TCP���������ƿ�
	
//	uint8_t *tbuf;
	uint8_t res=0;		
	
//	tbuf=mymalloc(SRAMIN,200);	//�����ڴ�
//	if(tbuf==NULL)return ;		//�ڴ�����ʧ����,ֱ���˳�
	//sprintf((char*)tbuf,"Server Port:%d",TCP_Server_PORT);//�������˿ں�
	tcppcbnew=tcp_new();	//����һ���µ�pcb
	if(tcppcbnew)			//�����ɹ�
	{ 
		err=tcp_bind(tcppcbnew,IP_ADDR_ANY,TCP_Server_PORT);	//������IP��ָ���Ķ˿ںŰ���һ��,IP_ADDR_ANYΪ�󶨱������е�IP��ַ
		if(err==ERR_OK)	//�����
		{
			tcppcbconn=tcp_listen(tcppcbnew); 			//����tcppcb�������״̬
			tcp_accept(tcppcbconn,tcp_server_accept); 	//��ʼ��LWIP��tcp_accept�Ļص�����
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
	
	p=pbuf_alloc(PBUF_TRANSPORT,msg->len,PBUF_POOL);//�����ڴ�
	pbuf_take(p,msg->buf,msg->len);
		//printf("\r\nlen=%d\r\n",msg->len);
		//printf("num=%d\r\n",pbuf_clen(p));
		for(i=0,q=p;q!=NULL;q=q->next)  //����������pbuf����
		{
			printf("want to send %d\r\n",q->len);
			while(q->len>tcp_sndbuf(msg->pcb))osDelay(1);
			wr_err=tcp_write(msg->pcb,q->payload,q->len,TCP_WRITE_FLAG_COPY); 	//test
			tcp_output(msg->pcb);
			//printf("i=%d\r\n",++i);
			//printf("p=%d ref=%d tot_len=%d len=%d next=%d\r\n",q,q->ref,q->tot_len,q->len,q->next);
			tcp_recved(msg->pcb,q->len); 		//����tcp���ڴ�С
    //osDelay(100);
			
		}
			
		pbuf_free(p); 	//�ͷ��ڴ�	

//		q=p;
	//	printf("after free\r\np=%d ref=%d tot_len=%d len=%d flags=%d next=%d\r\n",q,q->ref,q->tot_len,q->len,q->flags,q->next);
		//if(p) pbuf_free(p); 	//�ͷ��ڴ�	
	

			//tcp_server_senddata(tpcb,es); 		//��ѯ��ʱ����Ҫ���͵�����
	
}
