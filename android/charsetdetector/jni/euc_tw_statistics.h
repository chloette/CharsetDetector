/*
* euc_tw_statistics.h
* 
* In this file, the interface of [EUC-TW] Statistics is implemented.
*
*/

#ifndef _EUC_TW_STATISTICS_H_
#define _EUC_TW_STATISTICS_H_

#include "i_euc_statistics.h"

class CEUCTWStatistics :
	public IEUCStatistics
{
public:
	CEUCTWStatistics();

	virtual ~CEUCTWStatistics();

public:
	virtual float*	FirstByteFreq();

	virtual float*	SecondByteFreq();
};

#endif //_EUC_TW_STATISTICS_H_
