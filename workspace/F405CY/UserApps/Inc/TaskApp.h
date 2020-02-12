#ifndef TASK_APP_H_
#define TASK_APP_H_

#include "string.h"
#include "main.h"
#include "cmsis_os2.h"
#include "stdio.h"
#include "parameters.h"
#include "cmsis_os.h"
#include "stdlib.h"
//#include "tcp.h"
//#include "sockets.h"

//#include "FreeRTOS.h"
//#include "task.h"
//#include "main.h"
//#include "cmsis_os2.h"
#include "arm_math.h"
#include "arm_const_structs.h"

#define NPT 32          /* NPT = No of FFT point*/

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif


#ifdef __NVIC_PRIO_BITS
#undef __NVIC_PRIO_BITS
#define __NVIC_PRIO_BITS      4
#endif

/*
#define SPI_CS_0   	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET)
#define SPI_CS_1   	HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET)
#define SPI_CLK_0		HAL_GPIO_WritePin(SPI_CLK_GPIO_Port,SPI_CLK_Pin,GPIO_PIN_RESET)
#define SPI_CLK_1   HAL_GPIO_WritePin(SPI_CLK_GPIO_Port,SPI_CLK_Pin,GPIO_PIN_SET)
#define SPI_IN			HAL_GPIO_ReadPin(SPI_SI_GPIO_Port,SPI_SI_Pin)
*/

//typedef struct
//{
//	uint16_t			CS_Pin;
//	GPIO_TypeDef*	CS_Port;
//	uint16_t			CLK_Pin;
//	GPIO_TypeDef*	CLK_Port;
//	uint16_t			DATA_Pin;
//	GPIO_TypeDef*	DATA_Port;
//	uint16_t			Result;
//}MAX6675_TypeDef;

//typedef struct
//{
//	uint16_t			CS_Pin;
//	GPIO_TypeDef*	CS_Port;
//	uint16_t			CLK_Pin;
//	GPIO_TypeDef*	CLK_Port;
//	uint16_t			DATA_Pin;
//	GPIO_TypeDef*	DATA_Port;
//	uint32_t			Result;
//}MAX31855_TypeDef;

//typedef struct
//{
//	int 						s;
//	struct tcp_pcb	*pcb;    //Ö¸Ïòµ±Ç°µÄpcb
//	uint8_t					*buf;
//	uint16_t				len;
//}ETH_MSG;

typedef struct
{
	uint16_t	Gain;			//����ֵ����һ��ϵ���������

	uint16_t	Kp;
	uint16_t	Ki;
	uint16_t	Kd;

	float 		Proportion;		//��������
	float		Intergral;		//���ֳ���
	float		Derivative;		//΢�ֳ���

	int			LastError;			//e(k-1)
	int			PrevErroe;			//e(k-2)
	int			Increase ;

	int16_t		*pSetPoint;	//�趨Ŀ��
	int16_t		*pFeedback;	//������ѹ

	uint16_t	Target;
	uint16_t	Range;

	uint16_t	AbsOfIncrease;

	uint16_t	Period;		//��������
	uint16_t	PrdCnt;

//	int Uk;		//��ǰ�����
//	int Uk_Max;
//	int Uk_Min;

}MPID;

typedef struct
{
	int16_t 	*pCurrent;
	int16_t 	*pTarget;
	int16_t 	TargetValue;
	int16_t 	range;

	uint8_t 	normalTime;
	uint8_t 	reverseTime;
	int16_t 	rstDelayTime;

	uint16_t 	period;
	uint16_t 	period_cnt;

	uint8_t 	rstCmdChn;
	uint8_t 	norCmdChn;
	uint8_t 	revCmdChn;

	uint8_t 	cmd;

}MOTOR;

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
}ESP_MSG;

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
	CAN_HandleTypeDef	*hcan;
	CAN_RxHeaderTypeDef	RxHead;
	uint8_t				RxBuffer[8];
}CAN_MSG;

#define USART_BUF_SIZE	256
typedef struct
{
	UART_HandleTypeDef	*UsartHandle;
	uint8_t				cmd_rcv_flag;
	uint16_t			rx_len;
	uint8_t				RxBuf[USART_BUF_SIZE];
	uint16_t			tx_len;
	uint8_t				TxBuf[USART_BUF_SIZE];
}USART_DATA;

