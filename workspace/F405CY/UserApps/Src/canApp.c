#include "taskApp.h"

//SN65HVD234 3.3v收发器

//CANbps=APB1Clock/(1+CAN_BS1+CAN_BS2)/CAN_Prescaler
//配置can工作模式
//  	CAN_InitStructure.CAN_Mode= mode;	 //模式设置
//  	CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;	//重新同步跳跃宽度(Tsjw)为tsjw+1个时间单位 CAN_SJW_1tq~CAN_SJW_4tq
//  	CAN_InitStructure.CAN_BS1=CAN_BS1_7tq; //时间段1的时间单元.  Tbs1范围CAN_BS1_1tq ~CAN_BS1_16tq
//  	CAN_InitStructure.CAN_BS2=CAN_BS2_6tq; //时间段2的时间单元.  Tbs2范围CAN_BS2_1tq ~	CAN_BS2_8tq
//  	CAN_InitStructure.CAN_Prescaler=6;  //分频系数(Fdiv)为brp+1


CAN_MSG can_msg;

uint16_t LocalAddr;
uint16_t MasterFlag=0;

CAN_TxHeaderTypeDef     TxMeg;  
CAN_RxHeaderTypeDef     RxMeg;  

//uint32_t get_can_msg(CAN_MSG *msg)
//{
//	osStatus_t status = osMessageQueueGet(canMsgQueueHandle, msg, NULL,portMAX_DELAY);
//	return status;
//}
HAL_StatusTypeDef CAN_Transmit(CAN_HandleTypeDef *hcan,uint32_t StdId,uint32_t ExtId,uint8_t * buf,uint32_t len)
{
	CAN_TxHeaderTypeDef TxMessage;
  TxMessage.DLC=len;
  TxMessage.StdId=StdId;
  TxMessage.ExtId=ExtId;
	
  TxMessage.IDE=CAN_ID_STD;
  //TxMessage.IDE=CAN_ID_EXT;
  
	TxMessage.RTR=CAN_RTR_REMOTE;
  //TxMessage.RTR=CAN_RTR_DATA;
  while(HAL_CAN_GetTxMailboxesFreeLevel(hcan)==0){
	    osDelay(1);
	}
	HAL_StatusTypeDef result;
  result=HAL_CAN_AddTxMessage(hcan,&TxMessage,buf,(uint32_t*)CAN_TX_MAILBOX0);
	return result;
}
HAL_StatusTypeDef CAN1_Transmit(uint32_t StdId,uint32_t ExtId,uint8_t * buf,uint32_t len)
{
	CAN_TxHeaderTypeDef TxMessage;
  TxMessage.DLC=len;
  TxMessage.StdId=StdId;
  TxMessage.ExtId=ExtId;
  TxMessage.IDE=CAN_ID_STD;//CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_DATA;
  
	HAL_StatusTypeDef result;
  result=HAL_CAN_AddTxMessage(&hcan1,&TxMessage,buf,(uint32_t*)CAN_TX_MAILBOX0);
	return result;
}
HAL_StatusTypeDef CanSendData(uint32_t StdId,uint32_t len,uint8_t * buf)
{
	if(MasterFlag==0){
		StdId&=0x7f;
		StdId|=LocalAddr<<7;
	}

	CAN_TxHeaderTypeDef TxMessage;
  TxMessage.DLC=len;
  TxMessage.StdId=StdId;
  TxMessage.ExtId=0;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.RTR=CAN_RTR_DATA;
  
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)==0) osDelay(1);
	HAL_StatusTypeDef result;
  result=HAL_CAN_AddTxMessage(&hcan1,&TxMessage,buf,(uint32_t*)CAN_TX_MAILBOX0);
	return result;
}

