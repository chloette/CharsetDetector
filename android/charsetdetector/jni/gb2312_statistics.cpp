#include "charset_detector_common_inc.h"
#include "gb2312_statistics.h"

static float s_FirstByteFreq[] = {
	0.011628F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 
	0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.011628F, 0.012403F, 0.009302F, 0.003876F, 0.017829F, 
	0.037209F, 0.008527F, 0.010078F, 0.01938F, 0.054264F, 0.010078F, 0.041085F, 0.02093F, 0.018605F, 0.010078F, 
	0.013178F, 0.016279F, 0.006202F, 0.009302F, 0.017054F, 0.011628F, 0.008527F, 0.004651F, 0.006202F, 0.017829F, 
	0.024806F, 0.020155F, 0.013953F, 0.032558F, 0.035659F, 0.068217F, 0.010853F, 0.036434F, 0.117054F, 0.027907F, 
	0.100775F, 0.010078F, 0.017829F, 0.062016F, 0.012403F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 
	0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 
	0.0F, 0.0F, 0.0F, 0.00155F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 
	0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 
	0.0F, 0.0F, 0.0F, 0.0F
};

static float s_SecondByteFreq[] = {
	0.006202F, 0.031008F, 0.005426F, 0.003101F, 0.00155F, 0.003101F, 0.082171F, 0.014729F, 0.006977F, 0.00155F, 
	0.013953F, 0.0F, 0.013953F, 0.010078F, 0.008527F, 0.006977F, 0.004651F, 0.003101F, 0.003101F, 0.003101F, 
	0.008527F, 0.003101F, 0.005426F, 0.005426F, 0.005426F, 0.003101F, 0.00155F, 0.006202F, 0.014729F, 0.010853F, 
	0.0F, 0.011628F, 0.0F, 0.031783F, 0.013953F, 0.030233F, 0.039535F, 0.008527F, 0.015504F, 0.0F, 
	0.003101F, 0.008527F, 0.016279F, 0.005426F, 0.00155F, 0.013953F, 0.013953F, 0.044961F, 0.003101F, 0.004651F, 
	0.006977F, 0.00155F, 0.005426F, 0.012403F, 0.00155F, 0.015504F, 0.0F, 0.006202F, 0.00155F, 0.0F, 
	0.007752F, 0.006977F, 0.00155F, 0.009302F, 0.011628F, 0.004651F, 0.010853F, 0.012403F, 0.017829F, 0.005426F, 
	0.024806F, 0.0F, 0.006202F, 0.0F, 0.082171F, 0.015504F, 0.004651F, 0.0F, 0.006977F, 0.004651F, 
	0.0F, 0.008527F, 0.012403F, 0.004651F, 0.003876F, 0.003101F, 0.022481F, 0.024031F, 0.00155F, 0.047287F, 
	0.009302F, 0.00155F, 0.005426F, 0.017054F
};

CGB2312Statistics::CGB2312Statistics() 
{
	mFirstByteStdDev	= 0.020081F;
	mFirstByteMean		= 0.010638F;
	mFirstByteWeight	= 0.586533F;
	mSecondByteStdDev	= 0.014156F;
	mSecondByteMean		= 0.010638F;
	mSecondByteWeight	= 0.413467F;
}

CGB2312Statistics::~CGB2312Statistics()
{

}

float* CGB2312Statistics::FirstByteFreq()
{
	return s_FirstByteFreq;
}

float* CGB2312Statistics::SecondByteFreq()
{
	return s_SecondByteFreq;
}