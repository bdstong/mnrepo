#include "taskApp.h"

/*
注意
1 I2C 初始化，要把时钟初始化放到引脚初始换前面 HAL_I2C_MspInit
2 UART若用DMA发送，必须开启串口中断，否则只能发送一次
3 mem to mem DMA要手动添加回调函数，名字自定
4 启动ADC前，加入自校准代码
5 在系统初始化后，要添加代码：启动定时器，启动串口接收，ADC的DMA启动
  HAL_DAC_Start
6 DMA设定：串口-接收循环，发送单次、ADC-循环、memtomem-单次
7 TIM触发ADC，Trigger Event Selection -->Update Event, Master/Slave-->Disable
8 外中断的引脚要上拉，以免浮空引进干扰频繁中断
*/
/*
APB1 tim2 tim3 tim4 tim5 tim6 tim7 tim12-tim14
APB2 tim1 tim8 tim9 tim10 tim11
APB2 ADC clock /2 /4 /6 /8
*/

extern USART_DATA UsartType1;
extern USART_DATA UsartType3;

extern uint16_t AllSampleBuf[];

extern TIM_HandleTypeDef htim14;

extern uint16_t Parameters[];

extern uint8_t EspTxBuffer[];

extern uint16_t LocalAddr;
extern uint16_t MasterFlag;
extern void InitOut(void);
extern uint16_t Out_Cmd[];
extern MOTOR Beng,Mada;
extern MPID bengPid,madaPid;

//extern MPID bengPid;
//void clrCapturreFlag(void);

/*
uint16_t start_tim,end_tim;//ÍâÖÐ¶ÏÂö³å¿ªÊ¼ºÍ½áÊø
uint16_t dur;
float distance=0.0;
uint32_t cct=0;
*/
uint32_t RunTime=0;
uint16_t AlarmFlag=0;

uint16_t ADC_DMA_BUF[ADC_NUMBER*NPT];
uint16_t FFT_ALL_IN[ADC_NUMBER*NPT];

uint16_t pidKp;
uint8_t pidEnable,pidPeriod;

uint8_t EspTxBuffer[512];

static uint32_t CpuID[3];
void Get_ChipID(void)
{
	CpuID[0]=*(__IO uint32_t*)(0x1FFF7A10);
	CpuID[1]=*(__IO uint32_t*)(0x1FFF7A14);
	CpuID[2]=*(__IO uint32_t*)(0x1FFF7A18);
}

uint8_t getChipidCRC()
{
	unsigned char crc=cal_crc_table((unsigned char *)CpuID,12);
	return crc;
}

//ÔÚÈÎÎñÖÐÏò´®¿Ú·¢ËÍÊý¾Ý£¬²»ÄÜÔÙISRÖÐµ÷ÓÃ
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
void init_led()
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	//	  HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin|KOUT1_Pin, GPIO_PIN_SET);
	//	  /*Configure GPIO pin Output Level */

	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
}

void InitMyApp()
{
//	clrCapturreFlag();
	init_led();

	Get_ChipID();
	LocalAddr=read_sw_sta();

	hdma_memtomem_dma2_stream1.XferCpltCallback=DMATransferComplete;

	HAL_TIM_Base_Start(&htim2);//Æô¶¯Tim2 ¶¨Ê±´¥·¢ADC
	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_DMA_BUF, ADC_NUMBER*NPT);//statr ADC with DMA use FFT
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_DMA_BUF, ADC_NUMBER);//statr ADC with DMA	direct

	HAL_UART_Receive_DMA(&huart1,UsartType1.RxBuf, USART_BUF_SIZE);
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);//¿ªÆô¿ÕÏÐÖÐ¶Ï
	__HAL_UART_DISABLE_IT(&huart1,UART_IT_RXNE);//¹Ø±Õ×Ö·û½ÓÊÕÖÐ¶Ï
	__HAL_UART_DISABLE_IT(&huart1,UART_IT_TXE);//¹Ø±Õ·¢ËÍ¿ÕÖÐ¶Ï

	//UsartType3.dmaSend_flag=USART_DMA_SENDOVER;
	HAL_UART_Receive_DMA(&huart3,UsartType3.RxBuf, USART_BUF_SIZE);
	__HAL_UART_ENABLE_IT(&huart3,UART_IT_IDLE);//¿ªÆô¿ÕÏÐÖÐ¶Ï
	__HAL_UART_DISABLE_IT(&huart3,UART_IT_RXNE);//¹Ø±Õ×Ö·û½ÓÊÕÖÐ¶Ï
	__HAL_UART_DISABLE_IT(&huart3,UART_IT_TXE);//¹Ø±Õ·¢ËÍ¿ÕÖÐ¶Ï

	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_2);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_4);

	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_1);
	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_2);
