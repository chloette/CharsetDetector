#include "charset_detector_common_inc.h"
#include "unicode_detector.h"

ICodepageDetector* CUnicodeDetector::m_pStaticInstance = NULL;

CUnicodeDetector::CUnicodeDetector()
{
	m_pStaticInstance = NULL;
}

CUnicodeDetector::~CUnicodeDetector()
{
	m_pStaticInstance = NULL;
}

ICodepageDetector*	CUnicodeDetector::GetInstance()
{
	if(NULL == m_pStaticInstance)
	{
		m_pStaticInstance = new CUnicodeDetector();
	}

	return m_pStaticInstance;
}

long CUnicodeDetector::DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType)
{
	long	lResult			= ERR_UNSUPPORTED;
	signed char*	pSingnedBuffer	= (signed char*)pBuffer;

	Log( ("CUnicodeDetector::DetectCodepage IN pBuffer = %d, dwBugLength = %d \r\n"), pBuffer, dwBufLength);

	if(NULL == pBuffer ||
		dwBufLength < 4 ||
		NULL == peType)
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	Log( ("CUnicodeDetector::DetectCodepage HERE 1 \r\n"));
	Log( ("CUnicodeDetector::DetectCodepage Sizeof(char) = %d , Sizeof(short) = %d \r\n"), sizeof(char), sizeof(short));
	Log( ("CUnicodeDetector::DetectCodepage pSingnedBuffer[0] = %d, pSingnedBuffer[1] = %d, pSingnedBuffer[2] = %d \r\n"), pSingnedBuffer[0], pSingnedBuffer[1], pSingnedBuffer[2]);

	if (0 == pSingnedBuffer[0] && 0 == pSingnedBuffer[1] && -2 == pSingnedBuffer[2] && -1 == pSingnedBuffer[3]) 
	{
		Log( ("CUnicodeDetector::DetectCodepage HERE 2 \r\n"));

		*peType = ECodepageUCS4BE;
		lResult = ERR_NONE;
	}
	else if (-1 == pSingnedBuffer[0] && -2 == pSingnedBuffer[1] && 0 == pSingnedBuffer[2] && 0 == pSingnedBuffer[3]) 
	{
		Log( ("CUnicodeDetector::DetectCodepage HERE 3 \r\n"));

		*peType = ECodepageUCS4LE;
		lResult = ERR_NONE;
	}
	else if (-17 == pSingnedBuffer[0] && -69 == pSingnedBuffer[1] && -65 == pSingnedBuffer[2]) 
	{
		Log( ("CUnicodeDetector::DetectCodepage HERE 4 \r\n"));

		*peType = ECodepageUTF8;
		lResult = ERR_NONE;
	}
	else if (-1 == pSingnedBuffer[0] && -2 == pSingnedBuffer[1]) 
	{
		Log( ("CUnicodeDetector::DetectCodepage HERE 5 \r\n"));

		*peType = ECodepageUCS2LE;
		lResult = ERR_NONE;
	}
	else if (-1 == pSingnedBuffer[0] && -1 == pSingnedBuffer[1]) 
	{
		Log( ("CUnicodeDetector::DetectCodepage HERE 6 \r\n"));

		*peType = ECodepageUCS2BE;
		lResult = ERR_NONE;
	}
	else if (0 == pSingnedBuffer[0] && 0 == pSingnedBuffer[1] && -2 == pSingnedBuffer[2] && -1 == pSingnedBuffer[3]) 
	{
		Log( ("CUnicodeDetector::DetectCodepage HERE 7 \r\n"));

		*peType = ECodepageUCS4BE;
		lResult = ERR_NONE;
	} 
	else 
	{
		Log( ("CUnicodeDetector::DetectCodepage HERE 8 \r\n"));

		*peType = ECodepageUNKNOWN;
		lResult = ERR_UNKNOWN;
	}
	if(lResult)
		goto EXIT;

	lResult = ERR_NONE;
EXIT:

	Log("CUnicodeDetector::DetectCodepage OUT lResult = %d \r\n", lResult);

	return lResult;
}
