#include "taskApp.h"

//int *M1_CURRENT, *M1_TARGET;
//int *M2_CURRENT, *M2_TARGET;
//int m1_target,m2_target;
//int m1_range,m2_range;
//int m1_period,m2_period;
//int m1_period_cnt,m2_period_cnt;

//volatile int m1_mode,m2_mode;
//int m1_stop_ahead,m2_stop_ahead;
//int m1_direction,m2_direction;
//int m1_pwm_cnt,m2_pwm_cnt;
//int m2_t_diff,m1_t_diff;
//unsigned int m1_delay_cnt,m2_delay_cnt;
//int m1_kongzhi_flag,m2_kongzhi_flag;
//#define FUWEI_DELAY 1250	//1s

//#define NEW_START							1
//#define NORMAL_RUN_DELAY			2
//#define NORMAL_RUN_CONTINUE		3
//#define REVERSE_RUN_DELAY			4
//#define REVERSE_RUN_CONTINUE	5
//#define STOP_DELAY						6

extern uint16_t Parameters[];
extern uint16_t AllSampleBuf[];
extern uint16_t Out_Cmd[];

MPID bengPid,madaPid;
MOTOR Beng,Mada;
uint8_t bengTest;
uint8_t madaTest;
int MotorStopFlag;

void motorInit(void)
{
	Beng.pCurrent=(int16_t*)&AllSampleBuf[0];
	Beng.pTarget=(int16_t*)&AllSampleBuf[0];
	Beng.period=10;
	Beng.period_cnt=0;
	Beng.range=10;
	Beng.normalTime=10;
	Beng.reverseTime=10;
	Beng.norCmdChn=M1_NORMAL_CMD_CHN;
	Beng.revCmdChn=M1_RESERVE_CMD_CHN;
	Beng.rstCmdChn=M1_RESET_CMD_CHN;
	Beng.rstDelayTime=M1_RESET_DEL;
	Beng.cmd=M_CMD_FREE;

	Mada.pCurrent=(int16_t*)&AllSampleBuf[0];
	Mada.pTarget=(int16_t*)&AllSampleBuf[0];
	Mada.period=10;
	Mada.period_cnt=0;
	Mada.range=10;
	Mada.normalTime=10;
	Mada.reverseTime=10;
	Mada.norCmdChn=M2_NORMAL_CMD_CHN;
	Mada.revCmdChn=M2_RESERVE_CMD_CHN;
	Mada.rstCmdChn=M2_RESET_CMD_CHN;
	Mada.rstDelayTime=M2_RESET_DEL;
	Mada.cmd=M_CMD_FREE;

	bengTest=0;
	madaTest=0;
}

void bengRun(MOTOR *motor,MPID *pPid)
{
	uint16_t kzCmdBuf[3];
	int inc;
	
	switch(motor->cmd)
	{
		case M_CMD_FREE:
			break;
		case M_CMD_STOP:
			if(Out_Cmd[motor->norCmdChn]==K_CMD_ON||Out_Cmd[motor->revCmdChn]==K_CMD_ON)
			{
				kzCmdBuf[0]=(1 << motor->norCmdChn) | (1 << motor->revCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->rstCmdChn);	//turn on
				kzCmdBuf[2]=motor->rstDelayTime;				//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);			
			}
			motor->cmd=M_CMD_FREE;
			motor->period_cnt=0;
			bengTest=4;
			break;
		case M_CMD_RUN:
			motor->period_cnt++;
			if(motor->period_cnt<motor->period)	break;
			motor->period_cnt=0;
			
//			m_diff=*motor->pTarget-*motor->pCurrent;
//			if(abs(m_diff)<=motor->range)
//			{
//				bengTest=11;
//				break; //误差范围内
//			}
				
//			if(BENG_ZS_KZ_FLAG==1234)//泵转速小于一定值，减小泵排量
//			{
//				if(BENG_ZHUANSU<=BENG_ZHUANSU_SET)
//				{
//					kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->norCmdChn);//turn off
//					kzCmdBuf[1]=(1 << motor->revCmdChn);	//turn on
//					kzCmdBuf[2]=motor->reverseTime ;			//delay
//					ProcDJCmd((uint8_t *)kzCmdBuf);
//					bengTest=5;
//					break;
//				}
//			}

			if(*motor->pCurrent<*motor->pTarget-motor->range)//<target
			{
//				if((motor->normalTime+motor->rstDelayTime+1)>=(motor->period*motor->samplPeriod)) motor->normalTime =motor->period*motor->samplPeriod-motor->rstDelayTime-1;
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->revCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->norCmdChn);	//turn on
				kzCmdBuf[2]=motor->normalTime;				//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				bengTest=1;
				break;
			}
			if(*motor->pCurrent>*motor->pTarget+motor->range)//>target
			{
//				if((motor->reverseTime+motor->rstDelayTime+1)>=(motor->period*motor->samplPeriod)) motor->reverseTime =motor->period*motor->samplPeriod-motor->rstDelayTime-1;
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->norCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->revCmdChn);	//turn on
				kzCmdBuf[2]=motor->reverseTime ;			//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				bengTest=2;
				break;
			}
			bengTest=3;
			break;
		case M_CMD_PID:
			pPid->PrdCnt++;
			if(pPid->PrdCnt<pPid->Period)	break;
			pPid->PrdCnt=0;
			
			inc=getPidInc(pPid);
			
