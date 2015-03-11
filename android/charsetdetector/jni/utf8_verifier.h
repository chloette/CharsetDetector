/*
* utf8_verifier.h
* 
* In this file, the interface of UTF8 Verifier is implemented.
*
*/

#ifndef _UTF8_VERIFIER_H_
#define _UTF8_VERIFIER_H_

#include "i_charset_verifier.h"

class CUTF8Verifier :
	public ICharsetVerifier
{
public:
	CUTF8Verifier();

	virtual	~CUTF8Verifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_UTF8_VERIFIER_H_