HAL_StatusTypeDef CanSendRemote(uint32_t StdId,uint32_t len,uint8_t * buf)
{
	if(MasterFlag==0){
		StdId&=0x7f;
		StdId|=LocalAddr<<7;
	}

	CAN_TxHeaderTypeDef TxMessage;
  TxMessage.DLC=len;
  TxMessage.StdId=StdId;
  TxMessage.ExtId=0;
  TxMessage.IDE=CAN_ID_STD;
  TxMessage.RTR=CAN_RTR_REMOTE;
  
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)==0) osDelay(1);
	HAL_StatusTypeDef result;
  result=HAL_CAN_AddTxMessage(&hcan1,&TxMessage,buf,(uint32_t*)CAN_TX_MAILBOX1);
	return result;
}
HAL_StatusTypeDef CanSendRemoteExd(uint32_t ExtId,uint32_t len,uint8_t * buf)
{
	CAN_TxHeaderTypeDef TxMessage;
  TxMessage.DLC=len;
  TxMessage.StdId=0;
  TxMessage.ExtId=ExtId;
  TxMessage.IDE=CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_REMOTE;
  
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)==0) osDelay(1);
	HAL_StatusTypeDef result;
  result=HAL_CAN_AddTxMessage(&hcan1,&TxMessage,buf,(uint32_t*)CAN_TX_MAILBOX1);
	return result;
}
HAL_StatusTypeDef CanSendDataExd(uint32_t ExtId,uint32_t len,uint8_t * buf)
{

	CAN_TxHeaderTypeDef TxMessage;
  TxMessage.DLC=len;
  TxMessage.StdId=0;
  TxMessage.ExtId=ExtId;
  TxMessage.IDE=CAN_ID_EXT;
  TxMessage.RTR=CAN_RTR_DATA;
  
	while(HAL_CAN_GetTxMailboxesFreeLevel(&hcan1)==0) osDelay(1);
	HAL_StatusTypeDef result;
  result=HAL_CAN_AddTxMessage(&hcan1,&TxMessage,buf,(uint32_t*)CAN_TX_MAILBOX0);
	return result;
}

