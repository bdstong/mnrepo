#include "parameters.h"
#include "taskApp.h"

uint16_t Parameters[PARAMETER_NUM];
extern uint16_t AlarmFlag;

//24c16
#define E2PROM_SIZE 		0x0800	//2k bytes
#define E2PROM_BASE_ID	0xA0
 
#define E2PROM_WRITE		0x00
#define E2PROM_READ			0x01
 
#define E2PROM_BASE_WID E2PROM_BASE_ID + E2PROM_WRITE
#define E2PROM_BASE_RID E2PROM_BASE_ID + E2PROM_READ
 
#define EE_PAGE_SIZE			16
#define E2PROM_PAGE_MASK	0x000F

#define I2C_TIMEOUT 		0x20

#define PARAMETER_ADDR	0x100

const uint16_t Para_Default_Table[PARAMETER_NUM-2]=
{
//start time
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
//on time
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
1,
//off time
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
10,
//-----------------------------------------------		
0,								//V1_MIN_AD_VALUE
0,
0,
0,
0,
0,
0,
0,
AD_FULL,					//V1_MAX_AD_VALUE
AD_FULL,							
AD_FULL,							
AD_FULL,							
AD_FULL,							
AD_FULL,							
AD_FULL,							
AD_FULL,							
		
//-------[64] start----------------------------------------		

100,							// M1_RESET_DEL [64]
100,							// M2_RESET_DEL		[65]

0,						// DJ_FUWEI_FLAG		[66]
0,						// XLF_POWER_UP		[67]

1,						// XITONG_YALI_CHN	[68]
0,						// JSF_YALI_BAOHU		[69]

11,						// MADA_ZHUANSU_CHN		[70]
0,						// JSF_ZHUNASU_BAOHU [71]

0,						// MADA_RESET_YALI	[72]

0,					// USE_FFT_METHOD	[73]

AD_3V+AD_300MV,		// M1_STOP_MAX_AD
AD_50MV,					// M2_STOP_MIN_AD
AD_3V+AD_900MV,		// M2_STOP_MAX_AD

AD_20MV,					// M1_T_ERROR
AD_20MV,					// M2_T_ERROR	

/////////////////////////////////////////////////////




2300,						// ZHUANSU_TABLE8		
2500,						// ZHUANSU_TABLE9	
	
600,						// MUBIAO_ZS_TABLE0 ???????????????
700,						// MUBIAO_ZS_TABLE1
800,						// MUBIAO_ZS_TABLE2
900,						// MUBIAO_ZS_TABLE3
1000,						// MUBIAO_ZS_TABLE4
1150,						// MUBIAO_ZS_TABLE5
1250,						// MUBIAO_ZS_TABLE6
1400,						// MUBIAO_ZS_TABLE7
1600,						// MUBIAO_ZS_TABLE8
1800,						// MUBIAO_ZS_TABLE9 ???????????????
9,							// YOUMEN_TABLE_NUM	


25,							// WD_BUCHANG		
5,							// LIUCHE_YALI1_FAZHI
		
1,							// M1_MODE4_MIN_CHESU		
30,						// M1_MODE4_MAX_CHESU		
1,							// M1_MODE6_MIN_CHESU		
12,							// M1_MODE6_MAX_CHESU		
1,							// M1_MODE7_MIN_CHESU		
30,							// M1_MODE7_MAX_CHESU		


//////////////////////page 2 ////////////////////////////////////
//----------------------------------------------------------
700,						// M1_MODE_5_ZS_DI ???????? 600
1500,						// M1_MODE_5_ZS_GAO ???????? 1300
45,							// M1_MODE_5_YL_DI ?????????% 60
85,							// M1_MODE_5_YL_GAO	?????????% 95

5,							// M1_MODE8_YALI_CHA	5%
15,							// M2_MODE5_BAOCHI_YALI	15% 

40,							// SHINENG_ENABLE_PERCENT 
36,							// SHINENG_END_PERCENT

5,							// SHACHE_CHUSU
300,							// ZNYZ_DELAY 

50,							// PID1_PERIOD		default 50ms
50,							// PID1_KP

50,							// PID2_PERIOD		default 50ms
1000,						// PID2_KP	

50,							// PID3_PERIOD		default 20ms
150,						// PID3_KP			default 1.50

50,							// PID4_PERIOD		default 50ms
150,							// PID4_KP			default 0.5

50,							// PID5_PERIOD		default 50ms
30000,						// PID5_KP	

50,							// PID6_PERIOD		default 50ms
150,						// PID6_KP

//----------???-------------------
2,							// M1_ALL_ON_TIME
1,							// M1_FUWEI_MIN_TIME
500,						// M1_ON_PERIOD

//----------???-------------------
2,							// M2_ALL_ON_TIME
1,							// M2_FUWEI_MIN_TIME
500,						// M2_ON_PERIOD

100,							// ZHUANSU_FANWEI
1000,							// WD_KG_PRD

1100,						// BYF_ON_ZHUANSU
5,							// BYF_OFF_YALI3

50,							// ZHIDONG_MAX_PERCENT
10,							// ZHUANSU_TAG_RANGE

50,							// FANTUI_OFF_YALI2	
37,							// FANTUI_START_YALI2
0,							// CHESU_SEL

1,							// M2_MODE4_MIN_CHESU
50,							// M2_MODE4_MAX_CHESU

5,							//WENDU_RANGE 

};

