/*
* ucs2le_verifier.h
* 
* In this file, the interface of UCS2LE Verifier is implemented.
*
*/

#ifndef _UCS2LE_VERIFIER_H_
#define _UCS2LE_VERIFIER_H_

#include "i_charset_verifier.h"

class CUCS2LEVerifier :
	public ICharsetVerifier
{
public:
	CUCS2LEVerifier();

	virtual	~CUCS2LEVerifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_UCS2LE_VERIFIER_H_