//			if(BENG_ZS_KZ_FLAG==1234)//泵转速小于一定值，减小泵排量
//			{
//				if(BENG_ZHUANSU<=BENG_ZHUANSU_SET)
//				{
//					kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->norCmdChn);//turn off
//					kzCmdBuf[1]=(1 << motor->revCmdChn);	//turn on
//					kzCmdBuf[2]=motor->reverseTime ;			//delay
//					ProcDJCmd((uint8_t *)kzCmdBuf);
//					clearPid(pPid);
//					bengTest=15;
//					break;
//				}
//			}

//			if(pPid->Range)
//			{
//				if(abs(*pPid->pSetPoint-*pPid->pFeedback)<pPid->Range)
//				{
//					kzCmdBuf[0]=(1 << motor->norCmdChn) | (1 << motor->revCmdChn);//turn off
//					kzCmdBuf[1]=(1 << motor->rstCmdChn);	//turn on
//					kzCmdBuf[2]=motor->rstDelayTime;				//delay
//					ProcDJCmd((uint8_t *)kzCmdBuf);			
//					clearPid(pPid);
//					bengTest=14;
//					break;
//				}
//			}

//			if(M1_DY_CHN<8 && inc>0) //泵排量限值使能
//			{
//				if(AllSampleBuf[M1_DY_CHN]>V8_MAX_AD_VALUE)
//				{
//					kzCmdBuf[0]=(1 << motor->norCmdChn) | (1 << motor->revCmdChn);//turn off
//					kzCmdBuf[1]=(1 << motor->rstCmdChn);	//turn on
//					kzCmdBuf[2]=motor->rstDelayTime;				//delay
//					ProcDJCmd((uint8_t *)kzCmdBuf);			
//					inc=0;
//				}
//			}
			
			if(inc>0)
			{
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->revCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->norCmdChn);	//turn on
				kzCmdBuf[2]=pPid->AbsOfIncrease;				//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				bengTest=11;
				break;
			}
			if(inc<0)
			{
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->norCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->revCmdChn);	//turn on
				kzCmdBuf[2]=pPid->AbsOfIncrease;			//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				bengTest=12;
				break;
			}
			bengTest=13;
			break;
		default:
			break;
	}
}

void madaRun(MOTOR *motor,MPID *pPid)
{
	uint16_t kzCmdBuf[3];
	int inc;
	switch(motor->cmd)
	{
		case M_CMD_FREE:
			break;
		case M_CMD_STOP:
			if(Out_Cmd[motor->norCmdChn]==K_CMD_ON||Out_Cmd[motor->revCmdChn]==K_CMD_ON)
			{
				kzCmdBuf[0]=(1 << motor->norCmdChn) | (1 << motor->revCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->rstCmdChn);	//turn on
				kzCmdBuf[2]=motor->rstDelayTime;				//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);			
			}
			motor->cmd=M_CMD_FREE;
			motor->period_cnt=0;
			madaTest=4;
			break;
		case M_CMD_RUN:
			motor->period_cnt++;
			if(motor->period_cnt<motor->period)	break;
			motor->period_cnt=0;

			if(*motor->pCurrent<*motor->pTarget-motor->range)//>target
			{
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->revCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->norCmdChn);	//turn on
				kzCmdBuf[2]=motor->normalTime;				//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				madaTest=1;
				break;
			}
			if(*motor->pCurrent>*motor->pTarget+motor->range)//<target
			{
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->norCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->revCmdChn);	//turn on
				kzCmdBuf[2]=motor->reverseTime ;			//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				madaTest=2;
				break;
			}
			madaTest=3;
			break;
		case M_CMD_PID:
			pPid->PrdCnt++;
			if(pPid->PrdCnt<pPid->Period)	break;
			pPid->PrdCnt=0;
			
			inc=getPidInc(pPid);

