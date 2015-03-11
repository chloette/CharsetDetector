/*
* iso2022_jp_verifier.h
* 
* In this file, the interface of ISO2022 JP Verifier is implemented.
*
*/

#ifndef _ISO2022_JP_VERIFIER_H_
#define _ISO2022_JP_VERIFIER_H_

#include "i_charset_verifier.h"

class CISO2022JPVerifier :
	public ICharsetVerifier
{
public:
	CISO2022JPVerifier();

	virtual	~CISO2022JPVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_ISO2022_JP_VERIFIER_H_
