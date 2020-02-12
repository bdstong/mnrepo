#ifndef _TCP_RAW_SERVER_H_
#define _TCP_RAW_SERVER_H_
#include "cmsis_os.h"



/*********************������ip��MAC����*****************************/
#define IMT407G_IP  			 192,168,0,240   	//������ip 
#define IMT407G_NETMASK    255,255,255,0    //��������������
#define IMT407G_WG		   	 192,168,0,1   		//������������
#define IMT407G_MAC_ADDR   0,0,0,0,0,1			//������MAC��ַ


#define TCP_Server_PORT 		8080   // �����嵱������ʱ�Ķ˿ں�


void TCP_server_init(void);
void tcp_send_msg(ETH_MSG *msg);

#endif