//			if(pPid->Range)
//			{
//				if(abs(*pPid->pSetPoint-*pPid->pFeedback)<pPid->Range)
//				{
//					kzCmdBuf[0]=(1 << motor->norCmdChn) | (1 << motor->revCmdChn);//turn off
//					kzCmdBuf[1]=(1 << motor->rstCmdChn);	//turn on
//					kzCmdBuf[2]=motor->rstDelayTime;				//delay
//					ProcDJCmd((uint8_t *)kzCmdBuf);			
//					clearPid(pPid);
//					madaTest=14;
//					break;
//				}
//			}

			if(inc>0)
			{
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->revCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->norCmdChn);	//turn on
				kzCmdBuf[2]=pPid->AbsOfIncrease;				//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				madaTest=11;
				break;
			}
			if(inc<0)
			{
				kzCmdBuf[0]=(1 << motor->rstCmdChn) | (1 << motor->norCmdChn);//turn off
				kzCmdBuf[1]=(1 << motor->revCmdChn);	//turn on
				kzCmdBuf[2]=pPid->AbsOfIncrease;			//delay
				ProcDJCmd((uint8_t *)kzCmdBuf);
				madaTest=12;
				break;
			}
			madaTest=13;
			break;
		default:
			break;
	}
}

///////////////////////////////////////////////////
/*
int get_m1_stop_value()
{
	int val=get_y(M1_STOP_MIN,M1_STOP_MAX,YALI1_MIN_AD_VALUE,YALI1_MAX_AD_VALUE,XT_YALI);//压力最小和最大相差9倍
	return val;
}

int get_m2_stop_value()
{
	int val=get_y(M2_STOP_MIN,M2_STOP_MAX,YALI1_MIN_AD_VALUE,YALI1_MAX_AD_VALUE,XT_YALI);
	return val;
}

void motor_init(void)
{

	M1_CURRENT=(int*)&BENG_PL_DY;
	M1_TARGET=(int*)&BENG_PL_DY;
	m1_direction=NEW_START;
	m1_mode=0;
	m1_delay_cnt=0;

	M2_CURRENT=(int*)&M2_DY;
	M2_TARGET=(int*)&M2_DY;
	m2_direction=NEW_START;
	m2_mode=0;
	m2_delay_cnt=0;

	m1_kongzhi_flag=0;
	m2_kongzhi_flag=0;

	m1_period=m2_period=10;
	m1_period_cnt=m2_period_cnt=0;

}
void motor1_run(void)
{
	if(m1_kongzhi_flag) return;

	m1_stop_ahead=get_m1_stop_value();

	switch(m1_direction)
	{
	case NEW_START:
		if(m1_delay_cnt<(FUWEI_DELAY+1)) m1_delay_cnt++;
		if(m1_delay_cnt==FUWEI_DELAY)
		{
			M1_ALL_OFF;
		}

		if(MotorStopFlag==1)break;;

		m1_t_diff=*M1_TARGET-*M1_CURRENT;
		if(abs(m1_t_diff)<=M1_T_ERROR) break; //误差范围内
		if(m1_t_diff>M1_T_ERROR)
		{//正向
			if(BENG_PL_DY>=M1_MAX_AD_VALUE) break;//如果到了最大位置，不动
			M1_NORMAL;
			m1_direction=NORMAL_RUN_CONTINUE;
			break;
		}
		else
		{//反向
			if(BENG_PL_DY<=M1_MIN_AD_VALUE) break;	//如果到了最小位置
			M1_REVERSE;
			m1_direction=REVERSE_RUN_CONTINUE;
			break;
		}

	case NORMAL_RUN_CONTINUE:
		if(MotorStopFlag==1)
		{//如果压力不够，停止
			M1_STOP;
			m1_delay_cnt=0;
			m1_direction=NEW_START;
			break;
		}
		if(BENG_PL_DY>=(M1_MAX_AD_VALUE-M1_T_ERROR))
		{//快到最大行程,必须停止
			M1_STOP;
			m1_delay_cnt=0;
			m1_direction=NEW_START;
			break;
		}
		m1_t_diff=*M1_TARGET-*M1_CURRENT;
		if(m1_t_diff>m1_stop_ahead)
		{//继续正向
			M1_NORMAL;
			break;
		}
		else
		{//到达误差范围，必须停止
			M1_STOP;
			m1_delay_cnt=0;
			m1_direction=NEW_START;
			break;
		}
	case REVERSE_RUN_CONTINUE:
		if(MotorStopFlag==1)
		{
			M1_STOP;
			m1_delay_cnt=0;
			m1_direction=NEW_START;
			break;
		}
		if(BENG_PL_DY<=(M1_MIN_AD_VALUE+M1_T_ERROR))
		{//快到最小行程,必须停止
			M1_STOP;
			m1_delay_cnt=0;
			m1_direction=NEW_START;
			break;
		}
		m1_t_diff=*M1_CURRENT-*M1_TARGET;
		if(m1_t_diff>m1_stop_ahead)
		{//反向继续
			M1_REVERSE;
			break;
		}
		else
		{//到达误差范围，必须停止
			M1_STOP;
			m1_delay_cnt=0;
			m1_direction=NEW_START;
			break;
		}
	default:
		M1_STOP;
		m1_delay_cnt=0;
		m1_direction=NEW_START;
		break;
	}//end of switch
}

void motor2_run(void)
{
	if(m2_kongzhi_flag) return;

	m2_stop_ahead=get_m2_stop_value();

	switch(m2_direction)
	{
	case NEW_START:
		if(m2_delay_cnt<(FUWEI_DELAY+1)) m2_delay_cnt++;
		if(m2_delay_cnt==FUWEI_DELAY)
		{
			M2_ALL_OFF;
		}

		if(MotorStopFlag==1) break;

		m2_t_diff=*M2_TARGET-*M2_CURRENT;
		if(abs(m2_t_diff)<=M2_T_ERROR) break;//误差范围内，不动
		if(m2_t_diff>M2_T_ERROR)
		{//正向
			if(M2_DY>=M2_MAX_AD_VALUE) break;//如果到了最大位置，不动
			M2_NORMAL;
			m2_direction=NORMAL_RUN_CONTINUE;
			break;
		}
		else
		{//反向
			if(M2_DY<=M2_MIN_AD_VALUE) break;	//如果到了最小位置，不动
			M2_REVERSE;
			m2_direction=REVERSE_RUN_CONTINUE;
		break;
	}

	case NORMAL_RUN_CONTINUE:
		if(MotorStopFlag==1)
		{//如果压力不够，停止
			M2_STOP;
			m2_delay_cnt=0;
			m2_direction=NEW_START;
			break;
		}
		if(M2_DY>=(M2_MAX_AD_VALUE-M2_T_ERROR))
		{
			M2_STOP;
			m2_delay_cnt=0;
			m2_direction=NEW_START;
			break;
		}
		m2_t_diff=*M2_TARGET-*M2_CURRENT;
		if(m2_t_diff>m2_stop_ahead)
		{//继续正
			M2_NORMAL;
			break;
		}
		else
		{//到达误差范围，必须停止
			M2_STOP;
			m2_delay_cnt=0;
			m2_direction=NEW_START;
			break;
		}
	case REVERSE_RUN_CONTINUE://
		if(MotorStopFlag==1)
		{//如果压力不够，停止
			M2_STOP;
			m2_delay_cnt=0;
			m2_direction=NEW_START;
			break;
		}
		if(M2_DY<=(M2_MIN_AD_VALUE+M2_T_ERROR))
		{
			M2_STOP;
			m2_delay_cnt=0;
			m2_direction=NEW_START;
			break;
		}
		m2_t_diff=*M2_CURRENT-*M2_TARGET;
		if(m2_t_diff>m2_stop_ahead)
		{//反向继续
			M2_REVERSE;
			break;
		}
		else
		{//到达误差范围，必须停止
			M2_STOP;
			m2_delay_cnt=0;
			m2_direction=NEW_START;
			break;
		}
	default:
			M2_STOP;
			m2_delay_cnt=0;
			m2_direction=NEW_START;
		break;
	}//end of switch
}
*/
/*
void motor1_kongzhi(int *Caiyang,int *Mubiao,int *Fanwei,int *Tiqian,int *m_mode)
{
//#ifdef LED_TEST
//ToggleLED(LED3);
//#endif
int Piancha;
int direction;
int m_mode_bak;
int fw_delay;
	m1_kongzhi_flag=1;

	m_mode_bak=*m_mode;

	M1_CURRENT=&BENG_PL_DY;
	M1_TARGET=&BENG_PL_DY;
	M1_STOP;
	fw_delay=0;

	direction=NEW_START;
	while(1)
	{
		OSTimeDly(1);
		if(*m_mode!=m_mode_bak)
		{
			M2_CURRENT=&M2_DY;
			M2_TARGET=&M2_DY;
			M1_STOP;
			m1_delay_cnt=0;
			break;
		}

		switch(direction)
		{
		case NEW_START:
			if(fw_delay<(FUWEI_DELAY+1)) fw_delay++;
			if(fw_delay==FUWEI_DELAY)
			{
				M1_ALL_OFF;
			}

			if(NoRunning_Flag==1)break;;

			Piancha=*Mubiao-*Caiyang;
			if(abs(Piancha)<=*Fanwei) break; //误差范围内

			if(Piancha>*Fanwei)
			{//正向
				if(M1_Percent>=99) break;//如果到了最大位置，不动
				M1_NORMAL;
				direction=NORMAL_RUN_CONTINUE;
				break;
			}
			else
			{//反向
				if(M1_Percent<=1) break;	//如果到了钚∥恢?
				M1_REVERSE;
				direction=REVERSE_RUN_CONTINUE;
				break;
			}

		case NORMAL_RUN_CONTINUE:
			if(NoRunning_Flag==1)
			{
				M1_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
			if(M1_Percent>=99)
			{
				M1_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}

			Piancha=*Mubiao-*Caiyang;
			if(Piancha>*Tiqian)
			{//继续正向
				break;
			}
			else
			{//到达误差范围，必须停止
				M1_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
		case REVERSE_RUN_CONTINUE:
			if(NoRunning_Flag==1)
			{
				M1_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
			if(M1_Percent<=1)
			{
				M1_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}

			Piancha=*Caiyang-*Mubiao;
			if(Piancha>*Tiqian)
			{//反向继续
				break;
			}
			else
			{//到达误差范围，必须停止
				M1_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
		default:
			M1_STOP;
			fw_delay=0;
			direction=NEW_START;
			break;
		}//end of switch
	}
	m1_kongzhi_flag=0;
}

void motor2_kongzhi(int *Caiyang,int *Mubiao,int *Fanwei,int *Tiqian,int *m_mode)
{
//#ifdef LED_TEST
//ToggleLED(LED3);
//#endif
int Piancha;
int direction;
int m_mode_bak;
int fw_delay;
	m2_kongzhi_flag=1;

	m_mode_bak=*m_mode;

	M2_CURRENT=&M2_DY;
	M2_TARGET=&M2_DY;
	M2_STOP;
	fw_delay=0;

	direction=NEW_START;
	while(1)
	{
		OSTimeDly(1);
		if(*m_mode!=m_mode_bak)
		{
			M2_CURRENT=&M2_DY;
			M2_TARGET=&M2_DY;
			M2_STOP;
			m2_delay_cnt=0;
			break;
		}

		switch(direction)
		{
		case NEW_START:
			if(fw_delay<(FUWEI_DELAY+1)) fw_delay++;
			if(fw_delay==FUWEI_DELAY)
			{
				M2_ALL_OFF;
			}

			if(NoRunning_Flag==1)break;;

			Piancha=*Mubiao-*Caiyang;
			if(abs(Piancha)<=*Fanwei) break; //误差范围内

			if(Piancha>*Fanwei)
			{//正向
				if(M2_Percent>=99) break;//如果到了最大位置，不动
				M2_NORMAL;
				direction=NORMAL_RUN_CONTINUE;
				break;
			}
			else
			{//反向
				if(M2_Percent<=1) break;	//如果到了最小位置
				M2_REVERSE;
				direction=REVERSE_RUN_CONTINUE;
				break;
			}

		case NORMAL_RUN_CONTINUE:
			if(NoRunning_Flag==1)
			{
				M2_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
			if(M2_Percent>=99)
			{
				M2_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}

			Piancha=*Mubiao-*Caiyang;
			if(Piancha>*Tiqian)
			{//继续正向
				break;
			}
			else
			{//到达误差范围，必须停止
				M2_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
		case REVERSE_RUN_CONTINUE:
			if(NoRunning_Flag==1)
			{
				M2_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
			if(M2_Percent<=1)
			{
				M2_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}

			Piancha=*Caiyang-*Mubiao;
			if(Piancha>*Tiqian)
			{//反向继续
				break;
			}
			else
			{//到达误差范围，必须停止
				M2_STOP;
				fw_delay=0;
				direction=NEW_START;
				break;
			}
		default:
			M2_STOP;
			fw_delay=0;
			direction=NEW_START;
			break;
		}//end of switch
	}
	m2_kongzhi_flag=0;
}
*/