/*
void CAN2_TRANSMIT(void)
{
  TxMessage2.DLC=8;
  TxMessage2.StdId=0x018;
  TxMessage2.ExtId=0x00002018;
  TxMessage2.IDE=CAN_ID_EXT;
  TxMessage2.RTR=CAN_RTR_DATA;
   uint8_t TR_BUF[8]="87654321";

   if(HAL_CAN_AddTxMessage(&hcan2,&TxMessage2,TR_BUF,(uint32_t*)CAN_TX_MAILBOX0)!=HAL_OK)
  {
     //printf("Failed\n");
  }
  }
*/
void CAN1_Filter_Init_StdID(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  uint16_t tmp;
  
	tmp=LocalAddr<<7;//code 高4位是本机地址
  sFilterConfig.FilterBank = 1;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh =(tmp<<5);//验证码
  sFilterConfig.FilterIdLow =0;
  
	tmp=0xf<<7;//mask 屏蔽码 4位 bit=0 not care
  sFilterConfig.FilterMaskIdHigh =(tmp<<5);
  sFilterConfig.FilterMaskIdLow =0;//|0x02; 		//只接收数据帧
  
  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
void CAN1_Filter_Init_ExdID(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  uint32_t ExtId1 =BROADCAST_EXTID;
  uint32_t ExtId2 =BROADCAST_EXTID;
  
  sFilterConfig.FilterBank = 2;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;

  sFilterConfig.FilterIdHigh = ((ExtId1<<3)>>16)&0xffff;
  sFilterConfig.FilterIdLow = ((ExtId1<<3)&0xffff)|CAN_ID_EXT|CAN_RTR_DATA;

  sFilterConfig.FilterMaskIdHigh = ((ExtId2<<3)>>16)&0xffff;
  sFilterConfig.FilterMaskIdLow = ((ExtId2<<3)&0xffff)|CAN_ID_EXT|CAN_RTR_REMOTE;

  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}

void CAN1_Filter_Init_All_In(void)
{
	CAN_FilterTypeDef  CAN_FilterInitStructure;
   
	//配置过滤器
	CAN_FilterInitStructure.FilterBank=0;	  //过滤器0
	CAN_FilterInitStructure.FilterMode=CAN_FILTERMODE_IDMASK; 
	CAN_FilterInitStructure.FilterScale=CAN_FILTERSCALE_32BIT; //32位
	
	CAN_FilterInitStructure.FilterIdHigh=0x0000;////32位ID
	CAN_FilterInitStructure.FilterIdLow=0x0000;
	
	CAN_FilterInitStructure.FilterMaskIdHigh=0x0000;//32位MASK
	CAN_FilterInitStructure.FilterMaskIdLow=0x0000;
	
	CAN_FilterInitStructure.FilterFIFOAssignment=CAN_FILTER_FIFO0;//过滤器0关联到FIFO0
	CAN_FilterInitStructure.FilterActivation=ENABLE; //激活过滤器
	
	CAN_FilterInitStructure.SlaveStartFilterBank=14;
	
  HAL_CAN_ConfigFilter(&hcan1,&CAN_FilterInitStructure);//滤波器初始化
		
}

/*
static void CANFilterConfig_Scale32_IdList(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  uint32_t StdId =0x321;
  uint32_t ExtId =0x1800f001;
  
  sFilterConfig.FilterBank = 0;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh = StdId<<5;
  sFilterConfig.FilterIdLow = 0|CAN_ID_STD;
  sFilterConfig.FilterMaskIdHigh = ((ExtId<<3)>>16)&0xffff;
  sFilterConfig.FilterMaskIdLow = ((ExtId<<3)&0xffff)|CAN_ID_EXT;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void CANFilterConfig_Scale16_IdList(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  uint32_t StdId1 =0x123;
  uint32_t StdId2 =0x124;
  uint32_t StdId3 =0x125;
  uint32_t StdId4 =0x126;
  
  sFilterConfig.FilterBank = 1;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  sFilterConfig.FilterIdHigh = StdId1<<5;
  sFilterConfig.FilterIdLow = StdId2<<5;
  sFilterConfig.FilterMaskIdHigh = StdId3<<5;
  sFilterConfig.FilterMaskIdLow = StdId4<<5;
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void CANFilterConfig_Scale32_IdMask_StandardIdOnly(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  uint16_t StdIdArray[10] ={0x7e0,0x7e1,0x7e2,0x7e3,0x7e4,
                                0x7e5,0x7e6,0x7e7,0x7e8,0x7e9};
  uint16_t      mask,num,tmp,i;
  
  sFilterConfig.FilterBank = 2;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh =(StdIdArray[0]<<5);//验证码设为数组中的任意一个
  sFilterConfig.FilterIdLow =0;
  
  mask =0x7ff;						//?????????
  num =sizeof(StdIdArray)/sizeof(StdIdArray[0]);
  for(i =0; i<num; i++)		//数组中所有的异或
  {
    tmp =StdIdArray[i] ^ (~StdIdArray[0]);
    mask &=tmp;
  }
  sFilterConfig.FilterMaskIdHigh =(mask<<5);
  sFilterConfig.FilterMaskIdLow =0|0x02; 		//只接收数据帧
  
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void CANFilterConfig_Scale32_IdMask_ExtendIdOnly(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  
uint32_t ExtIdArray[10] ={0x1839f101,0x1835f102,0x1835f113,0x1835f124,0x1835f105,
                            0x1835f106,0x1835f107,0x1835f108,0x1835f109,0x1835f10A};
  uint32_t      mask,num,tmp,i;
  
  sFilterConfig.FilterBank = 3;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
  sFilterConfig.FilterIdHigh =((ExtIdArray[0]<<3) >>16) &0xffff;
  sFilterConfig.FilterIdLow =((ExtIdArray[0]<<3)&0xffff) | CAN_ID_EXT;
  
  mask =0x1fffffff;
  num =sizeof(ExtIdArray)/sizeof(ExtIdArray[0]);
  for(i =0; i<num; i++)	
  {
    tmp =ExtIdArray[i] ^ (~ExtIdArray[0]);
    mask &=tmp;
  }
  mask <<=3;    								//?????
  sFilterConfig.FilterMaskIdHigh = (mask>>16)&0xffff;
  sFilterConfig.FilterMaskIdLow = (mask&0xffff)|0x02;//只接收数据帧
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void CANFilterConfig_Scale32_IdMask_StandardId_ExtendId_Mix(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  //??????CAN ID
uint32_t StdIdArray[10] ={0x711,0x712,0x713,0x714,0x715,
                          0x716,0x717,0x718,0x719,0x71a};
  //????????CAN ID
uint32_t ExtIdArray[10] ={0x1900fAB1,0x1900fAB2,0x1900fAB3,0x1900fAB4,0x1900fAB5,
                            0x1900fAB6,0x1900fAB7,0x1900fAB8,0x1900fAB9,0x1900fABA};
  uint32_t      mask,num,tmp,i,standard_mask,extend_mask,mix_mask;
  
  sFilterConfig.FilterBank = 4;				//?????4
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;		//???????
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;	//??32??
  sFilterConfig.FilterIdHigh =((ExtIdArray[0]<<3) >>16) &0xffff;	//???????CAN  ID?????
  sFilterConfig.FilterIdLow =((ExtIdArray[0]<<3)&0xffff);
  
  standard_mask =0x7ff;		//????????
  num =sizeof(StdIdArray)/sizeof(StdIdArray[0]);
  for(i =0; i<num; i++)			//?????????CAN ID????
  {
    tmp =StdIdArray[i] ^ (~StdIdArray[0]);
    standard_mask &=tmp;
  }
  
  extend_mask =0x1fffffff;
  num =sizeof(ExtIdArray)/sizeof(ExtIdArray[0]);
  for(i =0; i<num; i++)			//?????????CAN ID????
  {
    tmp =ExtIdArray[i] ^ (~ExtIdArray[0]);
    extend_mask &=tmp;
  }
  mix_mask =(StdIdArray[0]<<18)^ (~ExtIdArray[0]);	//?????CAN ID???CAN ID??????
  mask =(standard_mask<<18)& extend_mask &mix_mask;	//??????????
  mask <<=3;    						//?????
 
  sFilterConfig.FilterMaskIdHigh = (mask>>16)&0xffff;
  sFilterConfig.FilterMaskIdLow = (mask&0xffff);
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
  if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
static void CANFilterConfig_Scale16_IdMask(void)
{
  CAN_FilterTypeDef  sFilterConfig;
  uint16_t StdIdArray1[10] ={0x7D1,0x7D2,0x7D3,0x7D4,0x7D5,	//???????CAN ID
                          0x7D6,0x7D7,0x7D8,0x7D9,0x7DA};
  uint16_t StdIdArray2[10] ={0x751,0x752,0x753,0x754,0x755,	//???????CAN ID
                          0x756,0x757,0x758,0x759,0x75A};
  uint16_t      mask,tmp,i,num;
  
  sFilterConfig.FilterBank = 5;
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
  sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
  
  //????????
  sFilterConfig.FilterIdLow =StdIdArray1[0]<<5;			
  mask =0x7ff;
  num =sizeof(StdIdArray1)/sizeof(StdIdArray1[0]);
  for(i =0; i<num; i++)	
  {
    tmp =StdIdArray1[i] ^ (~StdIdArray1[0]);
    mask &=tmp;
  }
  sFilterConfig.FilterMaskIdLow =(mask<<5)|0x10;    //只接收数据帧
  
  //????????
  sFilterConfig.FilterIdHigh = StdIdArray2[0]<<5;
  mask =0x7ff;
  num =sizeof(StdIdArray2)/sizeof(StdIdArray2[0]);
  for(i =0; i<num; i++)					//????????
  {
    tmp =StdIdArray2[i] ^ (~StdIdArray2[0]);
    mask &=tmp;
  }
  sFilterConfig.FilterMaskIdHigh = (mask<<5)|0x10;  //只接收数据帧
  
 
  sFilterConfig.FilterFIFOAssignment = 0;
  sFilterConfig.FilterActivation = ENABLE;
  sFilterConfig.SlaveStartFilterBank = 14;
  
if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
  {
    Error_Handler();
  }
}
uint8_t CANx_SendNormalData(CAN_HandleTypeDef* hcan,uint16_t ID,uint8_t *pData,uint16_t Len)  
{  
    HAL_StatusTypeDef   HAL_RetVal;  
        uint16_t SendTimes,SendCNT=0;  
    uint8_t  FreeTxNum=0;  
    TxMeg.StdId=ID;  
    if(!hcan || ! pData ||!Len)  return 1;  
    SendTimes=Len/8+(Len%8?1:0);  
    FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
    TxMeg.DLC=8;  
    while(SendTimes--){  
        if(0==SendTimes){  
            if(Len%8)  
                TxMeg.DLC=Len%8;  
        }  
        while(0==FreeTxNum){  
            FreeTxNum=HAL_CAN_GetTxMailboxesFreeLevel(&hcan1);  
        }  
        HAL_Delay(1);   //would fail if no delay
        HAL_RetVal=HAL_CAN_AddTxMessage(&hcan1,&TxMeg,pData+SendCNT,(uint32_t*)CAN_TX_MAILBOX0);   
        if(HAL_RetVal!=HAL_OK)  
        {  
            return 2;  
        }  
        SendCNT+=8;  
    }  
      
  return 0;  
}
*/
