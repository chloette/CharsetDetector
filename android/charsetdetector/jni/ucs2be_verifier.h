/*
* ucs2be_verifier.h
* 
* In this file, the interface of UCS2BE Verifier is implemented.
*
*/

#ifndef _UCS2BE_VERIFIER_H_
#define _UCS2BE_VERIFIER_H_

#include "i_charset_verifier.h"

class CUCS2BEVerifier :
	public ICharsetVerifier
{
public:
	CUCS2BEVerifier();

	virtual	~CUCS2BEVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_UCS2BE_VERIFIER_H_
