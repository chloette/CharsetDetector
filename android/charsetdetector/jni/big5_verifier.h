/*
* big5_verifier.h
* 
* In this file, the interface of Big5 Verifier is implemented.
*
*/

#ifndef _BIG5_VERIFIER_H_
#define _BIG5_VERIFIER_H_

#include "i_charset_verifier.h"

class CBig5Verifier :
	public ICharsetVerifier
{
public:
	CBig5Verifier();

	virtual	~CBig5Verifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_BIG5_VERIFIER_H_
