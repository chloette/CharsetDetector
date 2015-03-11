/*
* gb18030_verifier.h
* 
* In this file, the interface of GB18030 Verifier is implemented.
*
*/

#ifndef _GB18030_VERIFIER_H_
#define _GB18030_VERIFIER_H_

#include "i_charset_verifier.h"

class CGB18030Verifier :
	public ICharsetVerifier
{
public:
	CGB18030Verifier();

	virtual	~CGB18030Verifier();

	virtual	long	Init();

public:
	virtual ECodepageType	Charset();

	virtual bool	IsUCS2();
};

#endif //_GB18030_VERIFIER_H_
