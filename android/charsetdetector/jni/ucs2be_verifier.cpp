#include "charset_detector_common_inc.h"
#include "ucs2be_verifier.h"

CUCS2BEVerifier::CUCS2BEVerifier()
{

}

CUCS2BEVerifier::~CUCS2BEVerifier()
{

}

long	CUCS2BEVerifier::Init()
{
	long	lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_dwCclassLength = 32;
	m_dwStatesLength = 7;
	
	m_dwCclassArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwCclassLength);
	m_dwStatesArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwStatesLength);
	if(	NULL == m_dwCclassArray ||
		NULL == m_dwStatesArray)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	m_dwCclassArray[0] = 0;
	m_dwCclassArray[1] = 0x200100;
	m_dwCclassArray[2] = 0;
	m_dwCclassArray[3] = 12288;
	m_dwCclassArray[4] = 0;
	m_dwCclassArray[5] = 0x333330;
	m_dwCclassArray[6] = 0;
	m_dwCclassArray[7] = 0;
	m_dwCclassArray[8] = 0;
	m_dwCclassArray[9] = 0;
	m_dwCclassArray[10] = 0;
	m_dwCclassArray[11] = 0;
	m_dwCclassArray[12] = 0;
	m_dwCclassArray[13] = 0;
	m_dwCclassArray[14] = 0;
	m_dwCclassArray[15] = 0;
	m_dwCclassArray[16] = 0;
	m_dwCclassArray[17] = 0;
	m_dwCclassArray[18] = 0;
	m_dwCclassArray[19] = 0;
	m_dwCclassArray[20] = 0;
	m_dwCclassArray[21] = 0;
	m_dwCclassArray[22] = 0;
	m_dwCclassArray[23] = 0;
	m_dwCclassArray[24] = 0;
	m_dwCclassArray[25] = 0;
	m_dwCclassArray[26] = 0;
	m_dwCclassArray[27] = 0;
	m_dwCclassArray[28] = 0;
	m_dwCclassArray[29] = 0;
	m_dwCclassArray[30] = 0;
	m_dwCclassArray[31] = 0x54000000;

	m_dwStatesArray[0] = 0x11341775;
	m_dwStatesArray[1] = 0x22221111;
	m_dwStatesArray[2] = 0x11666622;
	m_dwStatesArray[3] = 0x66266666;
	m_dwStatesArray[4] = 0x17756666;
	m_dwStatesArray[5] = 0x66616685;
	m_dwStatesArray[6] = 0x116666;

	m_dwSTFactor = 6;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CUCS2BEVerifier::Charset()
{
	return ECodepageUCS2BE;
}

bool CUCS2BEVerifier::IsUCS2()
{
	return true;
}
