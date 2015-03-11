/*
* euc_tw_verifier.h
* 
* In this file, the interface of EUC_TW Verifier is implemented.
*
*/

#ifndef _EUC_TW_VERIFIER_H_
#define _EUC_TW_VERIFIER_H_

#include "i_charset_verifier.h"

class CEUCTWVerifier :
	public ICharsetVerifier
{
public:
	CEUCTWVerifier();

	virtual	~CEUCTWVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_EUC_TW_VERIFIER_H_
