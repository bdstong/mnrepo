#ifndef _TCP_RAW_SERVER_H_
#define _TCP_RAW_SERVER_H_
#include "cmsis_os.h"



/*********************开发板ip及MAC定义*****************************/
#define IMT407G_IP  			 192,168,0,240   	//开发板ip 
#define IMT407G_NETMASK    255,255,255,0    //开发板子网掩码
#define IMT407G_WG		   	 192,168,0,1   		//开发板子网关
#define IMT407G_MAC_ADDR   0,0,0,0,0,1			//开发板MAC地址


#define TCP_Server_PORT 		8080   // 开发板当服务器时的端口号


void TCP_server_init(void);
void tcp_send_msg(ETH_MSG *msg);

#endif


