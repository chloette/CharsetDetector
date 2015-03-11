/*
* i_charset_verifier.h
* 
* In this file, the interface of CharsetVerifier is implemented.
*
*/

#ifndef _I_CHARSET_VERIFIER_H_
#define _I_CHARSET_VERIFIER_H_

#define		VERIFIER_START	0
#define		VERIFIER_ERROR	1
#define		VERIFIER_ITSME	2

#define		VERIFIER_IDXSFT4BITS	3
#define		VERIFIER_SFTMSK4BITS	7
#define		VERIFIER_BITSFT4BITS	2
#define		VERIFIER_UNITMSK4BITS	15

#include "i_codepage_detector.h"

class ICharsetVerifier
{
public:
	ICharsetVerifier();

	virtual	~ICharsetVerifier();

	virtual	long	Init() = 0;

	virtual	long	Destroy();

public:
	virtual ECodepageType	Charset() = 0;

	virtual unsigned long	STFactor();

	virtual unsigned long*	Cclass();

	virtual	unsigned long	CclassLength();

	virtual unsigned long*	States();

	virtual unsigned long	StatesLength();

	virtual bool	IsUCS2() = 0;

	static	unsigned char	getNextState(ICharsetVerifier* pVerifier, char b, char s);

protected:
	unsigned long*	m_dwCclassArray;
	unsigned long* m_dwStatesArray;
	unsigned long	m_dwCclassLength;
	unsigned long	m_dwStatesLength;
	unsigned long	m_dwSTFactor;
};

#endif //_I_CHARSET_VERIFIER_H_
