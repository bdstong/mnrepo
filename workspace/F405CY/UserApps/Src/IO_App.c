#include "parameters.h"
#include "taskApp.h"

uint16_t AllSampleBuf[ADC_NUMBER+FREQ_NUMBER+128];//8个模拟量+8个频率+128个备用(存放modbus的发送数据)

//   (1000ms/calc_period)+1 calc_period--ms
#define PL_CALC_N (1000/PL_CALC_PERIOD+1)
uint16_t Counter[FREQ_NUMBER];
uint16_t Counter_Buf[FREQ_NUMBER][PL_CALC_N];
uint16_t PinLv_Ptr[FREQ_NUMBER];

__IO uint16_t KinStatus;
__IO uint16_t KoutStatus;
__IO uint16_t SWStatus;

uint16_t outDlaeyPorcCnt=0;;

__IO uint16_t Out_Cmd[TOTAL_OUT],Out_Cmd_Prv[TOTAL_OUT],Out_Cmd_Out[TOTAL_OUT],Out_Cmd_Cnt[TOTAL_OUT];
__IO uint16_t Out_On_Delay[TOTAL_OUT];//,Out_Off_Delay[TOTAL_OUT];
extern DAC_HandleTypeDef hdac;
extern uint16_t Parameters[];

uint16_t read_sw_sta()
{
	uint16_t temp=0;
	temp|=SW4_READ;
	temp|=(SW3_READ<<1);
	temp|=(SW2_READ<<2);
	temp|=(SW1_READ<<3);
	temp++;
	return temp;
}

uint16_t read_ki_sta()
{
	uint16_t temp=0;
	temp|=KIN1_READ;
	temp|=(KIN2_READ<<1);
	temp|=(KIN3_READ<<2);
	temp|=(KIN4_READ<<3);
	temp|=(KIN5_READ<<4);
	temp|=(KIN6_READ<<5);
//	temp|=(KIN7_READ<<6);
//	temp|=(KIN8_READ<<7);
	return temp;
}

/*
uint16_t read_ko_sta1()
{
	uint16_t temp=0;
	temp|=KOUT1_READ;
	temp|=(KOUT2_READ<<1);
	temp|=(KOUT3_READ<<2);
	temp|=(KOUT4_READ<<3);
	temp|=(KOUT5_READ<<4);
	temp|=(KOUT6_READ<<5);
	temp|=(KOUT7_READ<<6);
	temp|=(KOUT8_READ<<7);
	return temp;
}
*/
uint16_t GetKoutCmd()
{
	uint16_t temp=0;
	for(int i=0;i<TOTAL_OUT;i++)
	{	temp<<=1;
		if(Out_Cmd[TOTAL_OUT-1-i]==K_CMD_OFF)temp|=1;
	}
	return temp;
}

void read_KinOutSw()
{
		KinStatus=read_ki_sta();
		KoutStatus=GetKoutCmd();
		SWStatus=read_sw_sta();
}
void calc_pinlv()
{
int i;
	for(i=0;i<FREQ_NUMBER;i++)
	{
		Counter_Buf[i][PinLv_Ptr[i]]=Counter[i];
		AllSampleBuf[FREQ_INDEX+i]=(Counter_Buf[i][PinLv_Ptr[i]]-Counter_Buf[i][(PinLv_Ptr[i]+1)%PL_CALC_N]);
		PinLv_Ptr[i]=(PinLv_Ptr[i]+1)%PL_CALC_N;
	}
}


