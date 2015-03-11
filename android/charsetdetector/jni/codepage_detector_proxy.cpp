#include "charset_detector_common_inc.h"
#include "codepage_detector_proxy.h"

#include "ascii_detector.h"
#include "unicode_detector.h"
#include "jchar_detector.h"

ICodepageDetector* CCodepageDetectorProxy::m_pStaticInstance = NULL;

CCodepageDetectorProxy::CCodepageDetectorProxy()
{
	memset(m_DetectorArray, 0, sizeof(m_DetectorArray));

	m_pStaticInstance = NULL;
}

CCodepageDetectorProxy::~CCodepageDetectorProxy()
{
	Destroy();
	m_pStaticInstance = NULL;
}

ICodepageDetector*	CCodepageDetectorProxy::GetInstance()
{
	if(NULL == m_pStaticInstance)
	{
		m_pStaticInstance = new CCodepageDetectorProxy();
		if(m_pStaticInstance)
		{
			((CCodepageDetectorProxy*)m_pStaticInstance)->Init();
		}
	}

	return m_pStaticInstance;
}

ICodepageDetector*	CCodepageDetectorProxy::GetInstance(bool bCanGuess)
{
	m_pStaticInstance = CCodepageDetectorProxy::GetInstance();
	if(m_pStaticInstance)
	{
		((CCodepageDetectorProxy*)m_pStaticInstance)->EnableGuess(bCanGuess);
	}

	return m_pStaticInstance;
}

long	CCodepageDetectorProxy::Init()
{
	long lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_DetectorArray[0] = CUnicodeDetector::GetInstance();
	m_DetectorArray[1] = CJCharDetector::GetInstance();
	m_DetectorArray[2] = CASCIIDetector::GetInstance();
	if(	NULL == m_DetectorArray[0] ||
		NULL == m_DetectorArray[1] ||
		NULL == m_DetectorArray[2])
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	
	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{

	}

	return lResult;
}

long	CCodepageDetectorProxy::Destroy()
{
	for(long lIndex = 0; lIndex < DEFAULT_DETECTOR_COUNT; ++lIndex)
	{
		if(m_DetectorArray[lIndex])
		{
			delete m_DetectorArray[lIndex];
			m_DetectorArray[lIndex] = NULL;
		}
	}

	return ERR_NONE;
}

long CCodepageDetectorProxy::DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType)
{
	long	lResult = ERR_UNSUPPORTED;

	if(NULL == pBuffer ||
		dwBufLength < 4 ||
		NULL == peType)
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	for(long lIndex = 0; lIndex < DEFAULT_DETECTOR_COUNT; ++lIndex)
	{
		if(NULL == m_DetectorArray[lIndex])
		{
			continue;
		}

		if(m_DetectorArray[lIndex]->DetectCodepage(dwParserType, pBuffer, dwBufLength, peType))
		{
			continue;
		}

		break;
	}

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		if(peType)
		{
			*peType = ECodepageUNKNOWN;
		}
	}

	return lResult;
}

long	CCodepageDetectorProxy::EnableGuess(bool bCanGuess)
{
	long lResult = ERR_UNSUPPORTED;

	for(long lIndex = 0; lIndex < DEFAULT_DETECTOR_COUNT; ++lIndex)
	{
		if(NULL == m_DetectorArray[lIndex])
		{
			continue;
		}

		//how to check result of different detectors?
		lResult = m_DetectorArray[lIndex]->SetConfig(CODEPAGE_DETECTOR_CONFIG_ENABLE_GUESS, (void*)bCanGuess);
	}

	lResult = ERR_NONE;

	if(lResult)
	{

	}

	return lResult;
}
