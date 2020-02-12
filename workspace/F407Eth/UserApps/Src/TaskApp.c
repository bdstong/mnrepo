#include "TaskApp.h"

uint32_t RunTime = 0;

USART_DATA UsartType1;
USART_DATA UsartType3;

//extern uint16_t Parameters[];

//extern uint16_t LocalAddr;
//extern uint16_t MasterFlag;
PUTCHAR_PROTOTYPE {
	HAL_UART_Transmit(&huart1, (uint8_t*) &ch, 1, HAL_MAX_DELAY);
	return ch;
}

static uint32_t CpuID[3];
void Get_ChipID(void) {
	CpuID[0] = *(__IO uint32_t*) (0x1FFF7A10);
	CpuID[1] = *(__IO uint32_t*) (0x1FFF7A14);
	CpuID[2] = *(__IO uint32_t*) (0x1FFF7A18);
}

uint8_t getChipidCRC() {
	unsigned char crc = CRC16((unsigned char*) CpuID, 12);
	return crc;
}

//void init_led() {
//GPIO_InitTypeDef GPIO_InitStruct = { 0 };
//
///* GPIO Ports Clock Enable */
//__HAL_RCC_GPIOE_CLK_ENABLE();
//__HAL_RCC_GPIOG_CLK_ENABLE();
//
///*Configure GPIO pin Output Level */
////	  HAL_GPIO_WritePin(GPIOA, SPI_CS_Pin|KOUT1_Pin, GPIO_PIN_SET);
////	  /*Configure GPIO pin Output Level */
////	  HAL_GPIO_WritePin(GPIOC, LED_Pin|KOUT4_Pin|KOUT3_Pin|KOUT2_Pin, GPIO_PIN_SET);
//GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//GPIO_InitStruct.Pull = GPIO_NOPULL;
//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);
//
//GPIO_InitStruct.Pin = GPIO_PIN_9;
//GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
//GPIO_InitStruct.Pull = GPIO_NOPULL;
//GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
//HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);
//}
void InitMyApp() {
	Get_ChipID();
//	LocalAddr = 1;	//read_sw_sta();

	UsartType1.UsartHandle = &huart1;
	HAL_UART_Receive_DMA(&huart1, UsartType1.RxBuf, USART_BUF_SIZE);
	__HAL_UART_ENABLE_IT(&huart1, UART_IT_IDLE); //¿ªÆô¿ÕÏÐÖÐ¶Ï
	__HAL_UART_DISABLE_IT(&huart1, UART_IT_RXNE); //¹Ø±Õ×Ö·û½ÓÊÕÖÐ¶Ï
	__HAL_UART_DISABLE_IT(&huart1, UART_IT_TXE); //¹Ø±Õ·¢ËÍ¿ÕÖÐ¶Ï

//UsartType3.dmaSend_flag=USART_DMA_SENDOVER;
//	UsartType3.UsartHandle = &huart3;
//	HAL_UART_Receive_DMA(&huart3, UsartType3.RxBuf, USART_BUF_SIZE);
//	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE); //¿ªÆô¿ÕÏÐÖÐ¶Ï
//	__HAL_UART_DISABLE_IT(&huart3, UART_IT_RXNE); //¹Ø±Õ×Ö·û½ÓÊÕÖÐ¶Ï
//	__HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE); //¹Ø±Õ·¢ËÍ¿ÕÖÐ¶Ï

//hdma_memtomem_dma2_stream1.XferCpltCallback = DMATransferComplete;

//HAL_TIM_Base_Start(&htim2); //Æô¶¯Tim2 ¶¨Ê±´¥·¢ADC
//HAL_ADC_Start_DMA(&hadc1, (uint32_t*) ADC_DMA_BUF, ADC_NUMBER * NPT); //statr ADC with DMA use FFT
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t*)ADC_DMA_BUF, ADC_NUMBER);//statr ADC with DMA	direct

//HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_1);
//HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_2);
//HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_3);
//HAL_TIM_IC_Start_IT(&htim4, TIM_CHANNEL_4);
//
//HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_1);
//HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_2);
//HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_3);
//HAL_TIM_IC_Start_IT(&htim3, TIM_CHANNEL_4);

//  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
//  __HAL_TIM_ENABLE_IT(&htim4, TIM_IT_UPDATE);

//HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
//HAL_DAC_SetValue(&hdac, DAC_CHANNEL_1, DAC_ALIGN_12B_R, 0);

