/*
* euc_jp_verifier.h
* 
* In this file, the interface of EUC_JP Verifier is implemented.
*
*/

#ifndef _EUC_JP_VERIFIER_H_
#define _EUC_JP_VERIFIER_H_

#include "i_charset_verifier.h"

class CEUCJPVerifier :
	public ICharsetVerifier
{
public:
	CEUCJPVerifier();

	virtual	~CEUCJPVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_EUC_JP_VERIFIER_H_
