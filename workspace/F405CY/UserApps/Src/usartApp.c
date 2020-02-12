#include "TaskApp.h"

USART_DATA UsartType3;  
USART_DATA UsartType1;  

uint16_t SlaveDataFlag;	//���յ��ӻ��������ݰ��ı�־
uint16_t SlaveDataLen;	//���յ��ӻ����ݰ��ĳ���
uint8_t SlaveDataBuf[256];
uint8_t MasterDataBuf[256];	//�洢�������͵�����
uint16_t MasterDataLen;
uint8_t CanBuffer[256];

extern uint16_t LocalAddr;
extern uint16_t MasterFlag;

PUTCHAR_PROTOTYPE {
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	return ch;
}

/*
 void uartSendBuf(char *buf,int size)
 {
 static UART_MSG Msg;
 UART_MSG* pMsg;
 pMsg=&Msg;

 pMsg->buf=buf;
 pMsg->size=size;
 xQueueSend( xUartQueueHandle, &pMsg, 0 );
 }
 void uartSendStr(char *buf)
 {
 uartSendBuf(buf,strlen(buf));
 }
 void uartSendMsg(UART_MSG *msg)
 {
 xQueueSend( xUartQueueHandle, &msg, 0 );
 }
 */

/*
 void print_adc(void)
 {
 //printf("adc---------%d\r\n",cd);
 for(int j=0;j<ADC_NUMBER;j++)
 {
 float adv=(float)AllSampleBuf[j]/4095*3.3;
 printf("adc%d=%.3f\t",j,adv);
 }
 printf("\r\n");
 }
 void print_pl(void)
 {
 for(int j=0;j<FREQ_NUMBER;j++)
 {
 printf("pl%d=%d\t",j,AllSampleBuf[FREQ_INDEX+j]);
 }
 printf("\r\n");
 }
 void print_id()
 {
 char *p=(char*)	Get_ChipID;

 for(int j=0;j<12;j++)
 {
 printf("%02x\t",p[j]);
 }
 printf("\r\n");
 }
 */

//void ParseEspRxData(uint8_t *pdata, ReciveDataTypeDef *pesp) {
//	char *head, *clientID, *dataSize, *dataBuffer;
//	UNUSED(head);
//	head = strtok((char*) pdata, ",");		//+IPD
//	clientID = strtok(NULL, ",");		//0
//	dataSize = strtok(NULL, ":");		//5
//	dataBuffer = strtok(NULL, "");		//12345
//
//	pesp->clientID = atoi(clientID);
//	pesp->dataSize = atoi(dataSize);
//	pesp->pData = (uint8_t*) dataBuffer;
//}

void SendBufByUsart(USART_DATA *pRx, uint8_t *buf, int len) {
	HAL_UART_Transmit(pRx->UsartHandle, buf, len, HAL_MAX_DELAY);
}
void SendUsartData(USART_DATA *pRx) {
	HAL_UART_Transmit(pRx->UsartHandle, pRx->TxBuf, pRx->tx_len, HAL_MAX_DELAY);
}
//CAN 分批发送 buf中的数据
void SendBufByCan(int DataID, uint8_t *pBuf, int len) 
{
	uint8_t buf[8];
	int i, j;
	int cnt = len / 6;

	j = 0;
	for (i = 0; i < cnt; i++) {

		buf[0] = j;
		buf[1] = j >> 8;		//offset index

		buf[2] = pBuf[j];
		buf[3] = pBuf[j + 1];
		buf[4] = pBuf[j + 2];
		buf[5] = pBuf[j + 3];
		buf[6] = pBuf[j + 4];
		buf[7] = pBuf[j + 5];
		j += 6;
		CanSendData(DataID, 8, buf);
		osDelay(1);
	}
	if (len % 6) {
		i = 0;
		buf[i++] = j;
		buf[i++] = j >> 8;		//offset index
		for (; j < len; j++) {
			buf[i++] = pBuf[j];
		}
		CanSendData(DataID, i, buf);
		osDelay(1);
	}
	CanSendData(DataID, 0, buf);
}