#define LED_T		HAL_GPIO_TogglePin(LED_GPIO_Port,LED_Pin)
#define LED_ON		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_RESET)
#define LED_OFF		HAL_GPIO_WritePin(LED_GPIO_Port,LED_Pin,GPIO_PIN_SET)

#define LED0_T		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_3)
#define LED0_ON		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_RESET)
#define LED0_OFF	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_3,GPIO_PIN_SET)

#define LED1_T		HAL_GPIO_TogglePin(GPIOE,GPIO_PIN_4)
#define LED1_ON		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_RESET)
#define LED1_OFF	HAL_GPIO_WritePin(GPIOE,GPIO_PIN_4,GPIO_PIN_SET)

#define LED2_T		HAL_GPIO_TogglePin(GPIOG,GPIO_PIN_9)
#define LED2_ON		HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_RESET)
#define LED2_OFF	HAL_GPIO_WritePin(GPIOG,GPIO_PIN_9,GPIO_PIN_SET)

#define ADC_NUMBER		8					//模拟量数目
#define FREQ_NUMBER		8					//频率量数目

#define FREQ_INDEX		ADC_NUMBER			//频率的起始索引
#define PL_CALC_PERIOD	10					//频率计算周期 单位ms


#define PL_CNT1	Counter[0]
#define PL_CNT2	Counter[1]
#define PL_CNT3	Counter[2]
#define PL_CNT4	Counter[3]
#define PL_CNT5	Counter[4]
#define PL_CNT6	Counter[5]
//#define PL_CNT7	Counter[6]
//#define PL_CNT8	Counter[7]

#define KO_ON	GPIO_PIN_RESET
#define KO_OFF	GPIO_PIN_SET

