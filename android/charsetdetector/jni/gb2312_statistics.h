/*
* gb2312_statistics.h
* 
* In this file, the interface of GB2312[EUC-CN] Statistics is implemented.
*
*/

#ifndef _GB2312_STATISTICS_H_
#define _GB2312_STATISTICS_H_

#include "i_euc_statistics.h"

class CGB2312Statistics :
	public IEUCStatistics
{
public:
	CGB2312Statistics();

	virtual ~CGB2312Statistics();

public:
	virtual float*	FirstByteFreq();

	virtual float*	SecondByteFreq();
};

#endif //_GB2312_STATISTICS_H_
