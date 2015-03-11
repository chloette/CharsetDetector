#include "charset_detector_common_inc.h"
#include "euc_jp_verifier.h"

CEUCJPVerifier::CEUCJPVerifier()
{

}

CEUCJPVerifier::~CEUCJPVerifier()
{

}

long	CEUCJPVerifier::Init()
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

	m_dwCclassArray[0] = 0x44444444;
	m_dwCclassArray[1] = 0x55444444;
	m_dwCclassArray[2] = 0x44444444;
	m_dwCclassArray[3] = 0x44445444;
	m_dwCclassArray[4] = 0x44444444;
	m_dwCclassArray[5] = 0x44444444;
	m_dwCclassArray[6] = 0x44444444;
	m_dwCclassArray[7] = 0x44444444;
	m_dwCclassArray[8] = 0x44444444;
	m_dwCclassArray[9] = 0x44444444;
	m_dwCclassArray[10] = 0x44444444;
	m_dwCclassArray[11] = 0x44444444;
	m_dwCclassArray[12] = 0x44444444;
	m_dwCclassArray[13] = 0x44444444;
	m_dwCclassArray[14] = 0x44444444;
	m_dwCclassArray[15] = 0x44444444;
	m_dwCclassArray[16] = 0x55555555;
	m_dwCclassArray[17] = 0x31555555;
	m_dwCclassArray[18] = 0x55555555;
	m_dwCclassArray[19] = 0x55555555;
	m_dwCclassArray[20] = 0x22222225;
	m_dwCclassArray[21] = 0x22222222;
	m_dwCclassArray[22] = 0x22222222;
	m_dwCclassArray[23] = 0x22222222;
	m_dwCclassArray[24] = 0x22222222;
	m_dwCclassArray[25] = 0x22222222;
	m_dwCclassArray[26] = 0x22222222;
	m_dwCclassArray[27] = 0x22222222;
	m_dwCclassArray[28] = 0;
	m_dwCclassArray[29] = 0;
	m_dwCclassArray[30] = 0;
	m_dwCclassArray[31] = 0x50000000;
	m_dwStatesArray[0] = 0x11105343;
	m_dwStatesArray[1] = 0x22221111;
	m_dwStatesArray[2] = 0x11101022;
	m_dwStatesArray[3] = 0x13111011;
	m_dwStatesArray[4] = 0x00001113;

	m_dwSTFactor = 6;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CEUCJPVerifier::Charset()
{
	return ECodepageEUCJP;
}

bool CEUCJPVerifier::IsUCS2()
{
	return false;
}
