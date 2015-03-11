/*
* codepage_detector_proxy.h
* 
* In this file, the interface of Codepage Detector Proxy is implemented.
*
*/

#ifndef _CODEPAGE_DETECTOR_PROXY_H_
#define _CODEPAGE_DETECTOR_PROXY_H_

#include "i_codepage_detector.h"

#define DEFAULT_DETECTOR_COUNT 3

class CCodepageDetectorProxy :
	public ICodepageDetector
{
public:
	CCodepageDetectorProxy();

	virtual	~CCodepageDetectorProxy();

	static	ICodepageDetector*	GetInstance();
	
	static	ICodepageDetector*	GetInstance(bool bCanGuess);

protected:
	long	Init();

	long	Destroy();

public:
	virtual	long	DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType);

public:
	long	EnableGuess(bool bCanGuess);		//DEFAULT: unable to guess

private:
	ICodepageDetector*	m_DetectorArray[DEFAULT_DETECTOR_COUNT];

	static ICodepageDetector*	m_pStaticInstance;
};

#endif //_CODEPAGE_DETECTOR_PROXY_H_
