/*
* jchar_detector.h
* 
* In this file, the interface of JChar Detector is implemented.
*
*/

#ifndef _JCHAR_DETECTOR_H_
#define _JCHAR_DETECTOR_H_

#include "charset_detector.h"

class CJCharDetector:
	public ICodepageDetector
{
public:
	CJCharDetector();

	virtual	~CJCharDetector();

	static	ICodepageDetector*	GetInstance();	

protected:
	long	Init();

	long	Destroy();

public:
	virtual	long	DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType);

	virtual	long	SetConfig(unsigned long dwCfgType, void* pData);

private:
	static ICodepageDetector*	m_pStaticInstance;

	CCharsetDetector*	m_pCharsetDetector;
};

#endif //_JCHAR_DETECTOR_H_
