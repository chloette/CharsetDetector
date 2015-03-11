/*
* hz_verifier.h
* 
* In this file, the interface of HZ Verifier is implemented.
*
*/

#ifndef _HZ_VERIFIER_H_
#define _HZ_VERIFIER_H_

#include "i_charset_verifier.h"

class CHZVerifier :
	public ICharsetVerifier
{
public:
	CHZVerifier();

	virtual	~CHZVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_HZ_VERIFIER_H_