int SendDataToSlaveByCan(uint8_t StationNum, uint8_t *pData, uint16_t dataSize,int timeout) 
{
	int StdId = (DATA_OF_MASTER & 0x7f) | (StationNum << 7);
	SendBufByCan(StdId, pData, dataSize);
	SlaveDataFlag = 0;
	SlaveDataLen = 0;
	int i;

	for (i = 0; i < timeout; i++) 
	{
		osDelay(1);
		if (SlaveDataFlag)break;
	}
	return i;
}
//接收的CAN数据帧保存到DstBuf ,msg buf前2个字节是偏印地址
int SaveCanFrmaeToBuf(CAN_MSG *msg, uint8_t *DstBuf) {
	int len = msg->RxHead.DLC;
	int i;
	int addr = msg->RxBuffer[0];
	addr += (msg->RxBuffer[1] << 8);
	for (i = 0; i < len - 2; i++) {
		DstBuf[addr++] = msg->RxBuffer[i + 2];
	}
	return addr;
}
void CanMsgProc(CAN_MSG *msg) 
{
	if(msg->RxHead.ExtId)
	{
		if(msg->RxHead.RTR)//==1)
		{
			switch(msg->RxHead.ExtId)
			{
				case BROADCAST_EXTID:
					HAL_NVIC_SystemReset();
					break;
				default:
					break;
			}
		}
		return;
	}
//	int which = msg->RxHead.StdId >> 7;		//station num

	int DataID = msg->RxHead.StdId & 0x7f;

	if(msg->RxHead.RTR==1)//Զ��֡
	{
//		if(MasterFlag==0)//����Ǵӻ�
//		{
//			switch(DataID)
//			{
//				case REMOTE_SAMPLE_DATA:
//					CanSendAllInfo();
//					break;
//				case REMOTE_PARAMETER:
//					//Read_All_Parameters();
//					CanSendParameter();
//					break;
//				default:
//					break;
//			}
//		}
	}
	else//DATA
	{
		switch (DataID) 
		{
			//work as master
			case DATA_OF_SLAVE:			//���մӻ�������
				if (msg->RxHead.DLC == 0)			//������һ�����ݰ�
				{
					SlaveDataFlag = 1;
				} 
				else									//���δ洢�����ְ�������
				{
					SlaveDataLen = SaveCanFrmaeToBuf(msg, SlaveDataBuf);
				}
				break;

			//work as slave
			case DATA_OF_MASTER:			//��������ת������λ������
				if(MasterFlag) break;
				if(msg->RxHead.DLC==0)	//���յ����������ݰ�
				{
					int size =ParseModeBusData(MasterDataBuf,MasterDataLen,CanBuffer);
					//�����������
					SendBufByCan(DATA_OF_SLAVE,(uint8_t*)CanBuffer,size);
				}
				else										//�ְ��洢
				{
					MasterDataLen=SaveCanFrmaeToBuf(msg,MasterDataBuf);
				}
				break;
			default:
				break;
		}
	}
}

void ParseUsartModBusData(USART_DATA *pUsartData) {
	if (CRC16(pUsartData->RxBuf, pUsartData->rx_len) != 0) //CRCУ�����
		return;
	uint8_t StationNum, FunctionCode;
	uint16_t Register, regNumber;
	StationNum = pUsartData->RxBuf[0];		//��ַ��
	FunctionCode = pUsartData->RxBuf[1];	//������
	Register = pUsartData->RxBuf[2] << 8;	//�Ĵ�����ַ 16bit
	Register |= pUsartData->RxBuf[3];
	regNumber = pUsartData->RxBuf[4] << 8;
	regNumber |= pUsartData->RxBuf[5];

//		uint8_t pdata[2];//={0x12,0x34};
//		pdata[0]=(uint8_t)StationNum;
//		pdata[1]=(uint8_t)LocalAddr;
//		HAL_UART_Transmit(&huart1, pdata, 2,HAL_MAX_DELAY);
//	LED2_T;

	if(StationNum==LocalAddr)//Èç¹ûÊÇ±¾»úµØÖ·
	{
		pUsartData->tx_len=ParseModeBusData(pUsartData->RxBuf,pUsartData->rx_len,pUsartData->TxBuf);
		SendUsartData(pUsartData);
		return;
	}

	if(MasterFlag==0) return;//Èç¹û²»ÊÇÖ÷»ú¾Í·µ»Ø

	if(StationNum==0)							//�㲥��ַ
	{
		if(Register==ResetRegister && FunctionCode==WriteSingleRegister)	//��λϵͳ�Ĵ���
		{
			SendBufByUsart(pUsartData, pUsartData->RxBuf, pUsartData->rx_len);
			CanSendRemoteExd(BROADCAST_EXTID,0,0);//�㲥��λ
			osDelay(10);
			HAL_NVIC_SystemReset();
			return;
		}
	}

	//������ת�����ӻ�
	int timeout = 100;
	int ret = SendDataToSlaveByCan(StationNum, pUsartData->RxBuf, pUsartData->rx_len,timeout);//35
//	printf("paras time=%d\n",ret);
	if (ret < timeout)
	{
		SendBufByUsart(pUsartData, SlaveDataBuf, SlaveDataLen);
	} 
	else 
	{
	//printf("timeout=%d\n",timeout);
	}
}