//#define KIN1_READ		HAL_GPIO_ReadPin(KIN1_GPIO_Port,KIN1_Pin)
//#define KIN2_READ		HAL_GPIO_ReadPin(KIN2_GPIO_Port,KIN2_Pin)
//#define KIN3_READ		HAL_GPIO_ReadPin(KIN3_GPIO_Port,KIN3_Pin)
//#define KIN4_READ		HAL_GPIO_ReadPin(KIN4_GPIO_Port,KIN4_Pin)
//#define KIN5_READ		HAL_GPIO_ReadPin(KIN5_GPIO_Port,KIN5_Pin)
//#define KIN6_READ		HAL_GPIO_ReadPin(KIN6_GPIO_Port,KIN6_Pin)
////#define KIN7_READ		HAL_GPIO_ReadPin(KIN7_GPIO_Port,KIN7_Pin)
////#define KIN8_READ		HAL_GPIO_ReadPin(KIN8_GPIO_Port,KIN8_Pin)
//
//#define KOUT1_READ	HAL_GPIO_ReadPin(KOUT1_GPIO_Port,KOUT1_Pin)
//#define KOUT2_READ	HAL_GPIO_ReadPin(KOUT2_GPIO_Port,KOUT2_Pin)
//#define KOUT3_READ	HAL_GPIO_ReadPin(KOUT3_GPIO_Port,KOUT3_Pin)
//#define KOUT4_READ	HAL_GPIO_ReadPin(KOUT4_GPIO_Port,KOUT4_Pin)
//#define KOUT5_READ	HAL_GPIO_ReadPin(KOUT5_GPIO_Port,KOUT5_Pin)
//#define KOUT6_READ	HAL_GPIO_ReadPin(KOUT6_GPIO_Port,KOUT6_Pin)
//#define KOUT7_READ	HAL_GPIO_ReadPin(KOUT7_GPIO_Port,KOUT7_Pin)
//#define KOUT8_READ	HAL_GPIO_ReadPin(KOUT8_GPIO_Port,KOUT8_Pin)
//#define KOUT9_READ	HAL_GPIO_ReadPin(KOUT9_GPIO_Port,KOUT9_Pin)
//#define KOUT10_READ	HAL_GPIO_ReadPin(KOUT10_GPIO_Port,KOUT10_Pin)
//
//#define KOUT1_ON		HAL_GPIO_WritePin(KOUT1_GPIO_Port,KOUT1_Pin,KO_ON)
//#define KOUT1_OFF		HAL_GPIO_WritePin(KOUT1_GPIO_Port,KOUT1_Pin,KO_OFF)
//
//#define KOUT2_ON		HAL_GPIO_WritePin(KOUT2_GPIO_Port,KOUT2_Pin,KO_ON)
//#define KOUT2_OFF		HAL_GPIO_WritePin(KOUT2_GPIO_Port,KOUT2_Pin,KO_OFF)
//
//#define KOUT3_ON		HAL_GPIO_WritePin(KOUT3_GPIO_Port,KOUT3_Pin,KO_ON)
//#define KOUT3_OFF		HAL_GPIO_WritePin(KOUT3_GPIO_Port,KOUT3_Pin,KO_OFF)
//
//#define KOUT4_ON		HAL_GPIO_WritePin(KOUT4_GPIO_Port,KOUT4_Pin,KO_ON)
//#define KOUT4_OFF		HAL_GPIO_WritePin(KOUT4_GPIO_Port,KOUT4_Pin,KO_OFF)
//
//#define KOUT5_ON		HAL_GPIO_WritePin(KOUT5_GPIO_Port,KOUT5_Pin,KO_ON)
//#define KOUT5_OFF		HAL_GPIO_WritePin(KOUT5_GPIO_Port,KOUT5_Pin,KO_OFF)
//
//#define KOUT6_ON		HAL_GPIO_WritePin(KOUT6_GPIO_Port,KOUT6_Pin,KO_ON)
//#define KOUT6_OFF		HAL_GPIO_WritePin(KOUT6_GPIO_Port,KOUT6_Pin,KO_OFF)
//
//#define KOUT7_ON		HAL_GPIO_WritePin(KOUT7_GPIO_Port,KOUT7_Pin,KO_ON)
//#define KOUT7_OFF		HAL_GPIO_WritePin(KOUT7_GPIO_Port,KOUT7_Pin,KO_OFF)
//
//#define KOUT8_ON		HAL_GPIO_WritePin(KOUT8_GPIO_Port,KOUT8_Pin,KO_ON)
//#define KOUT8_OFF		HAL_GPIO_WritePin(KOUT8_GPIO_Port,KOUT8_Pin,KO_OFF)
//
//#define KOUT9_ON		HAL_GPIO_WritePin(KOUT9_GPIO_Port,KOUT9_Pin,KO_ON)
//#define KOUT9_OFF		HAL_GPIO_WritePin(KOUT9_GPIO_Port,KOUT9_Pin,KO_OFF)
//
//#define KOUT10_ON		HAL_GPIO_WritePin(KOUT10_GPIO_Port,KOUT10_Pin,KO_ON)
//#define KOUT10_OFF	HAL_GPIO_WritePin(KOUT10_GPIO_Port,KOUT10_Pin,KO_OFF)

//#define SW1_READ		HAL_GPIO_ReadPin(SW1_GPIO_Port,SW1_Pin)
//#define SW2_READ		HAL_GPIO_ReadPin(SW2_GPIO_Port,SW2_Pin)
//#define SW3_READ		HAL_GPIO_ReadPin(SW3_GPIO_Port,SW3_Pin)
//#define SW4_READ		HAL_GPIO_ReadPin(SW4_GPIO_Port,SW4_Pin)

#define KIN1_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9)
#define KIN2_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_8)
#define KIN3_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_7)
#define KIN4_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_6)
#define KIN5_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)
#define KIN6_READ		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_6)

#define KIN7_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_1)
#define KIN8_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_0)

#define KOUT1_READ		HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_8)
#define KOUT2_READ		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_9)
#define KOUT3_READ		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_8)
#define KOUT4_READ		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_7)

#define KOUT5_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_5)
#define KOUT6_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_4)
#define KOUT7_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_3)
#define KOUT8_READ		HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_2)

#define KOUT1_ON		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,KO_ON)
#define KOUT1_OFF		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_8,KO_OFF)

#define KOUT2_ON		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,KO_ON)
#define KOUT2_OFF		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_9,KO_OFF)

#define KOUT3_ON		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,KO_ON)
#define KOUT3_OFF		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_8,KO_OFF)

#define KOUT4_ON		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,KO_ON)
#define KOUT4_OFF		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_7,KO_OFF)

#define KOUT5_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,KO_ON)
#define KOUT5_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_5,KO_OFF)

#define KOUT6_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,KO_ON)
#define KOUT6_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_4,KO_OFF)

