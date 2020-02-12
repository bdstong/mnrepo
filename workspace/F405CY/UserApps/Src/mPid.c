#include "TaskApp.h"

void clearPid(MPID* ptr);
int getPidInc(MPID* ptr);
void initPidAllPara(MPID* ptr,uint16_t K,uint16_t kp,uint16_t ki,uint16_t kd);


//uint16_t pidKp=245;
//uint16_t pidKi=350;
//uint16_t pidKd=125;

/*
PID
Pdlt=Kp*(e(k)-e(k-1))+Kp*(T/Ti)*e(k)+Kp*(Td/T)*(e(k)-2*e(k-1)+e(k-2))
    =A*e(k)-B*e(k-1)+C*e(k-2)
	A=Kp*(1+T/Ti+Td/T)
	B=Kp*(1+2*Td/T)
	C=Kp*Td/T
	�ٽ�������Tu���ٽ����ϵ��Ku,����Z-N��������
	T=0.1Tu
	Ti=0.5Tu
	Td=0.125Tu
PdIt=Kp*(2.45e(k)-3.5e(k-1)+1.25e(k-2)
*/
/*
void SetPIDKP(MPID* ptr,int K,int P)
{
	ptr->Kp=(float)K/100.0;
	ptr->Proportion=2.45*ptr->Kp;
	ptr->Intergral=3.5*ptr->Kp;
	ptr->Derivative=1.25*ptr->Kp;
	ptr->Period=P;
}
*/
/*
void IncPIDInit(MPID* ptr)
{
	ptr->SumError=0;
	ptr->LastError=0;
	ptr->PrevErroe=0;
	ptr->PrdCnt=0;
}
*/
//PID������ʼ��
/*
void Init_PID(MPID* ptr,int P,int K,int MinDY,int MaxDY,int CurDY)
{
	ptr->Kp=(float)K/100.0f;
	ptr->Proportion=2.45f*ptr->Kp;
	ptr->Intergral=3.5f*ptr->Kp;
	ptr->Derivative=1.25f*ptr->Kp;
	ptr->Period=P;
	ptr->SumError=0;
	ptr->LastError=0;
	ptr->PrevErroe=0;
	ptr->PrdCnt=0;
	
	ptr->Uk_Min=MinDY;
	ptr->Uk_Max=MaxDY;
	ptr->Uk=CurDY;
}
*/
void clearPid(MPID* ptr)
{
//	ptr->SumError=0;
	ptr->LastError=0;
	ptr->PrevErroe=0;
	ptr->PrdCnt=0;
}
void initPidAllPara(MPID* ptr,uint16_t K,uint16_t kp,uint16_t ki,uint16_t kd)
{
	ptr->Gain=K;
	ptr->Kp=kp;//(float)kp/1000.0;
	ptr->Ki=ki;//(float)ki/1000.0;
	ptr->Kd=kd;//(float)kd/1000.0;

	//ptr->Kp=(float)K/100.0f;
	ptr->Proportion=((float)ptr->Kp/1000)*ptr->Gain/100;//ptr->Kp;
	ptr->Intergral=((float)ptr->Ki/1000)*ptr->Gain/100;//ptr->Kp;
	ptr->Derivative=((float)ptr->Kd/1000)*ptr->Gain/100;//ptr->Kp;

	//	ptr->SumError=0;
	ptr->LastError=0;
	ptr->PrevErroe=0;
	ptr->PrdCnt=0;
}
/*
void initPid(MPID* ptr,uint16_t K)
{
	ptr->Gain=K;
	ptr->Kp=2450;//kp;//(float)kp/1000.0;
	ptr->Ki=2500;//ki;//(float)ki/1000.0;
	ptr->Kd=1250;//kd;//(float)kd/1000.0;

	ptr->Proportion=((float)ptr->Kp/1000)*ptr->Gain/100;//ptr->Kp;
	ptr->Intergral=((float)ptr->Ki/1000)*ptr->Gain/100;//ptr->Kp;
	ptr->Derivative=((float)ptr->Kd/1000)*ptr->Gain/100;//ptr->Kp;
//	ptr->SumError=0;
	ptr->LastError=0;
	ptr->PrevErroe=0;
	ptr->PrdCnt=0;
}
void initPidDefPara(MPID* ptr,uint16_t K)
{
	ptr->Gain=K;

	ptr->Kp=2450;
	ptr->Ki=3500;
	ptr->Kd=1250;

	ptr->Proportion=((float)ptr->Kp/1000)*ptr->Gain/100;//ptr->Kp;
	ptr->Intergral=((float)ptr->Ki/1000)*ptr->Gain/100;//ptr->Kp;
	ptr->Derivative=((float)ptr->Kd/1000)*ptr->Gain/100;//ptr->Kp;

	//	ptr->SumError=0;
	ptr->LastError=0;
	ptr->PrevErroe=0;
	ptr->PrdCnt=0;
}
*/

//����PID PdIt=Kp*(2.45e(k)-3.5e(k-1)+1.25e(k-2)
int getPidInc(MPID* ptr)
{
	int iError;

	iError=*ptr->pSetPoint-*ptr->pFeedback;		//��ǰ���
//	if(ptr->Enable_min_err)
//	{
//		if(abs(iError)<ptr->Min_error) iError=0;
//	}
	ptr->Increase=ptr->Proportion*iError			//e(k)
		   -ptr->Intergral*ptr->LastError				//e(k-1)
		   +ptr->Derivative*ptr->PrevErroe;			//e(k-2)

	ptr->PrevErroe=ptr->LastError;
	ptr->LastError=iError;
	ptr->AbsOfIncrease=abs(ptr->Increase);
	return ptr->Increase;
}
/*
//����PID PdIt=Kp*(2.45e(k)-3.5e(k-1)+1.25e(k-2)
void IncPIDCalc(MPID* ptr)
{
//	int iError,iIncpid;
//	iError=ptr->SetPoint-ptr->Feedback;//��ǰ���
//	iIncpid=ptr->Proportion*iError			//e(k)
//		   -ptr->Intergral*ptr->LastError	//e(k-1)
//		   +ptr->Derivative*ptr->PrevErroe;//e(k-2)
//	ptr->PrevErroe=ptr->LastError;
//	ptr->LastError=iError;

	ptr->Uk+=getPidInc(ptr);//iIncpid
	if(ptr->Uk>ptr->Uk_Max) ptr->Uk=ptr->Uk_Max;
	if(ptr->Uk<ptr->Uk_Min) ptr->Uk=ptr->Uk_Min;
}
*/
/*
//λ��PID
void LocPIDCalc(MPID* ptr)
{
	int iError,dError;
	iError=ptr->SetPoint-ptr->Feedback;//ƫ��
	ptr->SumError+=iError;			//����
	dError=iError-ptr->LastError;	//����
	ptr->LastError=iError;
	ptr->Uk=ptr->Proportion*iError+ptr->Intergral*ptr->SumError+ptr->Derivative*dError;
}
*/
