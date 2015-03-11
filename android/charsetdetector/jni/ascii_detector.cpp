#include "charset_detector_common_inc.h"
#include "ascii_detector.h"

ICodepageDetector* CASCIIDetector::m_pStaticInstance = NULL;

CASCIIDetector::CASCIIDetector()
{
	m_pStaticInstance = NULL;
}

CASCIIDetector::~CASCIIDetector()
{
	m_pStaticInstance = NULL;
}

ICodepageDetector*	CASCIIDetector::GetInstance()
{
	if(NULL == m_pStaticInstance)
	{
		m_pStaticInstance = new CASCIIDetector();
	}

	return m_pStaticInstance;
}

long CASCIIDetector::DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType)
{
	long	lResult = ERR_UNSUPPORTED;

	if(NULL == pBuffer ||
		dwBufLength < 4 ||
		NULL == peType)
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	//just judge DEFAULT_MAX_STREAM_BUFFER_SIZE
	if(IsASCII(pBuffer, dwBufLength > DEFAULT_MAX_STREAM_BUFFER_SIZE ? DEFAULT_MAX_STREAM_BUFFER_SIZE : dwBufLength))
	{
		*peType = ECodepageASCII;
		lResult = ERR_NONE;
	}
	else 
	{
		*peType = ECodepageUNKNOWN;
		lResult = ERR_UNKNOWN;
	}
	if(lResult)
		goto EXIT;

	lResult = ERR_NONE;
EXIT:
	
	return lResult;
}

bool	CASCIIDetector::IsASCII(char* pBuffer, unsigned long dwBufLength)
{
	bool bASCII = false;

	if(	NULL == pBuffer ||
		0 >= dwBufLength)
	{
		return false;
	}

	for(long lIndex = 0; lIndex < dwBufLength; ++lIndex)
	{
		if(0 <= pBuffer[lIndex] && pBuffer[lIndex] <= 127)
		{
			bASCII = true;
			continue;
		}
		else
		{
			bASCII = false;
			break;
		}
	}

	return bASCII;
}
