#include "TaskApp.h"

//extern CAN_MSG can_msg;
extern USART_DATA UsartType1;
extern USART_DATA UsartType3;

//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan) {
//	if (hcan->Instance == CAN1) {
//		HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &can_msg.RxHead,
//				can_msg.RxBuffer);
//		can_msg.hcan = hcan;
//		//osStatus_t status=
//		osMessageQueuePut(canMsgQueueHandle, &can_msg, 0, 0);
////printf("put status=%x\n",status);
////		printf("123\n");
////CanMsgProcMbus(&can_msg);
//
//	}
//}

//空闲中断
void UART1_IDLECallBack(void) {
	HAL_UART_DMAStop(&huart1);
	UsartType1.rx_len = USART_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart1.hdmarx);
	HAL_UART_Receive_DMA(&huart1, UsartType1.RxBuf, USART_BUF_SIZE);
	//uint32_t rflags=
	//osThreadFlagsSet(usart1TaskHandle,0x01U);
	LED1_T;//
	osThreadFlagsSet(ethTaskHandle,0x01U);

//	printf("usart1 flag=%d\n",rflags);
//	HAL_UART_Transmit(&huart1, UsartType1.RxBuf, UsartType1.rx_len,HAL_MAX_DELAY);

//	ParseUsartModBusData(&UsartType1);

//	uint8_t pdata[4];//={0x12,0x34};
//	pdata[0]=(uint8_t)rflags;
//	pdata[1]=(uint8_t)(rflags>>8);
//	pdata[2]=(uint8_t)(rflags>>16);
//	pdata[3]=(uint8_t)(rflags>>24);
//	HAL_UART_Transmit(&huart1, pdata, 4,HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, UsartType1.RxBuf, 8,HAL_MAX_DELAY);
//	HAL_UART_Transmit(&huart1, UsartType3.RxBuf, 8,HAL_MAX_DELAY);
//   printf("rt=%x\n",0x12345678U);
}
void UART3_IDLECallBack(void) {
	HAL_UART_DMAStop(&huart3);
	UsartType3.rx_len = USART_BUF_SIZE - __HAL_DMA_GET_COUNTER(huart3.hdmarx);
	UsartType3.cmd_rcv_flag = 1;
	HAL_UART_Receive_DMA(&huart3, UsartType3.RxBuf, USART_BUF_SIZE);
	//uint32_t rflags=osThreadFlagsSet(usart3TaskHandle,0x01U);
}

//memory to  memory DMA complete callback
//void DMATransferComplete(DMA_HandleTypeDef *hdma)
//{
////	//MEM-TO-MEMÍê³Éºó·¢ËÍÍ¨Öª->ADC´¦ÀíÈÎÎñ
////	BaseType_t xHigherPriorityTaskWoken;
////	xHigherPriorityTaskWoken = pdFALSE;
////	vTaskNotifyGiveFromISR(adcProcessHandle,&xHigherPriorityTaskWoken );//²»´øÍ¨ÖªÖµ
////	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//	uint32_t ret=osThreadFlagsSet(adcTaskHandle,0x01U);
//}

//ADC DMA´«ÊäÍê
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//	HAL_DMA_Start_IT(&hdma_memtomem_dma2_stream1, (uint32_t)ADC_DMA_BUF, (uint32_t)FFT_ALL_IN, ADC_NUMBER*NPT);//use FFT
//
////	uint16_t i,j,sum;
////
////	for(i=0;i<ADC_NUMBER;i++)
////	{
////		for(j=0;j<FNT-1;j++)
////		{
////			adcbuf[i][j]=adcbuf[i][j+1];
////		}
////		adcbuf[i][j]=ADC_DMA_BUF[i];
////	}
////
////	adccnt++;
////	if(adccnt<FNT) return;
////
////	adccnt=0;
////	for(i=0;i<ADC_NUMBER;i++)//ADCÆ½¾ùÖµ
////	{
////		sum=0;
////		for(j=0;j<FNT;j++)
////		{
////			sum+=adcbuf[i][j];
////		}
////		AllSampleBuf[i]=sum/FNT;
////
////		//AllSampleBuf[i]=ADC_DMA_BUF[i];
////	}
//
//}

//void clrCapturreFlag()
//{
//	for(int i=0;i<8;i++)
//	{
//		//ChannelFlag[i]=0;
////		ChannelFreq[i]=0;
//		//TimUpdateFlag[i]=0;
//		AllSampleBuf[ADC_NUMBER+FREQ_NUMBER+i]=0;
//	}
//}

//ÍâÖÐ¶Ï»Øµ÷º¯Êý

//void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
//{
//
//	switch(GPIO_Pin)
//	{
//		case GPIO_PIN_3:
//			if(HAL_GPIO_ReadPin(EXIT3_GPIO_Port,EXIT3_Pin)==GPIO_PIN_SET)
//				{//up raise
//					start_tim=LL_TIM_GetCounter(htim2.Instance);
//				}
//				else
//				{
//					end_tim=LL_TIM_GetCounter(htim2.Instance);
//					dur=end_tim-start_tim;
//					//if(dur<2350)
//					{
//						distance=(float)340*dur/1000000/2;
//					}
//				}
//				//
//				break;
//			case GPIO_PIN_4:
//			//	LED1_Toggle();
//				break;
//			default:break;
//    }
//
//}


