/*
*********************************************************************************************************
*    ? ? ?: arm_rfft_fast_f32_app
*    ????: ????arm_rfft_fast_f32??1024????????????????arm_cfft_f32???????
*    ?    ?:?
*    ? ? ?: ?
*********************************************************************************************************
*/
#include "TaskApp.h"
#include "arm_math.h"
#include "arm_const_structs.h"

q15_t FFT_in[NPT];
q15_t FFT_out[NPT];

#define PI2 2*3.1415926f

void q15Sin(q15_t *lBUFIN,long nfill, long Fs, long Freq1, long Freq2, long Ampli)
{
 
  float fFs, fFreq1, fFreq2, fAmpli;
  float fZ,fY;

  fFs = (float) Fs;
  fFreq1 = (float) Freq1;
  fFreq2 = (float) Freq2;
  fAmpli = (float) Ampli;

  for (uint32_t i=0; i < nfill; i++)
  {
    fY = sin(PI2 * i * (fFreq1/fFs)) + sin(PI2 * i * (fFreq2/fFs));
    fZ = fAmpli * fY;
    lBUFIN[i]= ((q15_t)fZ);//<< 16 ;  /* sine_cosine  (cos=0x0) */
    //lBUFIN[i]=((short)(32767*sin(PI2 * i * (fFreq1/fFs))))<<16;
  }
}


uint16_t power(uint32_t in)
{
  uint16_t X,Y,out;
	long Z2;
  //X= (in<<16)>>16; /* sine_cosine --> cos */
  X= in&0xffff; /* sine_cosine --> cos */
  Y= in >> 16;   /* sine_cosine --> sin */    
	Z2=X*X+Y*Y;
	out = sqrt(Z2);
	return out;
}

void calc_fft_q15(uint16_t * all_data_in,uint16_t * result)//16-346us 3-66us
{
//  uint16_t X,Y,out;
//  long Z2;

	arm_rfft_instance_q15 S;
	arm_rfft_init_q15(&S,NPT,0,1);
	for(int j=0;j<ADC_NUMBER;j++)
	{
		for(int i=0;i<NPT;i++)
		{
			FFT_in[i]=all_data_in[ADC_NUMBER*i+j];
		}
		arm_rfft_q15(&S, (q15_t*)FFT_in, (q15_t*)FFT_out);//90us
		//result[j]=power(FFT_out[0]);//100us
		//X= FFT_out[0]&0xffff; /* sine_cosine --> cos */
		//Y= FFT_out[0] >> 16;   /* sine_cosine --> sin */    
		//Z2=X*X+Y*Y;
		//result[j] = sqrt(Z2);
		result[j]=FFT_out[0];
	}
}

float32_t Input_f32[NPT];
float32_t Output_f32[NPT];
float32_t Output[NPT];
void calc_fft(uint16_t * all_data_in,uint16_t * result)
{
	arm_rfft_fast_instance_f32 S1;
	arm_rfft_fast_init_f32(&S1, NPT);
	for(int j=0;j<ADC_NUMBER;j++)
	{
		for(int i=0;i<NPT;i++)
		{
			Input_f32[i]=(float32_t)all_data_in[ADC_NUMBER*i+j]/NPT;
		}
		arm_rfft_fast_f32(&S1, Input_f32, Output_f32, 0);
		arm_cmplx_mag_f32(Output_f32, Output, NPT);
		result[j]=Output[0];
	}
}

