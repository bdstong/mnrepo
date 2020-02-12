#include "taskApp.h"
uint8_t EspBuffer[512];

extern uint16_t LocalAddr;
extern uint16_t Parameters[];
extern uint16_t AllSampleBuf[];
extern uint16_t MasterFlag;

extern uint16_t KinStatus,KoutStatus;
extern uint16_t Out_Cmd[];
extern uint16_t SWStatus;
extern uint8_t bengTest,madaTest;
extern uint16_t AlarmFlag;
extern uint32_t RunTime;
extern MPID bengPid,madaPid;

int CteateSampleData(uint16_t *pbuf)
{
int j;
	read_KinOutSw();
	j=16;//前16个是 8路模拟量+8路频率
	pbuf[j++]=SWStatus;
	
	//16个开入量
	pbuf[j++]=KinStatus;
	
	//16个开出量
	pbuf[j++]=KoutStatus;
	pbuf[j++]=AlarmFlag;
	pbuf[j++]=RunTime;

	uint16_t tmp=HAL_DAC_GetValue(&hdac,DAC_CHANNEL_1);
	pbuf[j++]=tmp;
	
	pbuf[j++]=0;//TemperatureValue;
	pbuf[j++]=0;//TemperatureError;
	
/////////used for test 11 words//////////////////////////////////
	pbuf[j++]=bengPid.Target;
	pbuf[j++]=bengPid.Period;
	pbuf[j++]=bengPid.Gain;
	pbuf[j++]=bengPid.Kp;
	pbuf[j++]=bengPid.Ki;
	pbuf[j++]=bengPid.Kd;

	pbuf[j++]=bengTest;
	pbuf[j++]=madaTest;

	pbuf[j++]=bengPid.AbsOfIncrease;
	pbuf[j++]=bengPid.PrdCnt;
	pbuf[j++]=0;
//36
	return j;
}

/*
 错误返回
1  地址
2  功能码=请求码+0x80
3  错误码
4 CRC 2个字节 低位在前

错误码
0x01 不支持该功能码
0x02 越界
0x03 寄存器数量超出范围
0x04 读写错误
*/
#define NOT_SUPPORT 	0X01
#define OVER_STEP		0X02
#define REGISTER_ERR	0X03
#define WR_ERR			0X04

int makeErrorReturn(uint8_t funCode,uint8_t errCode,uint8_t *pDstBuf){
	int j=0;
	pDstBuf[j++]=LocalAddr;			//地址
	pDstBuf[j++]=(funCode+0x80);	//功能码
	pDstBuf[j++]=errCode;			//错误码

	uint16_t crc=CRC16(pDstBuf,j);
	pDstBuf[j++]=crc;				//校验
	pDstBuf[j++]=crc>>8;

	return j;
}
//返回原报文的前6个字节+CRC
int ReturnCopy(uint8_t * pData,uint8_t *pDstBuf)
{
	memcpy(pDstBuf,pData,6);
	uint16_t crc=CRC16(pDstBuf,6);
	pDstBuf[6]=crc;			//校验
	pDstBuf[7]=crc>>8;

	return 8;
}

//读寄存器返回
int makeReturnData(uint8_t funCode,uint16_t* pSrcBuf,uint8_t regNUm,uint8_t *pDstBuf)
{
	int j=0;
	pDstBuf[j++]=LocalAddr;	//地址
	pDstBuf[j++]=funCode;	//功能码
	pDstBuf[j++]=regNUm*2;	//数据长度=寄存器个数*2

	//数据区 高位在前
	for(int i=0;i<regNUm;i++)
	{
		pDstBuf[j++]=pSrcBuf[i]>>8;
		pDstBuf[j++]=pSrcBuf[i];
	}

	uint16_t crc=CRC16(pDstBuf,j);
	pDstBuf[j++]=crc;		//校验
	pDstBuf[j++]=crc>>8;

	return j;
}
int makeReturnDataByte(uint8_t funCode,uint8_t* pSrcBuf,uint8_t len,uint8_t *pDstBuf)
{
	int j=0;
	pDstBuf[j++]=LocalAddr;	//地址
	pDstBuf[j++]=funCode;	//功能码
	pDstBuf[j++]=len;		//字节数
	
	//数据区 高位在前
	for(int i=0;i<len;i++)pDstBuf[j++]=pSrcBuf[i];

	uint16_t crc=CRC16(pDstBuf,j);
	pDstBuf[j++]=crc;		//校验
	pDstBuf[j++]=crc>>8;
	
	return j;
}