int16_t get_y(int16_t y_start,int16_t y_end,int16_t x_start,int16_t x_end,int16_t x) //y=a*x+b 2011-5-27
{
int16_t y;
int32_t a,b;
	if(x_start==x_end) return y_start;
	if(y_start==y_end) return y_start;
	if(x_start<x_end)
	{
		if(x<=x_start) return y_start;
		if(x>=x_end) return y_end;
	}
	else
	{
		if(x>=x_start) return y_start;
		if(x<=x_end) return y_end;
	} 
	a=(int32_t)(y_start-y_end)*10000/(x_start-x_end);
	b=y_start-(int32_t)x_start*a/10000;
	y=(int32_t)x*a/10000+b;
	return y;
}
/*
void kout(int num,int cmd)
{
	switch(num)
	{
		case 1:
			if(cmd==KO_ON) KOUT1_ON;
			else KOUT1_OFF;
			break;
		case 2:
			if(cmd==KO_ON) KOUT2_ON;
			else KOUT2_OFF;
			break;
		case 3:
			if(cmd==KO_ON) KOUT3_ON;
			else KOUT3_OFF;
			break;
		case 4:
			if(cmd==KO_ON) KOUT4_ON;
			else KOUT4_OFF;
			break;
		case 5:
			if(cmd==KO_ON) KOUT5_ON;
			else KOUT5_OFF;
			break;
		case 6:
			if(cmd==KO_ON) KOUT6_ON;
			else KOUT6_OFF;
			break;
		case 7:
			if(cmd==KO_ON) KOUT7_ON;
			else KOUT7_OFF;
			break;
		case 8:
			if(cmd==KO_ON) KOUT8_ON;
			else KOUT8_OFF;
			break;
	}
}
*/
void SetVout(int dacl,int dach)
{
	HAL_DAC_SetValue(&hdac,DAC_CHANNEL_1,DAC_ALIGN_12B_R,(dacl+(dach<<8))&0xfff);
}


void Out_PWM_Proc(void)
{
uint16_t i;

	for(i=0;i<TOTAL_OUT;i++)
	{
		if(Parameters[OUT_BEGIN_OFFSET+i]==0){
				continue;
		}
		if(Out_Cmd[i]==K_CMD_OFF)
		{//OFF
			Out_Cmd_Prv[i]=K_CMD_OFF;
			Out_Cmd_Out[i]=K_CMD_OFF;
			Out_Cmd_Cnt[i]=0;
		}
		else
		{//ON
			if(Out_Cmd_Prv[i]==K_CMD_OFF)
			{//start ON
				Out_Cmd_Prv[i]=K_CMD_ON;
				Out_Cmd_Out[i]=K_CMD_ON;
				Out_Cmd_Cnt[i]++;
			}
			else
			{//already ON
				Out_Cmd_Cnt[i]++;
				if(Parameters[OUT_ON_OFFSET+i]>0 && Parameters[OUT_OFF_OFFSET+i]>0){
					if(Out_Cmd_Cnt[i]>=(Parameters[OUT_BEGIN_OFFSET+i]+Parameters[OUT_ON_OFFSET+i]))
					{
						Out_Cmd_Out[i]=K_CMD_OFF;
					}
					if(Out_Cmd_Cnt[i]>=((Parameters[OUT_BEGIN_OFFSET+i]+Parameters[OUT_ON_OFFSET+i])+Parameters[OUT_OFF_OFFSET+i]))
					{
						Out_Cmd_Out[i]=K_CMD_ON;
						Out_Cmd_Cnt[i]=Parameters[OUT_BEGIN_OFFSET+i];	
					}
				}
				else if(Parameters[OUT_ON_OFFSET+i]==0){
					if(Out_Cmd_Cnt[i]>=Parameters[OUT_BEGIN_OFFSET+i]){
						Out_Cmd_Out[i]=K_CMD_OFF;
						Out_Cmd[i]=K_CMD_OFF;
					}
				}
				else{//Parameters[OUT_OFF_OFFSET+i]==0
					if(Out_Cmd_Cnt[i]>=Parameters[OUT_BEGIN_OFFSET+i]){
						Out_Cmd_Out[i]=K_CMD_ON;
						Out_Cmd_Cnt[i]=Parameters[OUT_BEGIN_OFFSET+i];
					}
				}					
			}
		}
	}
	if(Out_Cmd_Out[0]==K_CMD_ON) KOUT1_ON;else KOUT1_OFF;
	if(Out_Cmd_Out[1]==K_CMD_ON) KOUT2_ON;else KOUT2_OFF;
	if(Out_Cmd_Out[2]==K_CMD_ON) KOUT3_ON;else KOUT3_OFF;
	if(Out_Cmd_Out[3]==K_CMD_ON) KOUT4_ON;else KOUT4_OFF;
	if(Out_Cmd_Out[4]==K_CMD_ON) KOUT5_ON;else KOUT5_OFF;
	if(Out_Cmd_Out[5]==K_CMD_ON) KOUT6_ON;else KOUT6_OFF;
	if(Out_Cmd_Out[6]==K_CMD_ON) KOUT7_ON;else KOUT7_OFF;
	if(Out_Cmd_Out[7]==K_CMD_ON) KOUT8_ON;else KOUT8_OFF;

	if(Out_Cmd_Out[8]==K_CMD_ON) KOUT9_ON;else KOUT9_OFF;
	if(Out_Cmd_Out[9]==K_CMD_ON) KOUT10_ON;else KOUT10_OFF;
}
void InitOut(void)
{
uint16_t i;
	for(i=0;i<TOTAL_OUT;i++)
	{
		Out_Cmd[i]=K_CMD_OFF;
		Out_Cmd_Prv[i]=K_CMD_OFF;
		Out_Cmd_Cnt[i]=0;
		Out_Cmd_Out[i]=K_CMD_OFF;
		Out_On_Delay[i]=0;
//		Out_Off_Delay[i]=0;
	}
	if(XLF_POWER_UP==1234) Out_Cmd[XLF_CMD_CHN]=K_CMD_ON;
	else Out_Cmd[XLF_CMD_CHN]=K_CMD_OFF;

	if(M12_POWER_UP==1234)
	{
		Out_Cmd[M1_RESET_CMD_CHN]=K_CMD_ON;
		Out_On_Delay[M1_RESET_CMD_CHN]=M1_RESET_DEL;

		Out_Cmd[M2_RESET_CMD_CHN]=K_CMD_ON;
		Out_On_Delay[M2_RESET_CMD_CHN]=M2_RESET_DEL;
	}
}

