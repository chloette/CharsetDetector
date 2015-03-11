/*
* iso2022_kr_verifier.h
* 
* In this file, the interface of ISO2022 KR Verifier is implemented.
*
*/

#ifndef _ISO2022_KR_VERIFIER_H_
#define _ISO2022_KR_VERIFIER_H_

#include "i_charset_verifier.h"

class CISO2022KRVerifier :
	public ICharsetVerifier
{
public:
	CISO2022KRVerifier();

	virtual	~CISO2022KRVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_ISO2022_KR_VERIFIER_H_
