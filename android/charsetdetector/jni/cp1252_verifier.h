/*
* cp1252_verifier.h
* 
* In this file, the interface of CP1252 Verifier is implemented.
*
*/

#ifndef _CP1252_VERIFIER_H_
#define _CP1252_VERIFIER_H_

#include "i_charset_verifier.h"

class CCP1252Verifier :
	public ICharsetVerifier
{
public:
	CCP1252Verifier();

	virtual	~CCP1252Verifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_CP1252_VERIFIER_H_