void Out_Delay_Proc(void)
{
uint16_t i;
	for(i=0;i<TOTAL_OUT;i++)
	{
		if(Out_Cmd[i]==K_CMD_ON){
			if(Out_On_Delay[i]==0) continue;
			else{
				if(Out_On_Delay[i]){
					Out_On_Delay[i]--;
					if(Out_On_Delay[i]==0){
						Out_Cmd[i]=K_CMD_OFF;
						//是否需要自动延时复位
						switch(i){
							case M1_NORMAL_CMD_CHN:
							case M1_RESERVE_CMD_CHN:
								Out_On_Delay[M1_RESET_CMD_CHN]=M1_RESET_DEL;
								Out_Cmd[M1_RESET_CMD_CHN]=K_CMD_ON;
								break;
							case M2_NORMAL_CMD_CHN:
							case M2_RESERVE_CMD_CHN:
								Out_On_Delay[M2_RESET_CMD_CHN]=M2_RESET_DEL;
								Out_Cmd[M2_RESET_CMD_CHN]=K_CMD_ON;
								break;
							default:
								break;
						}
					}
				}
			}
		}
	}
}

void ProcDJCmd(uint8_t *p){
	uint8_t buf[8];
	memcpy(buf,p,8);
	uint16_t offCmd,onCmd,delay;

	//first word which is off 
	offCmd=buf[0];
	offCmd+=(buf[1]<<8);
	//second word which is on
	onCmd=buf[2];
	onCmd+=(buf[3]<<8);
	//third word is delay
	delay=buf[4];
	delay+=(buf[5]<<8);
	
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

void ProcKout(void)
{
	Out_PWM_Proc();//3.25us max
	if(outDlaeyPorcCnt<1){
		outDlaeyPorcCnt++;//0.12us
	}
	else{
		outDlaeyPorcCnt=0;
		Out_Delay_Proc();
	}
}
