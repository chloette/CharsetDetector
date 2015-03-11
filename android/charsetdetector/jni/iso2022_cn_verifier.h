/*
* iso2022_cn_verifier.h
* 
* In this file, the interface of ISO2022 CN Verifier is implemented.
*
*/

#ifndef _ISO2022_CN_VERIFIER_H_
#define _ISO2022_CN_VERIFIER_H_

#include "i_charset_verifier.h"

class CISO2022CNVerifier :
	public ICharsetVerifier
{
public:
	CISO2022CNVerifier();

	virtual	~CISO2022CNVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_ISO2022_CN_VERIFIER_H_
