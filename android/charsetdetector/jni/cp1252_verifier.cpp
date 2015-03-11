#include "charset_detector_common_inc.h"
#include "cp1252_verifier.h"

CCP1252Verifier::CCP1252Verifier()
{

}

CCP1252Verifier::~CCP1252Verifier()
{

}

long	CCP1252Verifier::Init()
{
	long	lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_dwCclassLength = 32;
	m_dwStatesLength = 3;
	
	m_dwCclassArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwCclassLength);
	m_dwStatesArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwStatesLength);
	if(	NULL == m_dwCclassArray ||
		NULL == m_dwStatesArray)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	m_dwCclassArray[0] = 0x22222221;
	m_dwCclassArray[1] = 0x222222;
	m_dwCclassArray[2] = 0x22222222;
	m_dwCclassArray[3] = 0x22220222;
	m_dwCclassArray[4] = 0x22222222;
	m_dwCclassArray[5] = 0x22222222;
	m_dwCclassArray[6] = 0x22222222;
	m_dwCclassArray[7] = 0x22222222;
	m_dwCclassArray[8] = 0x22222222;
	m_dwCclassArray[9] = 0x22222222;
	m_dwCclassArray[10] = 0x22222222;
	m_dwCclassArray[11] = 0x22222222;
	m_dwCclassArray[12] = 0x22222222;
	m_dwCclassArray[13] = 0x22222222;
	m_dwCclassArray[14] = 0x22222222;
	m_dwCclassArray[15] = 0x22222222;
	m_dwCclassArray[16] = 0x22222202;
	m_dwCclassArray[17] = 0x1012122;
	m_dwCclassArray[18] = 0x22222220;
	m_dwCclassArray[19] = 0x11012122;
	m_dwCclassArray[20] = 0x22222222;
	m_dwCclassArray[21] = 0x22222222;
	m_dwCclassArray[22] = 0x22222222;
	m_dwCclassArray[23] = 0x22222222;
	m_dwCclassArray[24] = 0x11111111;
	m_dwCclassArray[25] = 0x11111111;
	m_dwCclassArray[26] = 0x21111111;
	m_dwCclassArray[27] = 0x11111111;
	m_dwCclassArray[28] = 0x11111111;
	m_dwCclassArray[29] = 0x11111111;
	m_dwCclassArray[30] = 0x21111111;
	m_dwCclassArray[31] = 0x11111111;
	m_dwStatesArray[0] = 0x22111031;
	m_dwStatesArray[1] = 0x14510412;
	m_dwStatesArray[2] = 65;

	m_dwSTFactor = 3;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CCP1252Verifier::Charset()
{
	return ECodepageCP1252;
}

bool CCP1252Verifier::IsUCS2()
{
	return false;
}