#define KOUT7_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,KO_ON)
#define KOUT7_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_3,KO_OFF)

#define KOUT8_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,KO_ON)
#define KOUT8_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_2,KO_OFF)

#define KOUT9_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,KO_ON)
#define KOUT9_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,KO_OFF)

#define KOUT10_ON		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,KO_ON)
#define KOUT10_OFF		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,KO_OFF)

#define SW1_READ		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_12)
#define SW2_READ		HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_2)
#define SW3_READ		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_5)
#define SW4_READ		HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13)

#define TOTAL_OUT 	10
#define K_CMD_ON	1
#define K_CMD_OFF	0

#define M1_NORMAL_CMD_CHN	0	//KOUT1
#define M1_RESERVE_CMD_CHN	1
#define M2_NORMAL_CMD_CHN	2	//KOUT3
#define M2_RESERVE_CMD_CHN	3
#define M1_RESET_CMD_CHN	4	//KOUT5
#define M2_RESET_CMD_CHN	5

#define XLF_CMD_CHN			8	//KOUT9
#define JSF_CMD_CHN			9	//KOUT10

#define M1_NORMAL_CMD  	Out_Cmd[M1_NORMAL_CMD_CHN]
#define M1_REVERSE_CMD 	Out_Cmd[M1_RESERVE_CMD_CHN]
#define M1_RESET_CMD  	Out_Cmd[M1_RESET_CMD_CHN]

#define M2_NORMAL_CMD  	Out_Cmd[M2_NORMAL_CMD_CHN]
#define M2_REVERSE_CMD 	Out_Cmd[M2_RESERVE_CMD_CHN]
#define M2_RESET_CMD  	Out_Cmd[M2_RESET_CMD_CHN]

#define M1_NORMAL	M1_NORMAL_CMD=K_CMD_ON;M1_REVERSE_CMD=K_CMD_OFF;M1_RESET_CMD=K_CMD_OFF
#define M1_REVERSE	M1_NORMAL_CMD=K_CMD_OFF;M1_REVERSE_CMD=K_CMD_ON;M1_RESET_CMD=K_CMD_OFF
#define M1_STOP		M1_NORMAL_CMD=K_CMD_OFF;M1_REVERSE_CMD=K_CMD_OFF;M1_RESET_CMD=K_CMD_ON

#define M2_NORMAL	M2_NORMAL_CMD=K_CMD_ON;M2_REVERSE_CMD=K_CMD_OFF;M2_RESET_CMD=K_CMD_OFF
#define M2_REVERSE	M2_NORMAL_CMD=K_CMD_OFF;M2_REVERSE_CMD=K_CMD_ON;M2_RESET_CMD=K_CMD_OFF
#define M2_STOP		M2_NORMAL_CMD=K_CMD_OFF;M2_REVERSE_CMD=K_CMD_OFF;M2_RESET_CMD=K_CMD_ON

#define M1_ALL_OFF	M1_NORMAL_CMD=K_CMD_OFF;M1_REVERSE_CMD=K_CMD_OFF;M1_RESET_CMD=K_CMD_OFF
#define M2_ALL_OFF	M2_NORMAL_CMD=K_CMD_OFF;M2_REVERSE_CMD=K_CMD_OFF;M2_RESET_CMD=K_CMD_OFF

#define XLF_OUT  	Out_Cmd[XLF_CMD_CHN]
#define XLF_ON		XLF_OUT=K_CMD_ON
#define XLF_OFF		XLF_OUT=K_CMD_OFF

#define JSF_OUT  	Out_Cmd[JSF_CMD_CHN]
#define JSF_ON		JSF_OUT=K_CMD_ON
#define JSF_OFF		JSF_OUT=K_CMD_OFF

#define M_CMD_FREE 	0
#define M_CMD_STOP 	1
#define M_CMD_RUN	2
#define M_CMD_PID	3

//#define M1_DY_CHN						7
//#define	XT_YALI_CHN					0
//#define	MADA_PAILYALI_CHN		3
//#define BENG_ZHUANSU_CHN		FREQ_INDEX+BENG_ZS_CHN

