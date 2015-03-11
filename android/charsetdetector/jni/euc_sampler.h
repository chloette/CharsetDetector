/*
* euc_sampler.h
* 
* In this file, the interface of EUC Sampler is implemented.
*
*/

#ifndef _EUC_SAMPLER_H_
#define _EUC_SAMPLER_H_

#include "i_charset_verifier.h"

class CEUCSampler
{
public:
	CEUCSampler();

	virtual	~CEUCSampler();

	long	Init();

	long	Destroy();

public:
	void Reset();

	bool EnoughData();

	bool GetSomeData();

	bool Sample(char* pBuf, unsigned long dwLength);

	void CalFreq();

	float	GetScore(float* aFirstByteFreq, float aFirstByteWeight, float* aSecondByteFreq, float aSecondByteWeight);

	float	GetScore(float* array1, float* array2);

private:
	unsigned long mTotal;
	unsigned long mThreshold;
	unsigned long mState;
	unsigned long*	mFirstByteCnt;
	unsigned long* mSecondByteCnt;
	float* mFirstByteFreq;
	float* mSecondByteFreq;
};

#endif //_EUC_SAMPLER_H_
