#ifndef INC_ALLTYPES_H_
#define INC_ALLTYPES_H_

#include "string.h"
#include "main.h"
#include "cmsis_os2.h"
#include "stdio.h"
#include "parameters.h"
#include "cmsis_os.h"
#include "stdlib.h"
#include "tcp.h"
#include "sockets.h"

//#include "FreeRTOS.h"
//#include "task.h"
//#include "main.h"
//#include "cmsis_os2.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif


//#ifdef __NVIC_PRIO_BITS
//#undef __NVIC_PRIO_BITS
//#define __NVIC_PRIO_BITS      4
//#endif

typedef struct
{
	uint16_t Gain;			//����ֵ����һ��ϵ���������

	uint16_t Kp;
	uint16_t Ki;
	uint16_t Kd;

	float Proportion;		//��������
	float Intergral;		//���ֳ���
	float Derivative;		//΢�ֳ���

	int LastError;			//e(k-1)
	int PrevErroe;			//e(k-2)
	int Increase ;

	int16_t	*pSetPoint;	//�趨Ŀ��
	int16_t *pFeedback;	//������ѹ

	uint16_t Target;
	uint16_t Range;

	uint16_t AbsOfIncrease;

	uint16_t Period;		//��������
	uint16_t PrdCnt;

//	int Uk;		//��ǰ�����
//	int Uk_Max;
//	int Uk_Min;

}MPID;

typedef enum
{
  TYPE_SER,
  TYPE_ESP,
}RxChnTypeDef;

typedef struct
{
	RxChnTypeDef		type;
	uint16_t 			clientID;
	uint16_t 			dataSize;
	uint8_t 			*pData;
	uint8_t 			*pDest;
	UART_HandleTypeDef	*port;
}ReciveDataTypeDef;

#define LED0_T	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_3)
#define LED1_T	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_4)
#define LED2_T	HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_9)

typedef struct
{
	uint16_t cmd;
	uint16_t clientID;
	uint16_t dataSize;
	uint8_t *pData;
}Esp_DataTypeDef;

typedef enum _esp_status
{
  ESP_OK			= 0x01U,
  ESP_ERROR			= 0x02U,
  ESP_SEND_OK		= 0x04U,
  ESP_SEND_READY	= 0x08U,
  ESP_RST_READY		= 0x10U,
}ESP_StatusTypeDef;

typedef struct
{
	int s;
	struct tcp_pcb *pcb;    //Ö¸Ïòµ±Ç°µÄpcb
	uint8_t *buf;
	uint16_t len;
}ETH_MSG;

//typedef struct
//{
//	CAN_HandleTypeDef 	*hcan;
//	CAN_RxHeaderTypeDef	RxHead;
//	uint8_t 			RxBuffer[8];
//}CAN_MSG;

#define BROADCAST_EXTID	0x12345678

#define USART_BUF_SIZE	256

typedef struct
{
	UART_HandleTypeDef	*UsartHandle;
	uint8_t 	cmd_rcv_flag;
	uint16_t 	rx_len;
	uint8_t		RxBuf[USART_BUF_SIZE];
	uint16_t 	tx_len;
	uint8_t		TxBuf[USART_BUF_SIZE];
}USART_DATA;

#define LED0_T	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_3)
#define LED1_T	HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_4)
#define LED2_T	HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_9)

#define LED0_ON		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)
#define LED0_OFF	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET)

#define LED1_ON		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET)
#define LED1_OFF	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET)

#define LED2_ON		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET)
#define LED2_OFF	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET)


#define WIFI_ERROR				0x01
#define TEMP_ERROR				0X02
#define I2C_ERROR				0X04

extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
//extern osThreadId_t usart1TaskHandle;
//extern osThreadId_t usart3TaskHandle;
//extern osThreadId_t defaultTaskHandle;
//extern osMessageQueueId_t espCmdQueueHandle;
//extern TIM_HandleTypeDef htim14;

//extern osThreadId_t adcTaskHandle;
//extern DMA_HandleTypeDef hdma_memtomem_dma2_stream1;
extern I2C_HandleTypeDef hi2c1;
extern osMessageQueueId_t canMsgQueueHandle;
extern osThreadId_t usart1TaskHandle;
extern osMessageQueueId_t ethMsgQueueHandle;

//extern TIM_HandleTypeDef htim2;
//extern ADC_HandleTypeDef hadc1;
//extern TIM_HandleTypeDef htim3;
//extern TIM_HandleTypeDef htim4;
//extern DAC_HandleTypeDef hdac;
//extern CAN_HandleTypeDef hcan1;

void UART3_IDLECallBack(void);
void UART1_IDLECallBack(void);
void InitMyApp();
void CAN1_Filter_Init_All_In(void);

void Write_All_Parameters(void);
void Read_All_Parameters(void);
//void ParseRxData(ReciveDataTypeDef *pRx);
//void CanMsgProc(CAN_MSG *msg);

void ParseUsartModBusData(USART_DATA *pRx);
//void CanMsgProc(CAN_MSG *msg);
//int ParseModeBusData(uint8_t * pData,uint16_t dataSize,uint8_t *pDstBuf);


uint16_t CRC16(uint8_t *data,uint16_t len);
HAL_StatusTypeDef CanSendData(uint32_t StdId, uint32_t len, uint8_t *buf);

void tcp_socket_svr_init(void);
//CAN
#define DATA_OF_MASTER		0x12
#define DATA_OF_SLAVE		0x40
extern osThreadId_t ethTaskHandle;
//01 03 03 E8 00 64 C4 51
//02 03 03 E8 00 64 C4 62

//01 04 03 E8 00 24 70 61
//02 04 03 E8 00 24 70 52

#endif /* INC_ALLTYPES_H_ */
