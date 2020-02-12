#ifndef _PARAMETERS_H
#define _PARAMETERS_H

#define AD_1MV		1
#define AD_2MV		2
#define AD_3MV		4
#define AD_4MV		5
#define AD_5MV		6
#define AD_6MV		7
#define AD_7MV		9
#define AD_8MV		10
#define AD_9MV		11
#define AD_10MV		12
#define AD_20MV		25
#define AD_30MV		37
#define AD_40MV		50
#define AD_50MV		62
#define AD_60MV		74
#define AD_70MV		87
#define AD_80MV		99
#define AD_90MV		112
#define AD_100MV	124
#define AD_200MV	248
#define AD_300MV	372
#define AD_400MV	496
#define AD_500MV	620
#define AD_600MV	745
#define AD_700MV	869
#define AD_800MV	993
#define AD_900MV	1117
#define AD_1V			1241
#define AD_2V			2482
#define AD_3V			3723
#define AD_FULL		4095

#define PARAMETER_NUM		142
#define OUT_BEGIN_OFFSET	0
#define OUT_ON_OFFSET			16
#define OUT_OFF_OFFSET		32

#define MIN_AD_V_OFFSET		48
#define V1_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET]
#define V2_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET+1]
#define V3_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET+2]
#define V4_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET+3]
#define V5_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET+4]
#define V6_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET+5]
#define V7_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET+6]
#define V8_MIN_AD_VALUE		Parameters[MIN_AD_V_OFFSET+7]

#define MAX_AD_V_OFFSET		56
#define V1_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET]
#define V2_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET+1]
#define V3_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET+2]
#define V4_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET+3]
#define V5_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET+4]
#define V6_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET+5]
#define V7_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET+6]
#define V8_MAX_AD_VALUE		Parameters[MAX_AD_V_OFFSET+7]


#define M1_RESET_DEL			Parameters[64]
#define M2_RESET_DEL			Parameters[65]
#define M12_POWER_UP			Parameters[66]

#define XLF_POWER_UP			Parameters[67]

#define XITONG_YALI_CHN		Parameters[68]
#define JSF_YALI_BAOHU		Parameters[69]

#define MADA_ZHUANSU_CHN	Parameters[70]
#define JSF_ZHUNASU_BAOHU Parameters[71]

#define MADA_RESET_YALI		Parameters[72]

#define USE_FFT_METHOD		Parameters[73]

//#define BENG_PAILIANG_ENB	Parameters[74]

//#define M1M2_SET_OFFSET		73	
//#define M1_STOP_MIN				Parameters[M1M2_SET_OFFSET]
//#define M1_STOP_MAX				Parameters[M1M2_SET_OFFSET+1]
//#define M2_STOP_MIN				Parameters[M1M2_SET_OFFSET+2]
//#define M2_STOP_MAX				Parameters[M1M2_SET_OFFSET+3]
//#define M1_T_ERROR				Parameters[M1M2_SET_OFFSET+4]
//#define M2_T_ERROR				Parameters[M1M2_SET_OFFSET+5]


#define WENDUCHIP					Parameters[PARAMETER_NUM-3]

#define PARAMETER_FLAG		Parameters[PARAMETER_NUM-2]
#define PARAMETER_SUM			Parameters[PARAMETER_NUM-1]

#define M1_MIN_AD_VALUE			V1_MIN_AD_VALUE
#define M1_MAX_AD_VALUE			V1_MAX_AD_VALUE
#define M2_MIN_AD_VALUE			V2_MIN_AD_VALUE
#define M2_MAX_AD_VALUE			V2_MAX_AD_VALUE
#define YALI1_MIN_AD_VALUE	V3_MIN_AD_VALUE
#define YALI1_MAX_AD_VALUE	V3_MAX_AD_VALUE

#endif