//	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_3);
//	HAL_TIM_IC_Start_IT(&htim3,TIM_CHANNEL_4);

//  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
//  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);

	HAL_DAC_Start(&hdac,DAC_CHANNEL_1);
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,0);

//#if ((defined SPI_CS_Pin) && (defined SPI_CLK_Pin))
//	MX6675_Init(&Max6675TypeDef1);
//	MX31855_Init(&Max31855TypeDef1);
//#endif

	if(LocalAddr==0x01){
		CAN1_Filter_Init_All_In();
		MasterFlag=1;
	}
	else{
		CAN1_Filter_Init_StdID();
		CAN1_Filter_Init_ExdID();
	}
	HAL_CAN_Start(&hcan1);//¿ªÆôCAN1
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);//ÖÐ¶ÏÔÊÐí

	Read_All_Parameters();

	InitOut();
	HAL_TIM_Base_Start_IT(&htim14);//2K Hz

}
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
#define TEMBUF_LEN 2
void StartDefaultTask(void * argument)
{
//	uint16_t tem_buf[TEMBUF_LEN];
//	int i;
//	uint16_t temperature;
//	uint32_t sum;
//	for(i=0;i<TEMBUF_LEN;i++) tem_buf[i]=0;
/*
	if((temperature&0x4)==0)
	{
		for(i=0;i<TEMBUF_LEN;i++) tem_buf[i]=temperature>>3;
		TemperatureSampleValue=temperature>>3;
	}
	else
	{
		for(i=0;i<TEMBUF_LEN;i++) tem_buf[i]=0;
		TemperatureSampleValue=0xffff;
	}
*/

	//printf("chip=%d\n",WENDUCHIP);
  for(;;)
  {
		osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
	    osDelay(100);
		Write_All_Parameters();
//		LED0_T;
    //osDelay(500);
//		if(WENDUCHIP==31855) Read_MAX31855(&Max31855TypeDef1);
//		else if(WENDUCHIP==6675)Read_MAX6675(&Max6675TypeDef1);
//		else AlarmFlag|=TEMP_ERROR;
/*
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);//SPI_CS_0;
		HAL_SPI_Receive(&hspi1,(uint8_t*)&TemperatureSampleValue,2,0xffff);
		HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);//SPI_CS_1;
*/
		/*
		for(i=0;i<TEMBUF_LEN-1;i++) tem_buf[i]=tem_buf[i+1];
		tem_buf[i]=	temperature>>3;
		sum=0;
		for(i=0;i<TEMBUF_LEN;i++) sum+=tem_buf[i];
		TemperatureSampleValue=sum/TEMBUF_LEN;
*/
		/*
		if((temperature&0x4)==0)
		{
			for(i=0;i<TEMBUF_LEN-1;i++) tem_buf[i]=tem_buf[i+1];
			tem_buf[i]=	temperature>>3;
			sum=0;
			for(i=0;i<TEMBUF_LEN;i++) sum+=tem_buf[i];
			TemperatureSampleValue=sum/TEMBUF_LEN;
		}
		else
		{
			for(i=0;i<TEMBUF_LEN;i++) tem_buf[i]=0;
			TemperatureSampleValue=0xffff;
		}
	*/

  }
  /* USER CODE END StartDefaultTask */
}

void calc_avg(uint16_t * all_data_in,uint16_t * result)
{
	uint32_t sum;

	for(int j=0;j<ADC_NUMBER;j++)
	{
		sum=0;
		for(int i=0;i<NPT;i++)
		{
			sum+=all_data_in[ADC_NUMBER*i+j];
		}
		result[j]=sum/NPT;
	}

}

