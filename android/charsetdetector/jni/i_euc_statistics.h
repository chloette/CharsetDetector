/*
* i_euc_statistics.h
* 
* In this file, the interface of EUC Statistics is implemented.
*
*/

#ifndef _I_EUC_STATISTICS_H_
#define _I_EUC_STATISTICS_H_

class IEUCStatistics
{
public:
	IEUCStatistics();

	virtual	~IEUCStatistics();

public:
	virtual float*	FirstByteFreq() = 0;

	virtual float	FirstByteStdDev();

	virtual float	FirstByteMean();

	virtual float	FirstByteWeight();

	virtual float*	SecondByteFreq() = 0;

	virtual float	SecondByteStdDev();

	virtual float	SecondByteMean();

	virtual float	SecondByteWeight();

protected:
	float mFirstByteStdDev;
	float mFirstByteMean;
	float mFirstByteWeight;

	float mSecondByteStdDev;
	float mSecondByteMean;
	float mSecondByteWeight;
};

#endif //_I_EUC_STATISTICS_H_
