/*
* euc_kr_verifier.h
* 
* In this file, the interface of EUC_KR Verifier is implemented.
*
*/

#ifndef _EUC_KR_VERIFIER_H_
#define _EUC_KR_VERIFIER_H_

#include "i_charset_verifier.h"

class CEUCKRVerifier :
	public ICharsetVerifier
{
public:
	CEUCKRVerifier();

	virtual	~CEUCKRVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_EUC_KR_VERIFIER_H_