void StartAdcTask(void * argument)
{
	uint16_t runcnt=0;
	uint16_t ledcnt=0;
	uint16_t kzCmdBuf[3];

	int period;
	if(MasterFlag)
	{
		period=50;
	}
	else period=10;

	//printf("TaskOfAdcProcess\r\n");
	//PLkzEnable=0;
	motorInit();
  for(;;)
  {
		osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);

		//LED_ON;
		if(USE_FFT_METHOD==1234)
		{
			calc_fft_q15(FFT_ALL_IN,AllSampleBuf);//76us
			//calc_fft(FFT_ALL_IN,AllSampleBuf);
		}
		else
		{
			calc_avg(FFT_ALL_IN,AllSampleBuf);//
		}
		//LED_OFF;

		calc_pinlv();//2us

		bengRun(&Beng,&bengPid);		//0.65-0.8 us
		madaRun(&Mada,&madaPid);

		ledcnt++;
		if(ledcnt>=period)
		{
			ledcnt=0;
			LED_T;
			LED0_T;
		}

		RunTime++;

		runcnt++;
		if(runcnt>=10)
		{
			runcnt=0;
		}

//--------------------------------------------------------------
		if(JSF_YALI_BAOHU){
			if(AllSampleBuf[XITONG_YALI_CHN]>JSF_YALI_BAOHU){
				JSF_OFF;
			}
		}
		if(JSF_ZHUNASU_BAOHU)
		{
			if(AllSampleBuf[MADA_ZHUANSU_CHN]>JSF_ZHUNASU_BAOHU){
				JSF_OFF;
			}
		}

		if(MADA_RESET_YALI)//mada auto reset
		{
			if(Out_Cmd[Mada.norCmdChn]==K_CMD_ON && AllSampleBuf[XITONG_YALI_CHN]>MADA_RESET_YALI)
			{
				kzCmdBuf[0]=(1 << Mada.norCmdChn) | (1 << Mada.revCmdChn);//turn off
				kzCmdBuf[1]=(1 << Mada.rstCmdChn);	//turn on
				kzCmdBuf[2]=Mada.rstDelayTime;			//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
			}
		}
//--------------------------------------------------------------

}
}


