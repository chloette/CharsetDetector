#include "charset_detector_common_inc.h"
#include "iso2022_kr_verifier.h"

CISO2022KRVerifier::CISO2022KRVerifier()
{

}

CISO2022KRVerifier::~CISO2022KRVerifier()
{

}

long	CISO2022KRVerifier::Init()
{
	long	lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_dwCclassLength = 32;
	m_dwStatesLength = 5;
	
	m_dwCclassArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwCclassLength);
	m_dwStatesArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwStatesLength);
	if(	NULL == m_dwCclassArray ||
		NULL == m_dwStatesArray)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	m_dwCclassArray[0] = 2;
	m_dwCclassArray[1] = 0;
	m_dwCclassArray[2] = 0;
	m_dwCclassArray[3] = 4096;
	m_dwCclassArray[4] = 0x30000;
	m_dwCclassArray[5] = 64;
	m_dwCclassArray[6] = 0;
	m_dwCclassArray[7] = 0;
	m_dwCclassArray[8] = 20480;
	m_dwCclassArray[9] = 0;
	m_dwCclassArray[10] = 0;
	m_dwCclassArray[11] = 0;
	m_dwCclassArray[12] = 0;
	m_dwCclassArray[13] = 0;
	m_dwCclassArray[14] = 0;
	m_dwCclassArray[15] = 0;
	m_dwCclassArray[16] = 0x22222222;
	m_dwCclassArray[17] = 0x22222222;
	m_dwCclassArray[18] = 0x22222222;
	m_dwCclassArray[19] = 0x22222222;
	m_dwCclassArray[20] = 0x22222222;
	m_dwCclassArray[21] = 0x22222222;
	m_dwCclassArray[22] = 0x22222222;
	m_dwCclassArray[23] = 0x22222222;
	m_dwCclassArray[24] = 0x22222222;
	m_dwCclassArray[25] = 0x22222222;
	m_dwCclassArray[26] = 0x22222222;
	m_dwCclassArray[27] = 0x22222222;
	m_dwCclassArray[28] = 0x22222222;
	m_dwCclassArray[29] = 0x22222222;
	m_dwCclassArray[30] = 0x22222222;
	m_dwCclassArray[31] = 0x22222222;

	m_dwStatesArray[0] = 0x11000130;
	m_dwStatesArray[1] = 0x22221111;
	m_dwStatesArray[2] = 0x11411122;
	m_dwStatesArray[3] = 0x11151111;
	m_dwStatesArray[4] = 8465;

	m_dwSTFactor = 6;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CISO2022KRVerifier::Charset()
{
	return ECodepageISO2022KR;
}

bool CISO2022KRVerifier::IsUCS2()
{
	return false;
}