//1 读线圈
int MbusReadCoils(uint8_t *pData,uint8_t *pDstBuf)//regNumber 表示多少位
{
	uint8_t FunctionCode;
	uint16_t Register,regNumber;
	FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;	//寄存器地址
	Register|=pData[3];
	regNumber=pData[4]<<8;
	regNumber|=pData[5];

	int size=0;
	uint16_t DoBuf[16];
	switch(Register){
	case InputsRegister:
		DoBuf[0]=KoutStatus;
		DoBuf[1]=KoutStatus;
		uint16_t len=regNumber/8;
		if(regNumber%8) len++;
		size=makeReturnDataByte(ReadCoils,(uint8_t*)DoBuf,len,pDstBuf);
		break;
	default:
		size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
		break;
	}
	return size;
}

//2 读输入点 开入量
int MbusReadInputs(uint8_t *pData,uint8_t *pDstBuf)//regNumber 表示多少位
{
	uint8_t FunctionCode;
	uint16_t Register,regNumber;
	FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;	//寄存器地址
	Register|=pData[3];
	regNumber=pData[4]<<8;
	regNumber|=pData[5];

	int size=0;
	uint16_t DiBuf[16];
	switch(Register){
	case InputsRegister:
		DiBuf[0]=KinStatus;
		uint16_t len=regNumber/8;
		if(regNumber%8) len++;
		size=makeReturnDataByte(ReadInputs,(uint8_t*)DiBuf,len,pDstBuf);
		break;
	default:
		size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
		break;
	}
	return size;
}

//3 读保持寄存器
int MbusReadHoldingRegisters(uint8_t *pData,uint8_t *pDstBuf)
{
	uint8_t FunctionCode;
	uint16_t Register,regNumber;
	FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;	//寄存器地址
	Register|=pData[3];
	regNumber=pData[4]<<8;
	regNumber|=pData[5];

	int size=0;
	if(Register>=ParameterRegister){
		size=makeReturnData(ReadHoldingRegisters,&Parameters[Register-ParameterRegister],regNumber,pDstBuf);
	}
	else{
		size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
	}
	return size;
}

//4 读输入寄存器
int MbusReadInputRegisters(uint8_t *pData,uint8_t *pDstBuf)
{
	uint8_t FunctionCode;
	uint16_t Register,regNumber;
	FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;	//寄存器地址
	Register|=pData[3];
	regNumber=pData[4]<<8;
	regNumber|=pData[5];

	int size=0;
	if(Register>=SampleRegister){
		CteateSampleData(AllSampleBuf);//36个字
		size=makeReturnData(ReadInputRegisters,&AllSampleBuf[Register-SampleRegister],regNumber,pDstBuf);
	}
	else{
		size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
	}
	return size;
}


//5 写单个线圈
int MbusWriteSingleCoil(uint8_t *pData,uint8_t *pDstBuf)//opCode true-0xff00 false-0x0000
{
	uint8_t FunctionCode;
	uint16_t CoilIndex,opCode;
	FunctionCode=pData[1];	//功能码
	CoilIndex=pData[2]<<8;	//寄存器地址
	CoilIndex|=pData[3];
	opCode=pData[4]<<8;
	opCode|=pData[5];

	int size=0;
	if(CoilIndex<TOTAL_OUT){
		if(opCode==0)	//0X0000
			Out_Cmd[CoilIndex]=K_CMD_OFF;
		else			//0XFF00
			Out_Cmd[CoilIndex]=K_CMD_ON; //0xff00
		size=ReturnCopy(pData,pDstBuf);
	}
	else{
		size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
	}
	return size;
}

//6 写单个寄存器
int MbusWriteSingleRegister(uint8_t * pData,uint8_t *pDstBuf)
{
	uint8_t FunctionCode;
	uint16_t Register,regValue;
	//StationID=pData[0];	//地址码
	FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;	//寄存器地址 高位在前
	Register|=pData[3];
	regValue=pData[4]<<8;	//寄存器的值
	regValue|=pData[5];

	int size=0;
	switch(Register){
	case StopMotorRegister:
		StopMotor(regValue);
		size=ReturnCopy(pData,pDstBuf);
		break;
	default:
		size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
		break;
	}
	return size;
}