//#if ((defined SPI_CS_Pin) && (defined SPI_CLK_Pin))
//	MX6675_Init(&Max6675TypeDef1);
//	MX31855_Init(&Max31855TypeDef1);
//#endif
//	CAN1_Filter_Init_All_In();
//	MasterFlag = 1;

//if (LocalAddr == 0x01) {
//	CAN1_Filter_Init_All_In();
//	MasterFlag = 1;
//} else {
//	CAN1_Filter_Init_StdID();
//	CAN1_Filter_Init_ExdID();
//	MasterFlag = 0;
//}

//	HAL_CAN_Start(&hcan1); //¿ªÆôCAN1
//	HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING); //ÖÐ¶ÏÔÊÐí

	Read_All_Parameters();

//InitOut();
//HAL_TIM_Base_Start_IT(&htim14); //2K Hz

}
void StartEthTask(void *argument){
	ETH_MSG eth_msg;
	for (;;) {
		osStatus_t status = osMessageQueueGet(ethMsgQueueHandle, &eth_msg, NULL,
		portMAX_DELAY);
//printf("can task status=%x\n",status);
		//xStatus = xQueueReceive(canRxQueueHandle,&can_msg, portMAX_DELAY );
		if (status == osOK) {
			LED2_T;
			//HAL_UART_Transmit(&huart1,eth_msg.buf,eth_msg.len,HAL_MAX_DELAY);
			send(eth_msg.s,eth_msg.buf,eth_msg.len,MSG_DONTWAIT);//MSG_MORE);//MSG_DONTWAIT);
//			uint32_t rr=osThreadFlagsWait(0x01U, osFlagsWaitAny, 1000);//osWaitForever);
//			if(rr==0x01U){
//				//HAL_UART_Transmit(&huart1,UsartType1.RxBuf,UsartType1.rx_len,HAL_MAX_DELAY);
//				send(eth_msg.s,UsartType1.RxBuf,UsartType1.rx_len,MSG_DONTWAIT);
//			}
//			else
//			{
//				///printf("rr=%d\n",rr);
//			}
//			if(r!=eth_msg.len) 	printf("send er=%d\r\n",r);
		}
			//CanMsgProc(&can_msg);
			//if(can_msg.hcan==&hcan1)	LED1_T;
			//if(can_msg.hcan==&hcan2)	LED2_T;
//printf("can task\n");
		}

}
//void StartDefaultTask1(void *argument) {
//int ledcnt = 0;
//for (;;) {
//	osDelay(100);
//	//osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
//	LED1_T;
////	ledcnt++;
////	if (ledcnt == 25) {
////		ledcnt = 0;
////		LED1_T;
////	}
////	if(WENDUCHIP==31855) Read_MAX31855(&Max31855TypeDef1);
////	else if(WENDUCHIP==6675)Read_MAX6675(&Max6675TypeDef1);
////	else AlarmFlag|=TEMP_ERROR;
//	/*
//	 HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_RESET);//SPI_CS_0;
//	 HAL_SPI_Receive(&hspi1,(uint8_t*)&TemperatureSampleValue,2,0xffff);
//	 HAL_GPIO_WritePin(SPI_CS_GPIO_Port,SPI_CS_Pin,GPIO_PIN_SET);//SPI_CS_1;
//	 */
//	/*
//	 for(i=0;i<TEMBUF_LEN-1;i++) tem_buf[i]=tem_buf[i+1];
//	 tem_buf[i]=	temperature>>3;
//	 sum=0;
//	 for(i=0;i<TEMBUF_LEN;i++) sum+=tem_buf[i];
//	 TemperatureSampleValue=sum/TEMBUF_LEN;
//	 */
//	/*
//	 if((temperature&0x4)==0)
//	 {
//	 for(i=0;i<TEMBUF_LEN-1;i++) tem_buf[i]=tem_buf[i+1];
//	 tem_buf[i]=	temperature>>3;
//	 sum=0;
//	 for(i=0;i<TEMBUF_LEN;i++) sum+=tem_buf[i];
//	 TemperatureSampleValue=sum/TEMBUF_LEN;
//	 }
//	 else
//	 {
//	 for(i=0;i<TEMBUF_LEN;i++) tem_buf[i]=0;
//	 TemperatureSampleValue=0xffff;
//	 }
//	 */
//}
//}
//void StartAdcTask(void *argument) {
//uint16_t ledcnt = 0;
//uint16_t kzCmdBuf[3];
//
//int period;
//if (MasterFlag==1)
//	period = 50;
//else
//	period = 10;
//
//motorInit();
//
//for (;;) {
//	osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
//
//
//	//LED_ON;
//	if (USE_FFT_METHOD==1234)
//	{
//		calc_fft_q15(FFT_ALL_IN,AllSampleBuf);//76us
////		calc_avg(FFT_ALL_IN,AllSampleBuf);	//
//	}
//	else
//	{
//		calc_avg(FFT_ALL_IN,AllSampleBuf);	//
//	}
//	//LED_OFF;
//
//	calc_pinlv();	//2us
//
//	read_KinOutSw();
//
//	bengRun(&Beng, &bengPid);		//0.65-0.8 us
//	madaRun(&Mada, &madaPid);
//
//	ledcnt++;
//	if (ledcnt >= period) {
//		ledcnt = 0;
//		LED_T;
//		LED0_T;
//	}
//
//	RunTime++;
//
////--------------------------------------------------------------
//	if (JSF_YALI_BAOHU) {
//		if(AllSampleBuf[XITONG_YALI_CHN]>JSF_YALI_BAOHU) {
//			JSF_OFF;
//		}
//	}
//	if (JSF_ZHUNASU_BAOHU)
//	{
//		if(AllSampleBuf[MADA_ZHUANSU_CHN]>JSF_ZHUNASU_BAOHU) {
//			JSF_OFF;
//		}
//	}
//
//	if (MADA_RESET_YALI)		//mada auto reset
//	{
//		if(Out_Cmd[Mada.norCmdChn]==K_CMD_ON && AllSampleBuf[XITONG_YALI_CHN]>MADA_RESET_YALI)
//		{
//			kzCmdBuf[0]=(1 << Mada.norCmdChn) | (1 << Mada.revCmdChn);//turn off
//			kzCmdBuf[1]=(1 << Mada.rstCmdChn);//turn on
//			kzCmdBuf[2]=Mada.rstDelayTime;//delay
//			ProcDJCmd((uint8_t *)kzCmdBuf);
//		}
//	}
////--------------------------------------------------------------
//
//}
//}
void StartUsart1Task(void *argument) {
	for (;;) {
		osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
		//HAL_UART_Transmit(&huart1,UsartType1.RxBuf,UsartType1.rx_len,HAL_MAX_DELAY);
//		ParseUsartModBusData(&UsartType1);
//  printf("12345");
		//xResult=xTaskNotifyWait(0,0,&NotifyValue,portMAX_DELAY);
		//uint32_t flags=osThreadFlagsSet(usart3TaskHandle,0x01U);

		//Uart1ParseCmd(UsartType1.RxBuf,UsartType1.rx_len);
		//ParseRxDataMbus(&UartRxData);
		//ParseRxData(&UartRxData);

//		UsartType1.RxBuf[UsartType1.rx_len]='\0';
//
//		for(int i=0;i<UsartType1.rx_len;i++)
//		{
//			uint8_t Value=UsartType1.RxBuf[i];
//			switch(Value)
//			{
//				case 0x31:
//				xResult=CAN_Transmit(&hcan1,0,0,"12345678",6);
//				printf("---------CAN1 err=%d\r\n",xResult);
//				break;
//
//				case 0x32:
//				print_id();
//				printf("s=%d\r\n",getChipidSum());
//				break;
//
//				case 0x33:
//					cct++;
//
//				if((Max6675TypeDef1.Result&4)) printf("---------------\r\n");
//				printf("\t%.1f\r\n",(float)(Max6675TypeDef1.Result>>3)/4);
//
//					//if((Max6675TypeDef1.Result&4)) printf("---------------\r\n");
//					//printf("\t%x %d\r\n",Max31855TypeDef1.Result,Max31855TypeDef1.Result>>18);
//				break;
//				case 0x34:
//					print_adc();
//				//psd();
//					printf("cnt=%d\r\n",cct);
//					break;
//				case 0x35:
//					print_pl();
//				cct=0;
//					break;
//				case 0x38:
//					SendSampleData();
//				break;
//				case 0x39:
//					HAL_NVIC_SystemReset();
//					break;
//			}
//		}

	}		//for
}


