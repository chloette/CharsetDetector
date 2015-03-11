/*
* sjis_verifier.h
* 
* In this file, the interface of SJIS Verifier is implemented.
*
*/

#ifndef _SJIS_VERIFIER_H_
#define _SJIS_VERIFIER_H_

#include "i_charset_verifier.h"

class CSJISVerifier :
	public ICharsetVerifier
{
public:
	CSJISVerifier();

	virtual	~CSJISVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_SJIS_VERIFIER_H_