//void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
//{
//	BaseType_t xHigherPriorityTaskWoken;
//	if(hcan->Instance==CAN2)
//  {
//		HAL_CAN_GetRxMessage(hcan,CAN_RX_FIFO1,&can_msg2.RxHead,&(can_msg2.RxBuffer[0]));
//		can_msg2.hcan=hcan;
//		xQueueSendToBackFromISR(canRxQueueHandle,&can_msg2,&xHigherPriorityTaskWoken );
//		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//  }
//}
//
//
//
////´®¿ÚDMA·¢ËÍÍê³ÉºóµÄ»Øµ÷º¯Êý,´®¿Ú±ØÐë¿ªÆôÖÐ¶Ï£¬·ñÔòDMAÖ»·¢ËÍÒ»´Î
//void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART3)
//	{
//	//·¢ËÍÍ¨Öª->´®¿Ú·¢ËÍÈÎÎñ
//	BaseType_t xHigherPriorityTaskWoken;
//	xHigherPriorityTaskWoken = pdFALSE;
//	vTaskNotifyGiveFromISR(xPinrtTastHandle,&xHigherPriorityTaskWoken );//²»´øÍ¨ÖªÖµ
//	portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//}
//
////	HAL_UART_Receive_DMA(&huart1,UsartType1.usartDMA_rxBuf, 1);//RECEIVELEN);//
//
//
////´®¿Ú (Ò²°üÀ¨DMA) ½ÓÊÕÍê³ÉºóµÄ»Øµ÷º¯Êý
//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//	if(huart->Instance==USART1)
//	{
//		BaseType_t xHigherPriorityTaskWoken;
//	//vTaskNotifyGiveFromISR(uart1ReciveHandle,&xHigherPriorityTaskWoken );//²»´øÍ¨ÖªÖµ
//		xQueueSendToBackFromISR( uart1RxQueueHandle,&UsartType1.RxBuf[0],&xHigherPriorityTaskWoken );
//		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
//	}
//}
//
//
//void calcperiod(uint16_t chn,uint32_t capTime)
//{
//		ChannelNowtime[chn]=capTime;
//		if(ChannelFlag[chn]==0)//»¹Ã»¿ªÊ¼²¶»ñ
//		{
//			ChannelFlag[chn]=1;
//			TimUpdateFlag[chn]=0;
//		}
//		else//ÒÑ¾­²¶»ñÁËÒ»´Î£¬¿ÉÒÔ¼ÆËã
//		{
//			if(TimUpdateFlag[chn]==0)//Ã»ÓÐÒç³ö
//			{
//				ChannelPeriod[chn]=ChannelNowtime[chn]-ChannelLastime[chn];
//			}
//			else
//			{
//				ChannelPeriod[chn]=ChannelNowtime[chn]+(0xffff-ChannelLastime[chn])+1;
//				if(TimUpdateFlag[chn]>1)
//				{
//					ChannelPeriod[chn]+=65536*(TimUpdateFlag[chn]-1);
//				}
//			}
//			AllSampleBuf[ADC_NUMBER+FREQ_NUMBER+chn]=84000000l/ChannelPeriod[chn];
//		}
//		ChannelLastime[chn]=ChannelNowtime[chn];
//		TimUpdateFlag[chn]=0;
//}

//²¶»ñ»Øµ÷º¯Êý
//void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
//{
////	uint32_t temp;
////	__HAL_TIM_GET_COMPARE(htim->Instance,TIM_CHANNEL_1);
//	if(htim->Instance==TIM4)//tim4 16bit
//	{
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4)//±ÃÅ¤¾Ø
//		{
//			PL_CNT1++;
//			//temp=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_4);
//			//calcperiod(0,temp);
//		}
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3)//±Ã×ªËÙ
//		{
//			PL_CNT2++;
//			//temp=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_3);
//			//calcperiod(1,temp);
//		}
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)//Âí´ïÅ¤¾Ø
//		{
//			PL_CNT3++;
//			//temp=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
//			//calcperiod(2,temp);
//		}
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)//Âí´ï×ªËÙ
//		{
//			PL_CNT4++;
//			//temp=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
//			//calcperiod(3,temp);
//		}
//	}
//
//	if(htim->Instance==TIM3)//tim3 16bit
//	{
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_2)
//		{
//			PL_CNT5++;
//			//temp=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_2);
//			//calcperiod(4,temp);
//		}
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_1)
//		{
//			PL_CNT6++;
//			//temp=HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
//			//calcperiod(5,temp);
//		}
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_4) PL_CNT7++;
//		if(htim->Channel==HAL_TIM_ACTIVE_CHANNEL_3) PL_CNT8++;
//	}
//
//}
#define CLR_CNT	640
void HAL_TIM_PeriodElapsedCallback1(TIM_HandleTypeDef *htim) {
	//UNUSED(htim);
//	if(htim->Instance==TIM14)//2kHz
//	{//total 4.25us max
//		Out_PWM_Proc();//3.25us max
//		if(outDlaeyPorcCnt<1){
//			outDlaeyPorcCnt++;//0.12us
//		}
//		else{
//			outDlaeyPorcCnt=0;
//			Out_Delay_Proc();
//		}
//	}

//	if(htim->Instance==TIM4)
//	{
//		//³¬Ê±ÇåÁãÆµÂÊ
//		for(int i=0;i<4;i++)
//		{
//			if(TimUpdateFlag[i]<CLR_CNT)TimUpdateFlag[i]++;
//			else
//			{
//				ChannelFlag[i]=0;
//				AllSampleBuf[ADC_NUMBER+FREQ_NUMBER+i]=0;
//			}
//		}
//	}

//	if(htim->Instance==TIM3)
//	{
//		for(int i=4;i<8;i++)
//		{
//			if(TimUpdateFlag[i]<CLR_CNT)TimUpdateFlag[i]++;
//			else
//			{
//				ChannelFlag[i]=0;
//				AllSampleBuf[ADC_NUMBER+FREQ_NUMBER+i]=0;
//			}
//		}
//	}

}

