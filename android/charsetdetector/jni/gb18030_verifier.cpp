#include "charset_detector_common_inc.h"
#include "gb18030_verifier.h"

CGB18030Verifier::CGB18030Verifier()
{

}

CGB18030Verifier::~CGB18030Verifier()
{

}

long	CGB18030Verifier::Init()
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

	m_dwCclassArray[0] = 0x11111111;
	m_dwCclassArray[1] = 0x111111;
	m_dwCclassArray[2] = 0x11111111;
	m_dwCclassArray[3] = 0x11110111;
	m_dwCclassArray[4] = 0x11111111;
	m_dwCclassArray[5] = 0x11111111;
	m_dwCclassArray[6] = 0x33333333;
	m_dwCclassArray[7] = 0x11111133;
	m_dwCclassArray[8] = 0x22222222;
	m_dwCclassArray[9] = 0x22222222;
	m_dwCclassArray[10] = 0x22222222;
	m_dwCclassArray[11] = 0x22222222;
	m_dwCclassArray[12] = 0x22222222;
	m_dwCclassArray[13] = 0x22222222;
	m_dwCclassArray[14] = 0x22222222;
	m_dwCclassArray[15] = 0x42222222;
	m_dwCclassArray[16] = 0x66666665;
	m_dwCclassArray[17] = 0x66666666;
	m_dwCclassArray[18] = 0x66666666;
	m_dwCclassArray[19] = 0x66666666;
	m_dwCclassArray[20] = 0x66666666;
	m_dwCclassArray[21] = 0x66666666;
	m_dwCclassArray[22] = 0x66666666;
	m_dwCclassArray[23] = 0x66666666;
	m_dwCclassArray[24] = 0x66666666;
	m_dwCclassArray[25] = 0x66666666;
	m_dwCclassArray[26] = 0x66666666;
	m_dwCclassArray[27] = 0x66666666;
	m_dwCclassArray[28] = 0x66666666;
	m_dwCclassArray[29] = 0x66666666;
	m_dwCclassArray[30] = 0x66666666;
	m_dwCclassArray[31] = 0x6666666;
	m_dwStatesArray[0] = 0x13000001;
	m_dwStatesArray[1] = 0x22111111;
	m_dwStatesArray[2] = 0x1122222;
	m_dwStatesArray[3] = 0x11110014;
	m_dwStatesArray[4] = 0x12111511;
	m_dwStatesArray[5] = 17;

	m_dwSTFactor = 7;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

ECodepageType	CGB18030Verifier::Charset()
{
	return ECodepageGB18030;
}

bool CGB18030Verifier::IsUCS2()
{
	return false;
}
