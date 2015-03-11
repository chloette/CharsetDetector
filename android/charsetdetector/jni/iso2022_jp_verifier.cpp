#include "charset_detector_common_inc.h"
#include "iso2022_jp_verifier.h"

CISO2022JPVerifier::CISO2022JPVerifier()
{

}

CISO2022JPVerifier::~CISO2022JPVerifier()
{

}

long	CISO2022JPVerifier::Init()
{
	long	lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_dwCclassLength = 32;
	m_dwStatesLength = 6;
	
	m_dwCclassArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwCclassLength);
	m_dwStatesArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwStatesLength);
	if(	NULL == m_dwCclassArray ||
		NULL == m_dwStatesArray)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	m_dwCclassArray[0] = 2;
	m_dwCclassArray[1] = 0x22000000;
	m_dwCclassArray[2] = 0;
	m_dwCclassArray[3] = 4096;
	m_dwCclassArray[4] = 0x70000;
	m_dwCclassArray[5] = 3;
	m_dwCclassArray[6] = 0;
	m_dwCclassArray[7] = 0;
	m_dwCclassArray[8] = 1030;
	m_dwCclassArray[9] = 1280;
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

	m_dwStatesArray[0] = 304;
	m_dwStatesArray[1] = 0x11111111;
	m_dwStatesArray[2] = 0x22222222;
	m_dwStatesArray[3] = 0x41115111;
	m_dwStatesArray[4] = 0x12121111;
	m_dwStatesArray[5] = 0x11221111;

	m_dwSTFactor = 8;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CISO2022JPVerifier::Charset()
{
	return ECodepageISO2022JP;
}

bool CISO2022JPVerifier::IsUCS2()
{
	return false;
}