//void StartUsart3Task(void *argument) {
//	ReciveDataTypeDef Esp_Rx;
//	uint8_t *str;
//
//	for (;;) {
//		osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
//
//		UsartType3.RxBuf[UsartType3.rx_len] = 0;
//		str = UsartType3.RxBuf;
//		//HAL_UART_Transmit(&huart1,(uint8_t*)str,UsartType3.rx_len,0xffff);
//
//		if (strstr((char*) str, "+IPD,"))	//
//				{
//			ParseEspRxData(str, &Esp_Rx);
//			osMessageQueuePut(canRxQueueHandle, &Esp_Rx, 0, 0);
//
//		} else if (strstr((char*) str, ">")) {
//			ESP_Status_Flag |= ESP_SEND_READY;
//		} else if (strstr((char*) str, "SEND OK")) {
//			ESP_Status_Flag |= ESP_SEND_OK;
//			//printf("send ok flag=1\r\n");
//		} else if (strstr((char*) str, "OK")) {
//			ESP_Status_Flag |= ESP_OK;
//		} else if (strstr((char*) str, "ready")) {
//			ESP_Status_Flag |= ESP_RST_READY;
//		}
//		/*
//		 else if(strstr((char*)str,"CONNECT"))
//		 {
//		 //printf("\r\n---------new------------\n");
//		 HAL_UART_Transmit(&huart1,UsartType3.RxBuf,UsartType3.rx_len,0xffff);
//		 }
//		 else if(strstr((char*)str,"CLOSED"))
//		 {
//		 //printf("\r\n---------new------------\n");
//		 HAL_UART_Transmit(&huart1,UsartType3.RxBuf,UsartType3.rx_len,0xffff);
//		 }
//		 */
//	}
//}
//void StartCan1Task(void *argument) {
//	CAN_MSG can_msg;
//	for (;;) {
//		osStatus_t status = osMessageQueueGet(canMsgQueueHandle, &can_msg, NULL,
//		portMAX_DELAY);
////printf("can task status=%x\n",status);
//		//xStatus = xQueueReceive(canRxQueueHandle,&can_msg, portMAX_DELAY );
//		if (status == osOK) {
//			CanMsgProc(&can_msg);
//			//if(can_msg.hcan==&hcan1)	LED1_T;
//			//if(can_msg.hcan==&hcan2)	LED2_T;
////printf("can task\n");
//		}
//
//		//get_can_msg(&can_msg);
//		//CanMsgProc(&can_msg);
////buf[0]=0xa5;
////buf[1]=0x5a;
////HAL_UART_Transmit(&huart1,buf,2,0xFFFF);
//
//		/*
//
//		 printf("Reviced RTR=%d\n",can_msg.RxHead.RTR);
//		 printf("Reviced IDE=%d\n",can_msg.RxHead.IDE);
//
//		 if(can_msg.RxHead.IDE==CAN_ID_STD)
//		 printf("StdId=%x\n",can_msg.RxHead.StdId);
//		 if(can_msg.RxHead.IDE==CAN_ID_EXT)
//		 printf("ExtId=%x\n",can_msg.RxHead.ExtId);
//
//		 printf("Reviced Remote DLC=%d\n",can_msg.RxHead.DLC);
//
//		 printf("CAN Recived data:");
//		 for(i=0;i<can_msg.RxHead.DLC;i++)	printf("%c",can_msg.RxBuffer[i]);
//		 printf("\n");
//		 */
//
//	}
//}
//void StartEspTask(void *argument) {
//	osDelay(2000);
//	if (esp8266_initAP())
//		AlarmFlag |= WIFI_ERROR;
//	else
//		AlarmFlag &= ~WIFI_ERROR;
//
//	//if(eret==0)
//	//	printf("\r\ninit ap ok\r\n");
//	//else printf("\r\ninit ap fail  -------%d\r\n",eret);
//
//	ReciveDataTypeDef EspRxData;
//	//printf("----------Task_Of_Esp\r\n");
//
//	for (;;) {
//		osStatus_t status = osMessageQueueGet(espCmdQueueHandle, &EspRxData,
//		NULL, portMAX_DELAY);
//		if (status == osOK) {
//
//			//printf( "\r\nclientID=%d\r\n", EspRxData.clientID );//???????
//			//printf( "dataSize=%d\r\n", EspRxData.dataSize );//???????
//			//printf( "dataBuffer=%s\r\n", EspRxData.pData );//???????
//
//			//HAL_UART_Transmit(&huart1,esp_rcv.pData,esp_rcv.dataSize,0xffff);
//			//esp_rcv
//
//			memcpy(EspTxBuffer, EspRxData.pData, EspRxData.dataSize);
//			EspRxData.type = TYPE_ESP;
//			EspRxData.pData = EspTxBuffer;
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
//	}
//	/* USER CODE END Task_Of_Esp */
//}

