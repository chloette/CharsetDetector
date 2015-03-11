#include "charset_detector_common_inc.h"
#include "hz_verifier.h"

CHZVerifier::CHZVerifier()
{

}

CHZVerifier::~CHZVerifier()
{

}

long	CHZVerifier::Init()
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

	m_dwCclassArray[0] = 1;
	m_dwCclassArray[1] = 0;
	m_dwCclassArray[2] = 0;
	m_dwCclassArray[3] = 4096;
	m_dwCclassArray[4] = 0;
	m_dwCclassArray[5] = 0;
	m_dwCclassArray[6] = 0;
	m_dwCclassArray[7] = 0;
	m_dwCclassArray[8] = 0;
	m_dwCclassArray[9] = 0;
	m_dwCclassArray[10] = 0;
	m_dwCclassArray[11] = 0;
	m_dwCclassArray[12] = 0;
	m_dwCclassArray[13] = 0;
	m_dwCclassArray[14] = 0;
	m_dwCclassArray[15] = 0x2504000;
	m_dwCclassArray[16] = 0x11111111;
	m_dwCclassArray[17] = 0x11111111;
	m_dwCclassArray[18] = 0x11111111;
	m_dwCclassArray[19] = 0x11111111;
	m_dwCclassArray[20] = 0x11111111;
	m_dwCclassArray[21] = 0x11111111;
	m_dwCclassArray[22] = 0x11111111;
	m_dwCclassArray[23] = 0x11111111;
	m_dwCclassArray[24] = 0x11111111;
	m_dwCclassArray[25] = 0x11111111;
	m_dwCclassArray[26] = 0x11111111;
	m_dwCclassArray[27] = 0x11111111;
	m_dwCclassArray[28] = 0x11111111;
	m_dwCclassArray[29] = 0x11111111;
	m_dwCclassArray[30] = 0x11111111;
	m_dwCclassArray[31] = 0x11111111;

	m_dwStatesArray[0] = 0x11000310;
	m_dwStatesArray[1] = 0x22221111;
	m_dwStatesArray[2] = 0x14001122;
	m_dwStatesArray[3] = 0x14551615;
	m_dwStatesArray[4] = 0x14144414;
	m_dwStatesArray[5] = 36;

	m_dwSTFactor = 6;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CHZVerifier::Charset()
{
	return ECodepageHZ;
}

bool CHZVerifier::IsUCS2()
{
	return false;
}
