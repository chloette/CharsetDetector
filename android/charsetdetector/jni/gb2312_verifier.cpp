#include "charset_detector_common_inc.h"
#include "gb2312_verifier.h"

CGB2312Verifier::CGB2312Verifier()
{

}

CGB2312Verifier::~CGB2312Verifier()
{

}

long	CGB2312Verifier::Init()
{
	long	lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_dwCclassLength = 32;
	m_dwStatesLength = 2;
	
	m_dwCclassArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwCclassLength);
	m_dwStatesArray = (unsigned long*)malloc(sizeof(unsigned long) * m_dwStatesLength);
	if(	NULL == m_dwCclassArray ||
		NULL == m_dwStatesArray)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	m_dwCclassArray[0] = 0x11111111;
	m_dwCclassArray[1] = 0x111111;
	m_dwCclassArray[2] = 0x11111111;
	m_dwCclassArray[3] = 0x11110111;
	m_dwCclassArray[4] = 0x11111111;
	m_dwCclassArray[5] = 0x11111111;
	m_dwCclassArray[6] = 0x11111111;
	m_dwCclassArray[7] = 0x11111111;
	m_dwCclassArray[8] = 0x11111111;
	m_dwCclassArray[9] = 0x11111111;
	m_dwCclassArray[10] = 0x11111111;
	m_dwCclassArray[11] = 0x11111111;
	m_dwCclassArray[12] = 0x11111111;
	m_dwCclassArray[13] = 0x11111111;
	m_dwCclassArray[14] = 0x11111111;
	m_dwCclassArray[15] = 0x11111111;
	m_dwCclassArray[16] = 0;;
	m_dwCclassArray[17] = 0;;
	m_dwCclassArray[18] = 0;;
	m_dwCclassArray[19] = 0;;
	m_dwCclassArray[20] = 0x22222220;
	m_dwCclassArray[21] = 0x33333322;
	m_dwCclassArray[22] = 0x22222222;
	m_dwCclassArray[23] = 0x22222222;
	m_dwCclassArray[24] = 0x22222222;
	m_dwCclassArray[25] = 0x22222222;
	m_dwCclassArray[26] = 0x22222222;
	m_dwCclassArray[27] = 0x22222222;
	m_dwCclassArray[28] = 0x22222222;
	m_dwCclassArray[29] = 0x22222222;
	m_dwCclassArray[30] = 0x22222222;
	m_dwCclassArray[31] = 0x2222222;
	m_dwStatesArray[0] = 0x11111301;
	m_dwStatesArray[1] = 0x112222;

	m_dwSTFactor = 4;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CGB2312Verifier::Charset()
{
	return ECodepageGB2312;
}

bool CGB2312Verifier::IsUCS2()
{
	return false;
}
