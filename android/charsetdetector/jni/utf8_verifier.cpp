#include "charset_detector_common_inc.h"
#include "utf8_verifier.h"

CUTF8Verifier::CUTF8Verifier()
{

}

CUTF8Verifier::~CUTF8Verifier()
{

}

long	CUTF8Verifier::Init()
{
	long	lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_dwCclassLength = 32;
	m_dwStatesLength = 26;
	
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
	m_dwCclassArray[16] = 0x33332222;
	m_dwCclassArray[17] = 0x44444444;
	m_dwCclassArray[18] = 0x44444444;
	m_dwCclassArray[19] = 0x44444444;
	m_dwCclassArray[20] = 0x55555555;
	m_dwCclassArray[21] = 0x55555555;
	m_dwCclassArray[22] = 0x55555555;
	m_dwCclassArray[23] = 0x55555555;
	m_dwCclassArray[24] = 0x66666600;
	m_dwCclassArray[25] = 0x66666666;
	m_dwCclassArray[26] = 0x66666666;
	m_dwCclassArray[27] = 0x66666666;
	m_dwCclassArray[28] = 0x88888887;
	m_dwCclassArray[29] = 0x88988888;
	m_dwCclassArray[30] = 0xbbbbbbba;
	m_dwCclassArray[31] = 0xfedddc;

	m_dwStatesArray[0] = 0xac111101;
	m_dwStatesArray[1] = 0x345678b9;
	m_dwStatesArray[2] = 0x11111111;
	m_dwStatesArray[3] = 0x11111111;
	m_dwStatesArray[4] = 0x22222222;
	m_dwStatesArray[5] = 0x22222222;
	m_dwStatesArray[6] = 0x11555511;
	m_dwStatesArray[7] = 0x11111111;
	m_dwStatesArray[8] = 0x11555111;
	m_dwStatesArray[9] = 0x11111111;
	m_dwStatesArray[10] = 0x11777711;
	m_dwStatesArray[11] = 0x11111111;
	m_dwStatesArray[12] = 0x11771111;
	m_dwStatesArray[13] = 0x11111111;
	m_dwStatesArray[14] = 0x11999911;
	m_dwStatesArray[15] = 0x11111111;
	m_dwStatesArray[16] = 0x11911111;
	m_dwStatesArray[17] = 0x11111111;
	m_dwStatesArray[18] = 0x11cccc11;
	m_dwStatesArray[19] = 0x11111111;
	m_dwStatesArray[20] = 0x11c11111;
	m_dwStatesArray[21] = 0x11111111;
	m_dwStatesArray[22] = 0x111ccc11;
	m_dwStatesArray[23] = 0x11111111;
	m_dwStatesArray[24] = 0x11000011;
	m_dwStatesArray[25] = 0x11111111;

	m_dwSTFactor = 16;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CUTF8Verifier::Charset()
{
	return ECodepageUTF8;
}

bool CUTF8Verifier::IsUCS2()
{
	return false;
}
