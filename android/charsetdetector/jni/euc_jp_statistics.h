/*
* euc_jp_statistics.h
* 
* In this file, the interface of [EUC-JP] Statistics is implemented.
*
*/

#ifndef _ERC_JP_STATISTICS_H_
#define _ERC_JP_STATISTICS_H_

#include "i_euc_statistics.h"

class CEUCJPStatistics :
	public IEUCStatistics
{
public:
	CEUCJPStatistics();

	virtual ~CEUCJPStatistics();

public:
	virtual float*	FirstByteFreq();

	virtual float*	SecondByteFreq();
};

#endif //_ERC_JP_STATISTICS_H_
