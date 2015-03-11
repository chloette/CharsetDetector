/*
* unicode_detector.h
* 
* In this file, the interface of Unicode Detector is implemented.
*
*/

#ifndef _UNICODE_DETECTOR_H_
#define _UNICODE_DETECTOR_H_

#include "i_codepage_detector.h"

class CUnicodeDetector:
	public ICodepageDetector
{
public:
	CUnicodeDetector();

	virtual	~CUnicodeDetector();

	static	ICodepageDetector*	GetInstance();

public:
	virtual	long	DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType);

private:
	static ICodepageDetector*	m_pStaticInstance;

};

#endif //_UNICODE_DETECTOR_H_
