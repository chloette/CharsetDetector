#include "charset_detector_common_inc.h"
#include "euc_tw_verifier.h"

CEUCTWVerifier::CEUCTWVerifier()
{

}

CEUCTWVerifier::~CEUCTWVerifier()
{

}

long	CEUCTWVerifier::Init()
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

	m_dwCclassArray[0] = 0x22222222;
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
	m_dwCclassArray[16] = 0;
	m_dwCclassArray[17] = 0x6000000;
	m_dwCclassArray[18] = 0;
	m_dwCclassArray[19] = 0;
	m_dwCclassArray[20] = 0x44444430;
	m_dwCclassArray[21] = 0x11111155;
	m_dwCclassArray[22] = 0x11111111;
	m_dwCclassArray[23] = 0x11111111;
	m_dwCclassArray[24] = 0x33331311;
	m_dwCclassArray[25] = 0x33333333;
	m_dwCclassArray[26] = 0x33333333;
	m_dwCclassArray[27] = 0x33333333;
	m_dwCclassArray[28] = 0x33333333;
	m_dwCclassArray[29] = 0x33333333;
	m_dwCclassArray[30] = 0x33333333;
	m_dwCclassArray[31] = 0x3333333;
	m_dwStatesArray[0] = 0x14333011;
	m_dwStatesArray[1] = 0x22111111;
	m_dwStatesArray[2] = 0x10122222;
	m_dwStatesArray[3] = 0x11111000;
	m_dwStatesArray[4] = 0x101115;
	m_dwStatesArray[5] = 16;

	m_dwSTFactor = 7;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CEUCTWVerifier::Charset()
{
	return ECodepageEUCTW;
}

bool CEUCTWVerifier::IsUCS2()
{
	return false;
}
