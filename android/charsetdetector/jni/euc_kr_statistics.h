/*
* euc_kr_statistics.h
* 
* In this file, the interface of [EUC-KR] Statistics is implemented.
*
*/

#ifndef _EUC_KR_STATISTICS_H_
#define _EUC_KR_STATISTICS_H_

#include "i_euc_statistics.h"

class CEUCKRStatistics :
	public IEUCStatistics
{
public:
	CEUCKRStatistics();

	virtual ~CEUCKRStatistics();

public:
	virtual float*	FirstByteFreq();

	virtual float*	SecondByteFreq();
};

#endif //_EUC_KR_STATISTICS_H_