//#define	BENG_PL_DY			AllSampleBuf[M1_DY_CHN]
//#define	XT_YALI					AllSampleBuf[XT_YALI_CHN]
//#define	MADA_PAIL_YALI	AllSampleBuf[MADA_PAILYALI_CHN]
//#define BENG_ZHUANSU		AllSampleBuf[BENG_ZHUANSU_CHN]

#define WIFI_ERROR	0x01
#define TEMP_ERROR	0X02
#define I2C_ERROR	0X04

extern DAC_HandleTypeDef hdac;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart3;
extern CAN_HandleTypeDef hcan1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim4;
extern ADC_HandleTypeDef hadc1;
extern DMA_HandleTypeDef hdma_memtomem_dma2_stream1;
extern DMA_HandleTypeDef hdma_usart1_rx;
extern I2C_HandleTypeDef hi2c2;

extern osThreadId_t defaultTaskHandle;
extern osThreadId_t can1TaskHandle;
extern osThreadId_t usart1TaskHandle;
extern osThreadId_t adcTaskHandle;
extern osThreadId_t usart3TaskHandle;
extern osMessageQueueId_t canMsgQueueHandle;
//extern osMessageQueueId_t espMsgQueueHandle;

void DMATransferComplete(DMA_HandleTypeDef *hdma);

void UART3_IDLECallBack(void);
void UART1_IDLECallBack(void);

void Write_All_Parameters(void);
void Read_All_Parameters(void);

void ParseRxData(ESP_MSG *pRx);
void CanMsgProc(CAN_MSG *msg);

void ParseUsartModBusData(USART_DATA *pRx);
int ParseModeBusData(uint8_t * pData,uint16_t dataSize,uint8_t *pDstBuf);

void calc_fft_q15(uint16_t * all_data_in,uint16_t * result);//16-346us 3-66us
void calc_fft(uint16_t * all_data_in,uint16_t * result);

void clrCapturreFlag(void);
void ProcKout(void);

unsigned char cal_crc_table(unsigned char *p,unsigned char j);

uint16_t CRC16(uint8_t *data,uint16_t len);

uint16_t GetModBusWord(uint8_t *pData);

int CteateSampleData(uint16_t *pbuf);

HAL_StatusTypeDef CanSendData(uint32_t StdId,uint32_t len,uint8_t * buf);
HAL_StatusTypeDef CanSendRemoteExd(uint32_t ExtId,uint32_t len,uint8_t * buf);
int esp8266_Data(ESP_MSG *Esp_Data);

void ProcModBusDJCmd(uint8_t *p);
void ProcModBusPidCmd(uint8_t *pData);
void ProcKZRunCmdBuf(uint8_t *pData);
void StopMotor(uint16_t motor);
void ProcDJCmd(uint8_t *p);
void motorInit(void);
void bengRun(MOTOR *motor,MPID *pPid);
void madaRun(MOTOR *motor,MPID *pPid);

void calc_pinlv(void);
void read_KinOutSw(void);
uint16_t read_sw_sta(void);

void CAN1_Filter_Init_All_In(void);
void CAN1_Filter_Init_StdID(void);
void CAN1_Filter_Init_ExdID(void);

void clearPid(MPID* ptr);
int getPidInc(MPID* ptr);
void initPidAllPara(MPID* ptr,uint16_t K,uint16_t kp,uint16_t ki,uint16_t kd);

int esp8266_initAP(void);

void InitMyApp(void);
void init_led(void);

//CAN
#define BROADCAST_EXTID		0x12345678
#define DATA_OF_MASTER		0x12
#define DATA_OF_SLAVE		0x40

//ModBus
#define ReadCoils				1
#define ReadInputs				2
#define ReadHoldingRegisters	3
#define ReadInputRegisters		4
#define WriteSingleCoil			5
#define WriteSingleRegister		6
#define byteDiagnostics			8
#define WriteMultipleCoils		15
#define WriteMultipleRegisters	16
//#define ReadWriteMultipleRegisters	23

#define ResetRegister		100
#define InputsRegister		1000

#define SampleRegister		1000

#define ParameterRegister	1000
#define	MotorCmdRegister	2000
#define	PIDCmdRegister		3000
#define	KZRunCmdRegister	4000

#define StopMotorRegister 	1000

//01 03 03 E8 00 64 C4 51
//02 03 03 E8 00 64 C4 62

//01 04 03 E8 00 24 70 61
//02 04 03 E8 00 24 70 52

#endif
