#include "taskApp.h"

extern uint16_t Out_Cmd[],Out_On_Delay[];
extern MOTOR Beng,Mada;
extern MPID bengPid,madaPid;
extern uint16_t AllSampleBuf[];

uint16_t GetExchangeWord(uint16_t orgWord)//高8位和低8位交换 0x1234->0x3412
{
	uint8_t h_byte,l_byte;
	l_byte=orgWord>>8;
	h_byte=orgWord;
	return((h_byte<<8)|l_byte);
}
void ProcModBusDJCmd(uint8_t *pData)
{
	uint16_t offCmd,onCmd,delay;

	pData+=7;	//数据从第8个字节开始(0开始)
	offCmd=GetModBusWord(pData);
	pData+=2;
	onCmd=GetModBusWord(pData);
	pData+=2;
	delay=GetModBusWord(pData);
	pData+=2;

	for(int i=0;i<TOTAL_OUT;i++){
		if((offCmd&(1<<i))==0) continue;
		Out_Cmd[i]=K_CMD_OFF;
	}
	
	for(int i=0;i<TOTAL_OUT;i++){
		if((onCmd&(1<<i))==0) continue;
		Out_On_Delay[i]=delay;
		Out_Cmd[i]=K_CMD_ON;
	}
}
void ProcKZRunCmdBuf(uint8_t *pData)
{
	pData+=7;	//数据从第8个字节开始

	uint16_t Target=GetModBusWord(pData);pData+=2;
	uint16_t Period=GetModBusWord(pData);pData+=2;
	uint16_t norTime=GetModBusWord(pData);pData+=2;
	uint16_t revTime=GetModBusWord(pData);pData+=2;
	uint16_t WhichMotor=GetModBusWord(pData);pData+=2;
	uint16_t Chn=GetModBusWord(pData);pData+=2;
	uint16_t NormalFlag=GetModBusWord(pData);pData+=2;
	uint16_t Range=GetModBusWord(pData);pData+=2;

	MOTOR *pMotor;
	MPID *pPid;
	if(WhichMotor==1)//1-Beng
	{
		pMotor=&Beng;
		pPid=&bengPid;
	}
	else
	{
		pMotor=&Mada;
		pPid=&madaPid;
	}

	pMotor->TargetValue=Target;
	pMotor->normalTime=norTime;
	pMotor->reverseTime=revTime;
	pMotor->range=Range;
	if(Period<10) pMotor->period=1;
	else pMotor->period=Period/10;

	if(NormalFlag)
	{
		pMotor->pCurrent=(int16_t*)&AllSampleBuf[Chn];
		pMotor->pTarget=&(pMotor->TargetValue);
	}
	else
	{
		pMotor->pTarget=(int16_t*)&AllSampleBuf[Chn];
		pMotor->pCurrent=&(pMotor->TargetValue);
	}
	
	pMotor->cmd=M_CMD_RUN;
}

void ProcModBusPidCmd(uint8_t *pData)
{
	pData+=7;	//数据从第8个字节开始
	
	uint16_t Target=GetModBusWord(pData);pData+=2;
	uint16_t Period=GetModBusWord(pData);pData+=2;
	uint16_t PID_KKK=GetModBusWord(pData);pData+=2;
	uint16_t PID_P=GetModBusWord(pData);pData+=2;
	uint16_t PID_I=GetModBusWord(pData);pData+=2;
	uint16_t PID_D=GetModBusWord(pData);pData+=2;
	uint16_t WhichMotor=GetModBusWord(pData);pData+=2;	//1-泵 2-马达
	uint16_t Chn=GetModBusWord(pData);pData+=2;					//哪一路采样作为反馈
	uint16_t NormalFlag=GetModBusWord(pData);pData+=2;	//1-正逻辑 0-反逻辑
	uint16_t Range=GetModBusWord(pData);pData+=2;

	MOTOR *pMotor;
	MPID *pPid;
	if(WhichMotor==1)//1-Beng
	{
		pMotor=&Beng;
		pPid=&bengPid;
	}
	else
	{
		pMotor=&Mada;
		pPid=&madaPid;
	}
	
	initPidAllPara(pPid,PID_KKK,PID_P,PID_I,PID_D);
	pPid->Target=Target;
	pPid->Range=Range;
	if(Period<10) pPid->Period=1;
	else pPid->Period=Period/10;
	
	if(NormalFlag)//正向
	{
		pPid->pFeedback=(int16_t*)&AllSampleBuf[Chn];
		pPid->pSetPoint=(int16_t*)&(pPid->Target);
	}
	else//反向
	{
		pPid->pSetPoint=(int16_t*)&AllSampleBuf[Chn];
		pPid->pFeedback=(int16_t*)&(pPid->Target);
	}
	pMotor->cmd=M_CMD_PID;
}

void StopMotor(uint16_t motor)
{
	if(motor==1)//beng
	{
		Beng.cmd=M_CMD_STOP;
	}
	if(motor==2)//mada
	{
		Mada.cmd=M_CMD_STOP;
	}
}