HAL_StatusTypeDef writeAT24C16(uint16_t addr, uint8_t *data, uint16_t len)
{
	uint8_t wNum = 0;
	uint16_t lenLeft = len;
	uint8_t deviceId ;
	uint8_t *p = data;
	
	//is the address overfolw
	if(addr + len >= E2PROM_SIZE)	return HAL_ERROR;
	
	//calculate the current write position to know how many word can write continully
	wNum = EE_PAGE_SIZE - addr & E2PROM_PAGE_MASK;
	if(wNum == 0)	wNum = EE_PAGE_SIZE;
	wNum = lenLeft>=wNum ? wNum : lenLeft;
	
	
	//transmit the date to e2prom

	AlarmFlag&=~I2C_ERROR;
	while(lenLeft)
	{
		//calculate the device id
		deviceId = (addr >> 8)<=0 ? E2PROM_BASE_WID : (E2PROM_BASE_WID | (uint8_t)((addr>>7)&0x0E));
		
		if( HAL_I2C_Mem_Write(&hi2c2, deviceId, addr&0x00FF, I2C_MEMADD_SIZE_8BIT, p, wNum, I2C_TIMEOUT) != HAL_OK)
			AlarmFlag|=I2C_ERROR;

		addr += wNum;
		lenLeft -= wNum;
		p += wNum;
		wNum = lenLeft > EE_PAGE_SIZE ? EE_PAGE_SIZE : lenLeft;
	 
		osDelay(5);
		//HAL_Delay(5);
	}
	if(AlarmFlag&I2C_ERROR) return HAL_ERROR;
	else return HAL_OK;
}
 
HAL_StatusTypeDef readAT24C16(uint16_t addr, uint8_t *data, uint16_t len)
{
	uint8_t rNum = 0;
	uint16_t lenLeft = len;
	uint8_t deviceId ;
	uint8_t *p = data;
 
	//is the address overfolw
	if(addr + len >= E2PROM_SIZE)	return HAL_ERROR;
	
	//calculate the current write position to know how many word can write continully
	rNum = EE_PAGE_SIZE - addr & E2PROM_PAGE_MASK;
	if(rNum == 0)	rNum = EE_PAGE_SIZE;
	rNum = lenLeft>=rNum ? rNum : lenLeft;
	
	//transmit the date to e2prom

	AlarmFlag&=~I2C_ERROR;
	while(lenLeft)
	{
		//calculate the device id
		deviceId = (addr >> 8)<=0 ? E2PROM_BASE_RID : (E2PROM_BASE_RID | (uint8_t)((addr>>7)&0x0E));
		
		if( HAL_I2C_Mem_Read(&hi2c2, deviceId, addr&0x00FF, I2C_MEMADD_SIZE_8BIT, p, rNum, I2C_TIMEOUT) != HAL_OK)
			AlarmFlag|=I2C_ERROR;

		addr += rNum;
		lenLeft -= rNum;
		p += rNum;
		rNum = lenLeft > EE_PAGE_SIZE ? EE_PAGE_SIZE : lenLeft;
	}
	if(AlarmFlag&I2C_ERROR) return HAL_ERROR;
	else return HAL_OK;
}
/* 
void UpdateParameters(uint8_t* buf)
{
	memcpy(&Parameters, buf, PARAMETER_NUM*2);
}
*/
void Load_Default_Parameters()
{
int i;
	for(i=0;i<PARAMETER_NUM-2;i++)
		Parameters[i]=Para_Default_Table[i];
//	memcpy(&Parameters, &Para_Default_Table, PARAMETER_NUM*2);
}

void Write_All_Parameters()
{
int i;

	PARAMETER_FLAG=0x5aa5;
	PARAMETER_SUM=0;
	for(i=0;i<PARAMETER_NUM-1;i++)
	{
		PARAMETER_SUM+=Parameters[i];
	}
	if(writeAT24C16(PARAMETER_ADDR, (uint8_t*)&Parameters,PARAMETER_NUM*2)== HAL_OK)
	{
		AlarmFlag&=~I2C_ERROR;
	}
	else
	{
		AlarmFlag|=I2C_ERROR;
	}
}

void Read_All_Parameters()
{
uint16_t sum;
int i;
	
	// read date from EEPROM
	if(readAT24C16(PARAMETER_ADDR, (uint8_t*)&Parameters,PARAMETER_NUM*2)==HAL_OK)
	{
		AlarmFlag&=~I2C_ERROR;
		sum=0;
		for(i=0;i<PARAMETER_NUM-1;i++)
		{
			sum+=Parameters[i];
		}
		if(PARAMETER_FLAG!=0x5aa5 || PARAMETER_SUM!=sum )
		{
			Load_Default_Parameters();
			Write_All_Parameters();
		}
	}
	else
	{
		AlarmFlag|=I2C_ERROR;
	}
}

void test_24()
{
////I2C test
#define BufferSize 284
uint8_t WriteBuffer[BufferSize],ReadBuffer[BufferSize];
	//printf("\r\n***************I2C Example*******************************\r\n");
	int i;
	for(i=0; i<BufferSize; i++)
	{
    WriteBuffer[i]=i;
		ReadBuffer[i]=0xff;
	}
	printf("start write\n");
	writeAT24C16(PARAMETER_ADDR, WriteBuffer,BufferSize);

	printf("start read\n");
	readAT24C16(PARAMETER_ADDR, ReadBuffer,BufferSize);

	for(int i=0;i<BufferSize;i++)
		{
			printf("%02X\t",ReadBuffer[i]);
		}
}
