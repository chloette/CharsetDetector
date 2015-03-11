#include "charset_detector_common_inc.h"
#include "euc_sampler.h"
#include "math.h"

#define DEFAULT_SAMPLER_LENGTH 94

CEUCSampler::CEUCSampler()
{
	mTotal		= 0;
	mThreshold	= 200;
	mState		= 0;
	mFirstByteCnt	= NULL;
	mSecondByteCnt	= NULL;
	mFirstByteFreq	= NULL;
	mSecondByteFreq = NULL;

}

CEUCSampler::~CEUCSampler()
{
	Destroy();
}

long	CEUCSampler::Init()
{
	long lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	mFirstByteCnt = (unsigned long*)malloc(sizeof(float) * DEFAULT_SAMPLER_LENGTH);
	mSecondByteCnt = (unsigned long*)malloc(sizeof(float) * DEFAULT_SAMPLER_LENGTH);
	mFirstByteFreq = (float*)malloc(sizeof(float) * DEFAULT_SAMPLER_LENGTH);
	mSecondByteFreq = (float*)malloc(sizeof(float) * DEFAULT_SAMPLER_LENGTH);
	if(	NULL == mFirstByteCnt ||
		NULL == mFirstByteFreq ||
		NULL == mSecondByteCnt ||
		NULL == mSecondByteFreq)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	memset(mFirstByteCnt, 0, sizeof(float) * DEFAULT_SAMPLER_LENGTH);
	memset(mSecondByteCnt, 0, sizeof(float) * DEFAULT_SAMPLER_LENGTH);
	memset(mFirstByteFreq, 0, sizeof(float) * DEFAULT_SAMPLER_LENGTH);
	memset(mSecondByteFreq, 0, sizeof(float) * DEFAULT_SAMPLER_LENGTH);

	Reset();

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

long	CEUCSampler::Destroy()
{
	if(mFirstByteCnt)
	{
		free(mFirstByteCnt);
		mFirstByteCnt = NULL;
	}

	if(mFirstByteFreq)
	{
		free(mFirstByteFreq);
		mFirstByteFreq = NULL;
	}

	if(mSecondByteCnt)
	{
		free(mSecondByteCnt);
		mSecondByteCnt = NULL;
	}

	if(mSecondByteFreq)
	{
		free(mSecondByteFreq);
		mSecondByteFreq = NULL;
	}

	return ERR_NONE;
}

void CEUCSampler::Reset()
{
	if(	NULL == mFirstByteCnt ||
		NULL == mSecondByteCnt)
	{
		return;
	}

	mTotal = 0;
	mState = 0;

	for (long lIndex = 0; lIndex < DEFAULT_SAMPLER_LENGTH; ++lIndex)
	{
		mFirstByteCnt[lIndex] = 0;
		mSecondByteCnt[lIndex] = 0;
	}
}

bool CEUCSampler::EnoughData()
{
	return mTotal > mThreshold;
}

bool CEUCSampler::GetSomeData()
{
	return mTotal > 1;
}

bool CEUCSampler::Sample(char* pBuf, unsigned long dwLength)
{
	if (mState == 1) {
		return false;
	}
	long p = 0;
	for (long i = 0; i < dwLength && 1 != mState;) {
		switch (mState) {
			case 0: // '\0'
				if ((pBuf[p] & 0x80) == 0) {
					break;
				}
				if (255 == (0xff & pBuf[p]) || 161 > (0xff & pBuf[p])) {
					mState = 1;
				} else {
					mTotal++;
					mFirstByteCnt[(0xff & pBuf[p]) - 161]++;
					mState = 2;
				}
				break;

			case 1: // '\001'
				break;

			case 2: // '\002'
				if ((pBuf[p] & 0x80) != 0) {
					if (255 == (0xff & pBuf[p]) || 161 > (0xff & pBuf[p])) {
						mState = 1;
					} else {
						mTotal++;
						mSecondByteCnt[(0xff & pBuf[p]) - 161]++;
						mState = 0;
					}
				} else {
					mState = 1;
				}
				break;

			default:
				mState = 1;
				break;
		}
		i++;
		p++;
	}

	return 1 != mState;
}

void CEUCSampler::CalFreq()
{
	if(	NULL == mFirstByteCnt ||
		NULL == mSecondByteCnt)
	{
		return;
	}

	for (long lIndex = 0; lIndex < DEFAULT_SAMPLER_LENGTH; ++lIndex)
	{
		mFirstByteFreq[lIndex] = (float)mFirstByteCnt[lIndex] / (float)mTotal;
		mSecondByteFreq[lIndex] = (float)mSecondByteCnt[lIndex] / (float)mTotal;
	}
}

float	CEUCSampler::GetScore(float* aFirstByteFreq, float aFirstByteWeight, float* aSecondByteFreq, float aSecondByteWeight)
{
	return aFirstByteWeight * GetScore(aFirstByteFreq, mFirstByteFreq) + aSecondByteWeight * GetScore(aSecondByteFreq, mSecondByteFreq);
}

float	CEUCSampler::GetScore(float* array1, float* array2)
{
	float sum = 0.0F;

	for (long lIndex = 0; lIndex < DEFAULT_SAMPLER_LENGTH; ++lIndex)
	{
		float s = array1[lIndex] - array2[lIndex];

		sum += s * s;

		mFirstByteFreq[lIndex] = (float)mFirstByteCnt[lIndex] / (float)mTotal;
		mSecondByteFreq[lIndex] = (float)mSecondByteCnt[lIndex] / (float)mTotal;
	}

	return (float)(sqrt(sum) / 94.0F);
}


