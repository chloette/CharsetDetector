#include "charset_detector_common_inc.h"
#include "jchar_detector.h"

ICodepageDetector* CJCharDetector::m_pStaticInstance = NULL;

CJCharDetector::CJCharDetector()
{
	m_pStaticInstance	= NULL;
	m_pCharsetDetector	= NULL;
}

CJCharDetector::~CJCharDetector()
{
	Destroy();
	m_pStaticInstance = NULL;
}

ICodepageDetector*	CJCharDetector::GetInstance()
{
	if(NULL == m_pStaticInstance)
	{
		m_pStaticInstance = new CJCharDetector();
		if(m_pStaticInstance)
		{
			((CJCharDetector*)m_pStaticInstance)->Init();
		}
	}

	return m_pStaticInstance;
}

long	CJCharDetector::Init()
{
	long lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;
	
	m_pCharsetDetector = new CCharsetDetector();
	if(NULL == m_pCharsetDetector)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	lResult = m_pCharsetDetector->Init();
	if(lResult)
		goto EXIT;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

long	CJCharDetector::Destroy()
{
	if(m_pCharsetDetector)
	{
		m_pCharsetDetector->Destroy();

		delete m_pCharsetDetector;
		m_pCharsetDetector = NULL;
	}

	return ERR_NONE;
}

long CJCharDetector::DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType)
{
	long	lResult = ERR_UNSUPPORTED;

	if(NULL == pBuffer ||
		dwBufLength < 4 ||
		NULL == peType)
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	m_pCharsetDetector->Reset();

	//I. "��������(elimination logic)����С�������ȷ���ַ���"��http://www.renpeicheng.com/plus/view-2619-1.html��
	m_pCharsetDetector->DoIt(dwParserType, pBuffer, dwBufLength > DEFAULT_MAX_STREAM_BUFFER_SIZE ? DEFAULT_MAX_STREAM_BUFFER_SIZE : dwBufLength);
	
	m_pCharsetDetector->Done();

	//II. "����ĳ�����Ե��ַ���Ƶ��ͳ����ȷ���ַ���"��http://www.renpeicheng.com/plus/view-2619-1.html��
	if(	!m_pCharsetDetector->IsDone() && 
		m_pCharsetDetector->CanGuess() && 
		ECodepageUNKNOWN == m_pCharsetDetector->GetResultCharset())
	{
		m_pCharsetDetector->Guess();
	}

	*peType = m_pCharsetDetector->GetResultCharset();

	if(ECodepageGB18030 == *peType)
	{
		*peType = ECodepageUNKNOWN;
	}

	lResult = ERR_NONE;
EXIT:
	
	return lResult;
}

long	CJCharDetector::SetConfig(unsigned long dwCfgType, void* pData)
{
	long lResult = ERR_UNSUPPORTED;

	switch(dwCfgType)
	{
	case CODEPAGE_DETECTOR_CONFIG_ENABLE_GUESS:
		{
			if(NULL == m_pCharsetDetector)
			{
				lResult = ERR_BAD_STATE;
				break;
			}

			lResult = m_pCharsetDetector->EnableGuess((bool)pData);
		}
		break;

	default:
		{
			lResult = ERR_UNSUPPORTED;
		}
		break;
	}
	if(lResult)
		goto EXIT;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{

	}

	return lResult;
}