//15 写多个线圈
int MbusWriteMultipleCoils(uint8_t * pData,uint8_t *pDstBuf)
{
	//uint8_t FunctionCode;
	uint16_t Register,regNumber;
	//StationID=pData[0];		//地址码
	//FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;		//寄存器地址
	Register|=pData[3];
	regNumber=pData[4]<<8;		//寄存器个数
	regNumber|=pData[5];

	int size=0;
	//size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
	size=ReturnCopy(pData,pDstBuf);
	return size;
}
uint16_t GetModBusWord(uint8_t *pData)
{
	uint16_t temp;
	temp=pData[0]<<8;
	temp|=pData[1];
	return temp;
}
//16 写多个寄存器
int MbusWriteMultipleRegisters(uint8_t*pData,uint8_t *pDstBuf)
{
	uint8_t FunctionCode;
	uint16_t Register,regNumber;
	//StationID=pData[0];		//地址码
	FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;		//寄存器地址 16bit
	Register|=pData[3];
	regNumber=pData[4]<<8;		//寄存器个数
	regNumber|=pData[5];
//	uint8_t datalen=pData[6];	//数据长度=寄存器个数*2

	int size=0;
	//写参数
	if(Register>=ParameterRegister && Register<(ParameterRegister+PARAMETER_NUM))
	{
		if(regNumber>(PARAMETER_NUM+ParameterRegister-Register)) return 2;

		uint8_t *pbuf=pData+7;	//数据从第8个字节开始(0开始)
		for(int i=0;i<regNumber;i++)
		{
			Parameters[Register-ParameterRegister+i]=GetModBusWord(pbuf);
			pbuf+=2;
		}
		osThreadFlagsSet(defaultTaskHandle,0x01U);
		//Write_All_Parameters();
		size=ReturnCopy(pData,pDstBuf);
		return size;
	}
	
	switch(Register){
	case PIDCmdRegister:
		ProcModBusPidCmd(pData);
		size=ReturnCopy(pData,pDstBuf);
		break;
	case MotorCmdRegister:
		ProcModBusDJCmd(pData);
		size=ReturnCopy(pData,pDstBuf);
		break;
	case KZRunCmdRegister:
		ProcKZRunCmdBuf(pData);
		size=ReturnCopy(pData,pDstBuf);
		break;
	default:
		size=makeErrorReturn(FunctionCode,REGISTER_ERR,pDstBuf);
		break;
	}
	return size;
}

//返回字节数
int ParseModeBusData(uint8_t * pData,uint16_t dataSize,uint8_t *pDstBuf)
{
	if(CRC16(pData,dataSize)!=0) return 0;

	uint8_t StationID,FunctionCode;
	uint16_t Register,regNumber;
	StationID=pData[0];		//地址码
	FunctionCode=pData[1];	//功能码
	Register=pData[2]<<8;	//寄存器地址 16bit
	Register|=pData[3];
	regNumber=pData[4]<<8;	
	regNumber|=pData[5];
	
	if(StationID!=LocalAddr)	//不是本机地址
	{
		return 0;
	}

	int size;
	switch(FunctionCode)
	{
		case ReadInputs:				//2 读输入
			size=MbusReadInputs(pData,pDstBuf);
			break;
		case ReadCoils:
			size=MbusReadCoils(pData,pDstBuf);
			break;
		case ReadHoldingRegisters:		//3 读多个寄存器
			size=MbusReadHoldingRegisters(pData,pDstBuf);
		break;

		case ReadInputRegisters:		//4
			size=MbusReadInputRegisters(pData,pDstBuf);
			break;
		
		case WriteSingleCoil:			//5 单路开关输出
			size=MbusWriteSingleCoil(pData,pDstBuf);
			break;
		
		case WriteSingleRegister:		//6 单路模拟量输出
			size=MbusWriteSingleRegister(pData,pDstBuf);
			break;

		case WriteMultipleCoils:		//15
			size=MbusWriteMultipleCoils(pData,pDstBuf);
			break;
		case WriteMultipleRegisters:	//16	写 多个寄存器
			size=MbusWriteMultipleRegisters(pData,pDstBuf);
			break;
		
		default:
			size=makeErrorReturn(FunctionCode,NOT_SUPPORT,pDstBuf);
			break;
	}
	return size;
}


//通过串口发送数据
//void SendDataBySerial(ReciveDataTypeDef *pRx)
//{
//	if(pRx->type==TYPE_SER)
//	{
//		HAL_UART_Transmit(pRx->port,pRx->pData,pRx->dataSize,0xFFFF);
//	}
//	if(pRx->type==TYPE_ESP)
//	{
//		pRx->pData=EspBuffer;
//		esp8266_Data(pRx);
//	}
//}
//接收到的CAN数据 转发给PC
//void ReSendCanMsgToPC(ReciveDataTypeDef *pRx,uint8_t *buf,int len){
//	pRx->dataSize=len;
//	pRx->pData=buf;
//	if(pRx->type==TYPE_SER)
//	{
//		HAL_UART_Transmit(pRx->port,pRx->pData,pRx->dataSize,0xFFFF);
//	}
//	if(pRx->type==TYPE_ESP)
//	{
//		esp8266_Data(pRx);
//	}
//}