void StartUsart1Task(void * argument)
{
	//printf("Task_of_Uart1\r\n");

	for(;;)
  {
		osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
		//HAL_UART_Transmit(&huart1,UsartType1.RxBuf,UsartType1.rx_len,0xffff);
		LED2_ON;
		ParseUsartModBusData(&UsartType1);
		LED2_OFF;


/*
		UsartType1.RxBuf[UsartType1.rx_len]='\0';

		for(int i=0;i<UsartType1.rx_len;i++)
		{
			uint8_t Value=UsartType1.RxBuf[i];
			switch(Value)
			{
				case 0x31:
				xResult=CAN_Transmit(&hcan1,0,0,"12345678",6);
				printf("---------CAN1 err=%d\r\n",xResult);
				break;

				case 0x32:
				print_id();
				printf("s=%d\r\n",getChipidSum());
				break;

				case 0x33:
					cct++;

				if((Max6675TypeDef1.Result&4)) printf("---------------\r\n");
				printf("\t%.1f\r\n",(float)(Max6675TypeDef1.Result>>3)/4);

					//if((Max6675TypeDef1.Result&4)) printf("---------------\r\n");
					//printf("\t%x %d\r\n",Max31855TypeDef1.Result,Max31855TypeDef1.Result>>18);
				break;
				case 0x34:
					print_adc();
				//psd();
					printf("cnt=%d\r\n",cct);
					break;
				case 0x35:
					print_pl();
				cct=0;
					break;
				case 0x38:
					SendSampleData();
				break;
				case 0x39:
					HAL_NVIC_SystemReset();
					break;
			}
		}
		*/
  }//for
}
//void ParseEspRxData(uint8_t *pdata,ESP_MSG *pesp)
//{
//	char *head,*clientID,*dataSize,*dataBuffer;
//	UNUSED(head);
//	head = strtok((char*)pdata, ",");//+IPD
//	clientID = strtok(NULL, ",");//0
//	dataSize = strtok(NULL, ":");//5
//	dataBuffer = strtok(NULL, "");//12345
//
//	pesp->clientID=atoi(clientID);
//	pesp->dataSize=atoi(dataSize);
//	pesp->pData=(uint8_t*)dataBuffer;
//}
//void StartUsart3Task(void * argument)
//{
//	ESP_MSG Esp_Rx;
//	uint8_t *str;
//
////	printf("Task_of_Uart3\r\n");
//
//	for(;;)
//  {
//		osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
////		//ulEventsToProcess =
////		ulTaskNotifyTake( pdTRUE, portMAX_DELAY );
//
//		UsartType3.RxBuf[UsartType3.rx_len]=0;
//		str=UsartType3.RxBuf;
//		//HAL_UART_Transmit(&huart1,(uint8_t*)str,UsartType3.rx_len,0xffff);
//
//		if(strstr((char*)str,"+IPD,"))//
//		{
//			ParseEspRxData(str,&Esp_Rx);
//			osMessageQueuePut(espMsgQueueHandle, &Esp_Rx, 0, 0);
//		}
//		else if(strstr((char*)str,">"))
//		{
//			ESP_Status_Flag|=ESP_SEND_READY;
//		}
//		else if(strstr((char*)str,"SEND OK"))
//		{
//			ESP_Status_Flag|=ESP_SEND_OK;
//			//printf("send ok flag=1\r\n");
//		}
//		else if(strstr((char*)str,"OK"))
//		{
//			ESP_Status_Flag|=ESP_OK;
//		}
//		else if(strstr((char*)str,"ready"))
//		{
//			ESP_Status_Flag|=ESP_RST_READY;
//		}
///*
//		else if(strstr((char*)str,"CONNECT"))
//		{
//				//printf("\r\n---------new------------\n");
//			HAL_UART_Transmit(&huart1,UsartType3.RxBuf,UsartType3.rx_len,0xffff);
//		}
//		else if(strstr((char*)str,"CLOSED"))
//		{
//				//printf("\r\n---------new------------\n");
//			HAL_UART_Transmit(&huart1,UsartType3.RxBuf,UsartType3.rx_len,0xffff);
//		}
//*/
//  }
//}
void StartCan1Task(void * argument)
{
	CAN_MSG can_msg;
//printf("Task_of_CAN\r\n");
  for(;;)
  {
		osStatus_t status =osMessageQueueGet(canMsgQueueHandle, &can_msg, NULL,portMAX_DELAY);
		if( status == osOK )
		{
			CanMsgProc(&can_msg);
			//if(can_msg.hcan==&hcan1)	LED1_T;
			//if(can_msg.hcan==&hcan2)	LED2_T;
		}

		//get_can_msg(&can_msg);
		//CanMsgProc(&can_msg);
//buf[0]=0xa5;
//buf[1]=0x5a;
//HAL_UART_Transmit(&huart1,buf,2,0xFFFF);


/*

		printf("Reviced RTR=%d\n",can_msg.RxHead.RTR);
		printf("Reviced IDE=%d\n",can_msg.RxHead.IDE);

		if(can_msg.RxHead.IDE==CAN_ID_STD)
		printf("StdId=%x\n",can_msg.RxHead.StdId);
		if(can_msg.RxHead.IDE==CAN_ID_EXT)
		printf("ExtId=%x\n",can_msg.RxHead.ExtId);

		printf("Reviced Remote DLC=%d\n",can_msg.RxHead.DLC);

		printf("CAN Recived data:");
		for(i=0;i<can_msg.RxHead.DLC;i++)	printf("%c",can_msg.RxBuffer[i]);
		printf("\n");
*/
  }
}
//void Task_Of_Esp(void * argument)
//{
//printf("Task_Of_Esp\r\n");
//
//	osDelay(2000);
//	if(esp8266_initAP())	AlarmFlag|=WIFI_ERROR;
//	else AlarmFlag&=~WIFI_ERROR;
//
//	//if(eret==0)
//	//	printf("\r\ninit ap ok\r\n");
//	//else printf("\r\ninit ap fail  -------%d\r\n",eret);
//
//	ESP_MSG EspRxData;
//
//  for(;;)
//  {
////		BaseType_t xStatus;
////		xStatus = xQueueReceive(espCmdQueueHandle,&EspRxData, portMAX_DELAY );
//		osStatus_t status =osMessageQueueGet(espMsgQueueHandle, &EspRxData, NULL,portMAX_DELAY);
//		if( status == osOK )
//		{
//
//				//printf( "\r\nclientID=%d\r\n", EspRxData.clientID );//???????
//				//printf( "dataSize=%d\r\n", EspRxData.dataSize );//???????
//				//printf( "dataBuffer=%s\r\n", EspRxData.pData );//???????
//
//			//HAL_UART_Transmit(&huart1,esp_rcv.pData,esp_rcv.dataSize,0xffff);
//			//esp_rcv
//
//			memcpy(EspTxBuffer,EspRxData.pData,EspRxData.dataSize);
//			EspRxData.type=TYPE_ESP;
//			EspRxData.pData=EspTxBuffer;
//			osDelay(20);
//			//ParseRxData(&EspRxData);
//
//			//printf( "buffer=%s\r\n", EspTxBuffer);//???????
//			//uint32_t ret=esp8266_Data(&EspRxData);
//			//if(ret!=0)printf("Send Error  %d\r\n",ret);
//		}
//
////    osDelay(500);
//		//uint16_t tt=ReadMAX6675();
//  }
//  /* USER CODE END Task_Of_Esp */
//}