/*
#define TING_ZHI	0
#define ZHENG_XIANG 1
#define FAN_XIANG	2
#define FU_WEI 		3
void M1_Run_PID(void)
{
int increment;

//	IncPIDInit(&M1RunPid);

	M1RunPid.PrdCnt++;
	if(M1RunPid.PrdCnt>=M1RunPid.Period)
	{
		M1RunPid.PrdCnt=0;
		M1RunPid.SetPoint=m1_target;
		M1RunPid.Feedback=BENG_PL_DY;
		increment=GetPIDInc(&M1RunPid);

		if(increment==0)
		{
			if(M1_REVERSE_BIT3==0 || M1_NORMAL_BIT3==0 || M1_FUWEI_BIT==0)
			{
				M1_NORMAL_BIT1_OFF;
				M1_NORMAL_BIT2_OFF;
				M1_NORMAL_BIT3_OFF;
				M1_REVERSE_BIT1_OFF;
				M1_REVERSE_BIT2_OFF;
				M1_REVERSE_BIT3_OFF;
				m1_diection=FU_WEI;
				m1_tm_count=0;
				M1_FUWEI_BIT_ON;
			}
			return;
		}
		if(increment>0)
		{
			increment=abs(increment);
			//正转
			if(M1_REVERSE_BIT3==0 || M1_FUWEI_BIT==0)
			{
				//如果当前是反转
				M1_REVERSE_BIT1_OFF;
				M1_REVERSE_BIT2_OFF;
				M1_REVERSE_BIT3_OFF;
				m1_fuweiflag=1;
				m1_tm_count=0;
				m1_tm_max=increment;
				m1_diection=ZHENG_XIANG;
				M1_FUWEI_BIT_ON;
				return;
			}
			if(M1_NORMAL_BIT3==1)
			{
				//上次的已完成
				M1_FUWEI_BIT_OFF;
				m1_tm_max=increment;
				m1_tm_count=0;
				m1_diection=ZHENG_XIANG;
				m1_fuweiflag=0;
				M1_NORMAL_BIT1_ON;
				M1_NORMAL_BIT2_ON;
				M1_NORMAL_BIT3_ON;
				return;
			}
			else
			{
				m1_tm_max=increment;
				m1_tm_count=0;
				m1_diection=ZHENG_XIANG;
				m1_fuweiflag=0;
				return;
			}
		}
		else
		{
			increment=abs(increment);
			//反转
			if(M1_NORMAL_BIT3==0 || M1_FUWEI_BIT==0)
			{
				//如果当前是正转
				M1_NORMAL_BIT1_OFF;
				M1_NORMAL_BIT2_OFF;
				M1_NORMAL_BIT3_OFF;
				m1_fuweiflag=1;
				m1_tm_count=0;
				m1_tm_max=increment;
				m1_diection=FAN_XIANG;
				M1_FUWEI_BIT_ON;
				return;
			}
			if(M1_REVERSE_BIT3==1)
			{
				//上次的已完成
				M1_FUWEI_BIT_OFF;
				m1_tm_max=increment;
				m1_tm_count=0;
				m1_diection=FAN_XIANG;
				m1_fuweiflag=0;
				M1_REVERSE_BIT1_ON;
				M1_REVERSE_BIT2_ON;
				M1_REVERSE_BIT3_ON;
				return;
			}
			else
			{
				m1_tm_max=increment;
				m1_tm_count=0;
				m1_diection=FAN_XIANG;
				m1_fuweiflag=0;
				return;
			}
		}
	}

switch(m1_diection)
{
	case TING_ZHI://stop
		break;
	case FU_WEI://复位
		m1_tm_count++;
		if(m1_tm_count>=M1_FUWEI_MAX_TIME)
		{
			M1_FUWEI_BIT_OFF;
			m1_diection=TING_ZHI;
		}
		break;
	case ZHENG_XIANG://正转
		if(m1_fuweiflag)
		{
			//先复位
			m1_tm_count++;
			if(m1_tm_count>=M1_FUWEI_MAX_TIME)
			{
				M1_FUWEI_BIT_OFF;
				m1_fuweiflag=0;
				m1_tm_count=0;
				M1_NORMAL_BIT1_ON;
				M1_NORMAL_BIT2_ON;
				M1_NORMAL_BIT3_ON;
			}
			break;
		}
		//if(m1_tm_count<(m1_tm_max+M1_ALL_ON_TIME+M1_ONE_ON_TIME+M1_FUWEI_MAX_TIME)) m1_tm_count++;
		//else break;
		if(m1_tm_count<(m1_tm_max+M1_ALL_ON_TIME)) m1_tm_count++;
		else
		{
			if(M1_NORMAL_BIT3==0)
			{
				M1_NORMAL_BIT1_OFF;
				M1_NORMAL_BIT2_OFF;
				M1_NORMAL_BIT3_OFF;
				m1_tm_count=0;
				M1_FUWEI_BIT_ON;
				m1_diection=FU_WEI;
				break;
			}
			if(M1_FUWEI_BIT==0)
			{
				m1_tm_count=1;
				m1_diection=FU_WEI;
				break;
			}
			break;
		}
		if(m1_tm_count==M1_ALL_ON_TIME)
		{
			M1_NORMAL_BIT1_OFF;
			M1_NORMAL_BIT2_OFF;
			break;
		}
//		if(m1_tm_count==(M1_ALL_ON_TIME+M1_ONE_ON_TIME))
		{
			M1_NORMAL_BIT3_OFF;
			M1_FUWEI_BIT_ON;
			break;
		}
  //		if(m1_tm_count==(M1_ALL_ON_TIME+M1_ONE_ON_TIME+M1_FUWEI_MAX_TIME))
		{
			M1_FUWEI_BIT_OFF;
			m1_diection=TING_ZHI;
			break;
		}
		break;

	case FAN_XIANG://反转
		if(m1_fuweiflag)
		{
			//先复位
			m1_tm_count++;
			if(m1_tm_count>=M1_FUWEI_MAX_TIME)
			{
				M1_FUWEI_BIT_OFF;
				m1_fuweiflag=0;
				m1_tm_count=0;
				M1_NORMAL_BIT1_ON;
				M1_NORMAL_BIT2_ON;
				M1_NORMAL_BIT3_ON;
			}
			break;
		}
//		if(m1_tm_count<(m1_tm_max+M1_ALL_ON_TIME+M1_ONE_ON_TIME+M1_FUWEI_MAX_TIME)) m1_tm_count++;
		else break;
		if(m1_tm_count==M1_ALL_ON_TIME)
		{
			M1_NORMAL_BIT1_OFF;
			M1_NORMAL_BIT2_OFF;
			break;
		}
//		if(m1_tm_count==(M1_ALL_ON_TIME+M1_ONE_ON_TIME))
		{
			M1_NORMAL_BIT3_OFF;
			M1_FUWEI_BIT_ON;
			break;
		}
//		if(m1_tm_count==(M1_ALL_ON_TIME+M1_ONE_ON_TIME+M1_FUWEI_MAX_TIME))
		{
			m1_diection=TING_ZHI;
			M1_FUWEI_BIT_OFF;
			break;
		}
		break;
	default:
		break;
}//end of switch
}

*/
