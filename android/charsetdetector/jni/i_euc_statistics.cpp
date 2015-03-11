#include "charset_detector_common_inc.h"
#include "i_euc_statistics.h"

IEUCStatistics::IEUCStatistics()
{
	mFirstByteStdDev	= 0.0F;
	mFirstByteMean		= 0.0F;
	mFirstByteWeight	= 0.0F;
	mSecondByteStdDev	= 0.0F;
	mSecondByteMean		= 0.0F;
	mSecondByteWeight	= 0.0F;
}

IEUCStatistics::~IEUCStatistics()
{

}

float IEUCStatistics::FirstByteStdDev()
{
	return mFirstByteStdDev;
}

float IEUCStatistics::FirstByteMean()
{
	return mFirstByteMean;
}

float IEUCStatistics::FirstByteWeight()
{
	return mFirstByteWeight;
}

float IEUCStatistics::SecondByteStdDev()
{
	return mSecondByteStdDev;
}

float IEUCStatistics::SecondByteMean()
{
	return mSecondByteMean;
}

float IEUCStatistics::SecondByteWeight()
{
	return mSecondByteWeight;
}
