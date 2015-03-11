/*
* gb2312_verifier.h
* 
* In this file, the interface of GB2312 Verifier is implemented.
*
*/

#ifndef _GB2312_VERIFIER_H_
#define _GB2312_VERIFIER_H_

#include "i_charset_verifier.h"

class CGB2312Verifier :
	public ICharsetVerifier
{
public:
	CGB2312Verifier();

	virtual	~CGB2312Verifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_GB2312_VERIFIER_H_
