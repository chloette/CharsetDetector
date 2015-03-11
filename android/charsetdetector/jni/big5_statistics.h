/*
* big5_statistics.h
* 
* In this file, the interface of BIG5 Statistics is implemented.
*
*/

#ifndef _BIG5_STATISTICS_H_
#define _BIG5_STATISTICS_H_

#include "i_euc_statistics.h"

class CBIG5Statistics :
	public IEUCStatistics
{
public:
	CBIG5Statistics();

	virtual ~CBIG5Statistics();

public:
	virtual float*	FirstByteFreq();

	virtual float*	SecondByteFreq();
};

#endif //_BIG5_STATISTICS_H_
