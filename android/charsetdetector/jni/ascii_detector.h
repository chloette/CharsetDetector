/*
* ascii_detector.h
* 
* In this file, the interface of ASCII Detector is implemented.
*
*/

#ifndef _ASCII_DETECTOR_H_
#define _ASCII_DETECTOR_H_

#include "i_codepage_detector.h"

class CASCIIDetector:
	public ICodepageDetector
{
public:
	CASCIIDetector();

	virtual	~CASCIIDetector();

	static	ICodepageDetector*	GetInstance();

public:
	virtual	long	DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType);

protected:
	bool	IsASCII(char* pBuffer, unsigned long dwBufLength);

private:
	static	ICodepageDetector*	m_pStaticInstance;

};

#endif //_ASCII_DETECTOR_H_
