#include "charset_detector_common_inc.h"
#include "charset_detector.h"

#include "utf8_verifier.h"
#include "sjis_verifier.h"
#include "euc_jp_verifier.h"
#include "iso2022_jp_verifier.h"
#include "euc_kr_verifier.h"
#include "iso2022_kr_verifier.h"
#include "big5_verifier.h"
#include "euc_tw_verifier.h"
#include "gb2312_verifier.h"
#include "gb18030_verifier.h"
#include "iso2022_cn_verifier.h"
#include "hz_verifier.h"
#include "cp1252_verifier.h"
#include "ucs2be_verifier.h"
#include "ucs2le_verifier.h"

#include "euc_jp_statistics.h"
#include "euc_kr_statistics.h"
#include "big5_statistics.h"
#include "euc_tw_statistics.h"
#include "gb2312_statistics.h"

extern const SequenceModel  Koi8rModel;
extern const SequenceModel  Win1251Model;
extern const SequenceModel  Latin2HungarianModel;
extern const SequenceModel  Win1250HungarianModel;
extern const SequenceModel  Win1252Model;
extern const SequenceModel  Latin7Model;
extern const SequenceModel  Win1253Model;
extern const SMModel           UTF8SMModel;

#define  MAX_SAMI_TOKEN_LEN 23
#define  MAX_MICRODVD_TOKEN_LEN 24
//#define  MAX_SUBVIEW_TOKEN_LEN 9
//#define  ISTIMELINE(num)   (((num) == 0x3a) || ((num) == 0x2c) || ((num) == 0x2e) || (((num) >= '0')&&((num) <= '9' )))
#define MIN_AFFECT_CHAR_COUNT 44
#define  NO_SPEED_OPTIMIZATION
static unsigned long getFromPck(unsigned char i, nsPkgInt c);
typedef enum _tag_detect_state
{
	EDetectStateOn,
	EDetectStateMe,
	EDetectStateNotMe,
	EDetectStateCount
} EDetectState;

CCharsetDetector::CCharsetDetector()
{
	memset(m_VerifierArray, 0, sizeof(ICharsetVerifier*) * MAX_CHARSET_VERIFIERS_COUNT);
	memset(m_StatisticsArray, 0, sizeof(IEUCStatistics*) * MAX_CHARSET_VERIFIERS_COUNT);

	m_pSampler = NULL;

	memset(m_State, 0, sizeof(unsigned char) * MAX_CHARSET_VERIFIERS_COUNT);
	memset(m_dwItemIdx, 0, sizeof(unsigned long) * MAX_CHARSET_VERIFIERS_COUNT);

	m_dwItems			= 0;
	m_dwClassItems		= 0;
	m_bDone				= false;
	m_bRunSampler		= false;
	m_bClassRunSampler	= false;
	m_eCodepageType		= ECodepageUNKNOWN;

	m_bCanGuess			= false;

	m_fGb2312				= 0.0;
	m_fGbEucKR				= 0.0;
	m_fBig5					= 0.0;
	m_dwBig5OnlyCount		= 0;
	m_dwCharacterCount		= 0;
	m_bSecondChar			= false;
	m_dwKRCount				= 0;
	m_dwZHJPCount			= 0;
	m_dwKRJPCount			= 0;
	m_dwJISJPCount			= 0;


	m_dwGB2312OccurrenceCharCount	= 0;
	m_dwGB2312Count	= 0;		
	m_dwEUCKROccurrenceCharCount	= 0;
	m_dwEUCKRCount	= 0;
	m_dwJISOccurrenceCharCount		= 0;
	m_dwJISCount	= 0;
	m_dwBIG5OccurrenceCharCount		= 0;
	m_dwBIG5Count	= 0;

	m_pSBKOIR_8Info	= NULL;
	m_pSBWIN_1251Info = NULL;
	m_pSBISO8859_2Info = NULL;
	m_pSBWIN_1250Info = NULL;
	m_pSBWIN_1252Info = NULL;
	m_pSBWIN_1253Info = NULL;
	m_pSBISO8859_7Info = NULL;
	m_pSMUTF_8Info = NULL;

}
CCharsetDetector::~CCharsetDetector()
{
	Destroy();
}


long	CCharsetDetector::Init()
{
	long	lResult = ERR_UNSUPPORTED;

	lResult = Destroy();
	if(lResult)
		goto EXIT;

	m_pSampler = new CEUCSampler();
	if(NULL == m_pSampler)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	lResult = m_pSampler->Init();
	if(lResult)
		goto EXIT;

	lResult = InitVerifiers();
	if(lResult)
		goto EXIT;

	m_pSBKOIR_8Info = (SingleByteInfo*)malloc(sizeof(SingleByteInfo));
	if (NULL == m_pSBKOIR_8Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	m_pSBWIN_1251Info = (SingleByteInfo*)malloc(sizeof(SingleByteInfo));
	if (NULL == m_pSBWIN_1251Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	m_pSBISO8859_2Info = (SingleByteInfo*)malloc(sizeof(SingleByteInfo));
	if (NULL == m_pSBISO8859_2Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	m_pSBWIN_1250Info = (SingleByteInfo*)malloc(sizeof(SingleByteInfo));
	if (NULL == m_pSBWIN_1250Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	m_pSBWIN_1252Info = (SingleByteInfo*)malloc(sizeof(SingleByteInfo));
	if (NULL == m_pSBWIN_1252Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	m_pSBWIN_1253Info = (SingleByteInfo*)malloc(sizeof(SingleByteInfo));
	if (NULL == m_pSBWIN_1253Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	m_pSBISO8859_7Info = (SingleByteInfo*)malloc(sizeof(SingleByteInfo));
	if (NULL == m_pSBISO8859_7Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}
	m_pSMUTF_8Info = (SMByteInfo*)malloc(sizeof(SMByteInfo));
	if (NULL == m_pSMUTF_8Info)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	Reset();

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

void CCharsetDetector::SetSingleByte(SingleByteInfo* pTempSingleByteInfo)
{
	pTempSingleByteInfo->fConfidence = 0.0f;
	pTempSingleByteInfo->dwFrqCharCount = 0;
	pTempSingleByteInfo->dwLastOrder = 0;
	pTempSingleByteInfo->dwSeqCountersMatrix[0] =0;
	pTempSingleByteInfo->dwSeqCountersMatrix[1] =0;
	pTempSingleByteInfo->dwSeqCountersMatrix[2] =0;
	pTempSingleByteInfo->dwSeqCountersMatrix[3] =0;
	pTempSingleByteInfo->dwState = 0;
	pTempSingleByteInfo->dwTotalCharacters = 0;
	pTempSingleByteInfo->dwTotalSquence = 0;
}

void CCharsetDetector::SetSMByte(SMByteInfo* pTempSMByteInfo)
{
	pTempSMByteInfo->dwCurrentBytePos = 0;
	pTempSMByteInfo->dwCurrentCharLen = 0;
	pTempSMByteInfo->dwDetectState = 0;
	pTempSMByteInfo->dwSMState = 0;//start
	pTempSMByteInfo->dwTotalMBChar = 0;
	pTempSMByteInfo->fConfidence = (float)0.0;
}

long	CCharsetDetector::Destroy()
{
	for(long lIndex = 0; lIndex < MAX_CHARSET_VERIFIERS_COUNT; ++lIndex)
	{
		if(m_VerifierArray[lIndex])
		{
			delete m_VerifierArray[lIndex];
			m_VerifierArray[lIndex] = NULL;
		}

		if(m_StatisticsArray[lIndex])
		{
			delete m_StatisticsArray[lIndex];
			m_StatisticsArray[lIndex] = NULL;
		}
	}

	if (NULL != m_pSBKOIR_8Info)
	{
		free(m_pSBKOIR_8Info);
		m_pSBKOIR_8Info = NULL;
	}
	if (NULL != m_pSBWIN_1251Info)
	{
		free(m_pSBWIN_1251Info);
		m_pSBWIN_1251Info = NULL;
	}
	if (NULL != m_pSBISO8859_2Info)
	{
		free(m_pSBISO8859_2Info);
		m_pSBISO8859_2Info = NULL;
	}
	if (NULL != m_pSBWIN_1250Info)
	{
		free(m_pSBWIN_1250Info);
		m_pSBWIN_1250Info = NULL;
	}
	if (NULL != m_pSBWIN_1252Info)
	{
		free(m_pSBWIN_1252Info);
		m_pSBWIN_1252Info = NULL;
	}
	if (NULL != m_pSBWIN_1253Info)
	{
		free(m_pSBWIN_1253Info);
		m_pSBWIN_1253Info = NULL;
	}
	if (NULL != m_pSBISO8859_7Info)
	{
		free(m_pSBISO8859_7Info);
		m_pSBISO8859_7Info = NULL;
	}
	if (NULL != m_pSMUTF_8Info)
	{
		free(m_pSMUTF_8Info);
		m_pSMUTF_8Info = NULL;
	}

	if(m_pSampler)
	{
		delete m_pSampler;
		m_pSampler = NULL;
	}

	return ERR_NONE;
}

void CCharsetDetector::Reset()
{
	m_bRunSampler = m_bClassRunSampler;
	m_bDone = false;
	m_dwItems = m_dwClassItems;
	for (long i = 0; i < m_dwItems; i++)
	{
		m_State[i] = 0;
		m_dwItemIdx[i] = i;
	}

	m_pSampler->Reset();

	SetSingleByte(m_pSBKOIR_8Info);
	SetSingleByte(m_pSBWIN_1251Info);
	SetSingleByte(m_pSBISO8859_2Info);
	SetSingleByte(m_pSBWIN_1250Info);
	SetSingleByte(m_pSBWIN_1252Info);
	SetSingleByte(m_pSBWIN_1253Info);
	SetSingleByte(m_pSBISO8859_7Info);

	m_pSBWIN_1252Info->dwLastOrder = OTH;
	m_dwWINvsISO8859Count = 0;

	SetSMByte(m_pSMUTF_8Info);

	m_eCodepageType = ECodepageUNKNOWN;
}

unsigned long CCharsetDetector::IsSamiNoise(char* pBuf, unsigned long dwCurrentBuf, unsigned long dwLength)
{
	unsigned long       dwTokenLen = 0;
	char*        pszStart = NULL;
	char*        pszTokenStart = NULL;
	char*        pszTokenEnd   = NULL;

	if (dwLength < dwCurrentBuf || !pBuf)
	{
		return 0;
	}

	if ('<' != pBuf[dwCurrentBuf])
		return 0;

	pszStart = &pBuf[dwCurrentBuf];
	//find first '<'
	pszTokenStart = strchr(pszStart, '<');
	if (NULL == pszTokenStart)
		return 0;

	//fine next '>'
	pszTokenEnd = strchr(pszTokenStart, '>');
	if (NULL == pszTokenEnd)
		return 0;

	dwTokenLen = pszTokenEnd - pszTokenStart;

	if (dwTokenLen > MAX_SAMI_TOKEN_LEN)
	{
		//<!-- ***** --> is a long token.
		if ((pszTokenStart[1] == '!') && (pszTokenStart[2] == '-') && (pszTokenStart[3] == '-') && (pszTokenStart[dwTokenLen - 1] == '-') && (pszTokenStart[dwTokenLen - 2] == '-'))
			return dwTokenLen;
		else
			return 0;
	}
	else
		return dwTokenLen;
}

unsigned long CCharsetDetector::IsMicroDvdNoise(char* pBuf, unsigned long dwCurrentBuf, unsigned long dwLength)
{
	unsigned long       dwTokenLen = 0;
	char*        pszStart = NULL;
	char*        pszTokenStart = NULL;
	char*        pszTokenEnd   = NULL;

	if (dwLength < dwCurrentBuf || !pBuf)
	{
		return 0;
	}

	if ('[' != pBuf[dwCurrentBuf])
		return 0;

	pszStart = &pBuf[dwCurrentBuf];
	//find first '['
	pszTokenStart = strchr(pszStart, '[');
	if (NULL == pszTokenStart)
		return 0;

	//fine next ']'
	pszTokenEnd = strchr(pszTokenStart, ']');
	if (NULL == pszTokenEnd)
		return 0;

	dwTokenLen = pszTokenEnd - pszTokenStart;

	if (dwTokenLen > MAX_MICRODVD_TOKEN_LEN)
		return 0;
	else
		return dwTokenLen;
}

bool CCharsetDetector::DoIt(unsigned long dwParserType, char* pBuf, unsigned long dwLength)
{
	//I. "��������(elimination logic)����С�������ȷ���ַ���"��http://www.renpeicheng.com/plus/view-2619-1.html��
	//float fTemp = 0.0;
	unsigned long	dwTempIdx	        = 0;
	unsigned long	dwTempOrder	    = 0;
	unsigned long dwSamiNoiseLen   = 0;
	unsigned long dwMicroDvdLen    = 0;
	unsigned long dwSubViewLen     = 0;
	unsigned long dwTimeLineNoiseLen = 0;
	unsigned long	dwTempPostion	= 0;
	bool	    bJISSecondChar	= true;

	Reset();
	if (IsTextUTF8(pBuf, dwLength))
	{
		m_pSMUTF_8Info->dwDetectState = EDetectStateMe;
		m_bDone =  true;
		goto EXIT;
	}
	for (long i = 0; i < dwLength; i++)
	{
		char b = pBuf[i];
		char c = 0x00;
		

		if (5 == dwParserType)// 5 means EParserTypeSmi.
		{
			dwSamiNoiseLen = IsSamiNoise(pBuf, i, dwLength);

			if (dwSamiNoiseLen > 0)
			{
				i += dwSamiNoiseLen - 1;
				continue;
			}
		}
		
		dwMicroDvdLen = IsMicroDvdNoise(pBuf, i, dwLength);
		if (dwMicroDvdLen > 0)
		{
			i += dwMicroDvdLen;
			continue;
		}
		
		//dwTimeLineNoiseLen = ISTimeLineNoise(pBuf, i, dwLength);
		//if (dwTimeLineNoiseLen > 0)
		//{
		//	i+=dwTimeLineNoiseLen - 1;
		//	continue;
		//}

		if (dwLength == (i - 1))
		{
			c = 0x00;
		}
		else
		{
			c = pBuf[i + 1];
		}

		if ((b&0xff) > 0xA0)
		{
			if (i > (dwTempIdx + 1))
			{
				m_bSecondChar = false;
			}
			if (true == m_bSecondChar)
			{
				m_fGb2312	+= m_StatisticsArray[8]->SecondByteFreq()[(unsigned long)((b&0xff) - 0xA0)] * 0.5;
				m_fGbEucKR	+= m_StatisticsArray[4]->SecondByteFreq()[(unsigned long)((b&0xff) - 0xA0)] * 0.5;
				m_fBig5		+= m_StatisticsArray[6]->SecondByteFreq()[(unsigned long)((b&0xff) - 0xA0)] * 0.5;


				//**************************************************************************************
				//	simplified chinese.
				//  first  byte range: 0xb0 -- 0xfe
				//  second byte range: 0xa1 -- 0xfe
				if (i > 0)
				{ 
					if ((unsigned long)(pBuf[i-1]&0xff) >= 0xb0 && (unsigned long)(b&0xff) >= 0xa1)
					{
						m_dwGB2312Count++;
						dwTempOrder = 94*((unsigned long)(pBuf[i-1]&0xff)-(unsigned long)0xb0) + (unsigned long)(b&0xff)- (unsigned long)0xa1;
					}
					else
						dwTempOrder =  -1;
				}
				if (dwTempOrder < GB2312_TABLE_SIZE)
				{
					if (GB2312CharToFreqOrder[dwTempOrder] < 1024)
					{
						m_dwGB2312OccurrenceCharCount++;
					}
				}

				//**************************************************************************************
				//	big5
				//  first  byte range: 0xa4 -- 0xfe
				//  second byte range: 0x40 -- 0x7e , 0xa1 -- 0xfe

				if (i > 0)
				{ 
					if ((unsigned long)(pBuf[i-1]&0xff) >= 0xa4 && (unsigned long)(b&0xff) >= 0xa1)
					{
						m_dwBIG5Count++;
						dwTempOrder = 157 * ((unsigned long)(pBuf[i-1]&0xff)-(unsigned long)0xa4) + (unsigned long)(b&0xff)- (unsigned long)0xa1 + 63;
					}
					else
						dwTempOrder =  -1;
				}
				if (dwTempOrder < BIG5_TABLE_SIZE)
				{
					if (Big5CharToFreqOrder[dwTempOrder] < 1024)
					{
						m_dwBIG5OccurrenceCharCount++;
					}
				}

				//**************************************************************************************
				//	EUC-KR
				//  first  byte range: 0xb0 -- 0xfe
				//  second byte range: 0xa1 -- 0xfe


				if (i > 0)
				{ 
					if ((unsigned long)(pBuf[i-1]&0xff) >= 0xb0 && (unsigned long)(b&0xff) >= 0xa1)
					{
						m_dwEUCKRCount++;
						dwTempOrder = 94 * ((unsigned long)(pBuf[i-1]&0xff)-(unsigned long)0xb0) + (unsigned long)(b&0xff)- (unsigned long)0xa1;
					}
					else
						dwTempOrder =  -1;
				}
				if (dwTempOrder < EUCKR_TABLE_SIZE)
				{
					if (EUCKRCharToFreqOrder[dwTempOrder] < 1024)
					{
						m_dwEUCKROccurrenceCharCount++;
					}
				}
				//**************************************************************************************


				m_dwCharacterCount++;
				m_bSecondChar = false;
			}
			else
			{
				m_fGb2312	+= m_StatisticsArray[8]->SecondByteFreq()[(unsigned long)((b&0xff) - 0xA0)] * 0.5;
				m_fGbEucKR	+= m_StatisticsArray[4]->SecondByteFreq()[(unsigned long)((b&0xff) - 0xA0)] * 0.5;
				m_fBig5		+= m_StatisticsArray[6]->SecondByteFreq()[(unsigned long)((b&0xff) - 0xA0)] * 0.5;

				if (((b&0xff) > 0xb0 && (b&0xff) < 0xca) && (c&0xff) > 0xa0)   //most korean character  16~40(+ A0) qzone 
				{
					m_dwKRCount++;
				}
				else if ((0xa4 == (b&0xff) || 0xa5 == (b&0xff)) && (c&0xff) > 0xa0)//chinese and euc-jp zone for kana
				{
					m_dwZHJPCount++;
				}
				else if ((0xaa == (b&0xff) || 0xab == (b&0xff)) && (c&0xff) > 0xa0)//korean zone for kana
				{
					m_dwKRJPCount++;
				}
				m_bSecondChar = true;
				dwTempIdx	= i;
			}
		}
		else if ((b&0xff) >= 0x40 && true == m_bSecondChar)
		{

			//**************************************************************************************
			//	big5
			//  first  byte range: 0xa4 -- 0xfe
			//  second byte range: 0x40 -- 0x7e , 0xa1 -- 0xfe

			if (i > 0)
			{ 
				if ((unsigned long)(pBuf[i-1]&0xff) >= 0xa4 && (unsigned long)(b&0xff) >= 0x40)
				{
					m_dwBIG5Count++;
					dwTempOrder = 157 * ((unsigned long)(pBuf[i-1]&0xff)-(unsigned long)0xa4) + (unsigned long)(b&0xff)- (unsigned long)0x40;
				}
				else
					dwTempOrder =  -1;
			}
			if (dwTempOrder < BIG5_TABLE_SIZE)
			{
				if (Big5CharToFreqOrder[dwTempOrder] < 1024)
				{
					m_dwBIG5OccurrenceCharCount++;
				}
			}
			//**************************************************************************************
			m_dwBig5OnlyCount++;
			m_bSecondChar = false;
		}
		else if (((b&0xff) == 0x82 &&(c&0xff) <=0xf1 && (c&0xff) >=0x9f) || ((b&0xff) == 0x83 &&(c&0xff) <=0x96 && (c&0xff) >=0x40))
		{
			m_dwJISJPCount++;
		}

		//************************************************************************************************************
		//for sjis encoding
		//  first  byte range: 0x81 -- 0x9f , 0xe0 -- 0xfe
		//  second byte range: 0x40 -- 0x7e,  0x81 -- oxfe

		if (i - dwTempPostion > 1)
		{
			bJISSecondChar	= true;
		}						
		if ((((b&0xff) >= 0x81 &&(b&0xff) <=0x9f) || ((b&0xff) >= 0xe0 &&(b&0xff) <=0xfe)) 
			&& (((c&0xff) >= 0x40 &&(c&0xff) <=0x7e) || ((c&0xff) >= 0x81 &&(c&0xff) <=0xfe))
			&& true == bJISSecondChar)
		{
			m_dwJISCount++;
			bJISSecondChar	= false;
			dwTempPostion	= i;
			if (((b&0xff) >= 0x81 &&(b&0xff) <=0x9f))
			{
				dwTempOrder = 188 * ((unsigned long)(b&0xff) - (unsigned long)0x81) +(unsigned long)(c&0xff) - 0x40;
			}
			else if (((b&0xff) >= 0xe0 &&(b&0xff) <=0xfe))
			{
				dwTempOrder = 188 * ((unsigned long)(b&0xff) - (unsigned long)0xe0 + 31) +(unsigned long)(c&0xff) - 0x40;
			}
			else
			{
				dwTempOrder =  -1;
			}
			if (dwTempOrder < JIS_TABLE_SIZE)
			{
				if (JISCharToFreqOrder[dwTempOrder] < 1024)
				{
					m_dwJISOccurrenceCharCount++;
				}
			}
		}



		/******************************************************
		/#The emphasis of following charset detector algorithm is <<CONFIDENCE>>.
		/#Confidence is a float value, if one's confidence is more than 0.65(found the right charset).
		/#Set the EDetectStateMe, and get out this function.
		******************************************************/

		//************************************************************************************************************
		/*for windows-1252 encoding*/
		if(!m_pSBWIN_1252Info->dwState)
			m_pSBWIN_1252Info->dwState = DetectWIN1252Codepage(&Win1252Model, pBuf[i], m_pSBWIN_1252Info);
		if (EDetectStateMe == m_pSBWIN_1252Info->dwState)
		{
			break;
		}
		//************************************************************************************************************
		//for koir-8 encoding
		//if character is not a symbol or punctuation character
		if (EDetectStateOn == m_pSBKOIR_8Info->dwState)
			m_pSBKOIR_8Info->dwState = DetectSingleByteCodepage(&Koi8rModel, pBuf[i], m_pSBKOIR_8Info);
		if (EDetectStateMe == m_pSBKOIR_8Info->dwState)
		{
			break;
		}
		//************************************************************************************************************
		//for win-1251 encoding
		if(!m_pSBWIN_1251Info->dwState)
			m_pSBWIN_1251Info->dwState = DetectSingleByteCodepage(&Win1251Model, pBuf[i], m_pSBWIN_1251Info);
		if (EDetectStateMe == m_pSBWIN_1251Info->dwState)
		{
			break;
		}
		//************************************************************************************************************
		/*for iso-1859-2 encoding*/
		if(!m_pSBISO8859_2Info->dwState)
			m_pSBISO8859_2Info->dwState = DetectSingleByteCodepage(&Latin2HungarianModel, pBuf[i], m_pSBISO8859_2Info);

		//************************************************************************************************************
		/*for win-1250 encoding*/
		if(!m_pSBWIN_1250Info->dwState)
			m_pSBWIN_1250Info->dwState = DetectSingleByteCodepage(&Win1250HungarianModel, pBuf[i], m_pSBWIN_1250Info);


		/*windows-1250 is very likely to iso-8859-2*/
		if (EDetectStateMe == m_pSBWIN_1250Info->dwState || EDetectStateMe == m_pSBISO8859_2Info->dwState)
		{
			break;
		}

		//************************************************************************************************************
		//for win-1253 encoding
		if(!m_pSBWIN_1253Info->dwState)
			m_pSBWIN_1253Info->dwState = DetectSingleByteCodepage(&Win1253Model, pBuf[i], m_pSBWIN_1253Info);

		//************************************************************************************************************
		//for iso-8859-7 encoding
		if(!m_pSBISO8859_7Info->dwState)
			m_pSBISO8859_7Info->dwState = DetectSingleByteCodepage(&Latin7Model, pBuf[i], m_pSBISO8859_7Info);

		/*windwos-1253 is very likely to iso-8859-7*/
		if (EDetectStateMe == m_pSBISO8859_7Info->dwState || EDetectStateMe == m_pSBWIN_1253Info->dwState)
		{
			break;
		}

		if (!m_pSMUTF_8Info->dwDetectState)
			m_pSMUTF_8Info->dwDetectState = DetectCodingStateMachine(&UTF8SMModel, pBuf[i], m_pSMUTF_8Info);
		
		/*************************************************************************************************************/
		/*====START filter charset.*/
		/*for almost the same win-1250 and iso-1859-2:
		*Because iso-8859-2 may not use characters between 128 with 159, so I count it.*/
		if ( (unsigned char)pBuf[i]>= 128 && (unsigned char)pBuf[i] <= 159)
		{
			m_dwWINvsISO8859Count++;
		}
		/*====END of filter charset.*/

		for (long j = 0; j < m_dwItems; )
		{
			unsigned char st = ICharsetVerifier::getNextState(m_VerifierArray[m_dwItemIdx[j]], b,
				m_State[j]);
			if (st == 2)
			{
				//Bingo to detector the charset.
				if (ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[j]]->Charset() && i < 2000)
				{
					j++;
					continue;
				}
				m_eCodepageType = m_VerifierArray[m_dwItemIdx[j]]->Charset();
				m_bDone = true;
				goto EXIT;
			}
			if (st == 1)
			{
				//filter				
				if (j < m_dwItems)
				{
					m_dwItems--;
					m_dwItemIdx[j] = m_dwItemIdx[m_dwItems];
					m_State[j] = m_State[m_dwItems];
				}
			}
			else
			{
				m_State[j++] = st;
			}
		}

		if (m_dwItems <= 1)
		{
			if ((i <= dwLength - 1) && i < 2000) 
			{
				continue;
			}
			if (1 == m_dwItems)
			{
				m_eCodepageType = m_VerifierArray[m_dwItemIdx[0]]->Charset();
			}
			m_bDone = true;
			goto EXIT;
		}
		long nonUCS2Num = 0;
		long nonUCS2Idx = 0;
		for (long j = 0; j < m_dwItems; j++)
		{
			if (!m_VerifierArray[m_dwItemIdx[j]]->IsUCS2()
				&& !m_VerifierArray[m_dwItemIdx[j]]->IsUCS2())
			{
				nonUCS2Num++;
				nonUCS2Idx = j;
			}
		}

		if (1 == nonUCS2Num)
		{
			if ((i <= dwLength - 1) && i < 2000)
			{
				continue;
			}
			m_eCodepageType = m_VerifierArray[m_dwItemIdx[nonUCS2Idx]]->Charset();
			m_bDone = true;
			goto EXIT;
		}
	}
	//m_bRunSampler = true;
	if (m_bRunSampler)
	{
		Sample(pBuf, dwLength);
		//	m_bRunSampler = false;
	}
EXIT:

	m_bSecondChar = false;
	return m_bDone;
}

ECodepageType	CCharsetDetector::GetResultCharset()
{
	return m_eCodepageType;
}

bool CCharsetDetector::IsTextUTF8( char* lpstrInputStream, unsigned long iLen )
{
	unsigned long   i;
	unsigned long cOctets;  // octets to go in this UTF-8 encoded character
	unsigned char chr;
	bool  bAllAscii= true;

	cOctets= 0;
	for( i=0; i < iLen; i++ ) {
		chr = *(lpstrInputStream+i);
		if( (chr&0x80) != 0 ) bAllAscii= false;

		if( cOctets == 0 )  {
			//
			// 7 bit ascii after 7 bit ascii is just fine.  Handle start of encoding case.
			//
			if( chr >= 0x80 ) {  
				//
				// count of the leading 1 bits is the number of characters encoded
				//
				do {
					chr <<= 1;
					cOctets++;
				}
				while( (chr&0x80) != 0 );

				cOctets--;                        // count includes this character
				if( cOctets == 0 )
				{
					return false;  // must start with 11xxxxxx
				}
				
			}
		}
		else {
			// non-leading bytes must start as 10xxxxxx
			if( (chr&0xC0) != 0x80 ) {
				return false;
			}
			cOctets--;                           // processed another octet in encoding
		}
	}

	//
	// End of text.  Check for consistency.
	//

	if( cOctets > 0 ) {   // anything left over at the end is an error
		return false;
	}

	if( bAllAscii ) {     // Not utf-8 if all ascii.  Forces caller to use code pages for conversion
		return false;
	}
	return true;
}

bool	CCharsetDetector::CanGuess()
{
	return m_bCanGuess;
}

long	CCharsetDetector::EnableGuess(bool bCanGuess)
{
	m_bCanGuess = bCanGuess;

	return ERR_NONE;
}

unsigned long CCharsetDetector::DetectSingleByteCodepage(const SequenceModel*const pSquModel, unsigned char Uint8Current,SingleByteInfo* pTemSingleByteInfo)//Integer multiple of 100
{
	unsigned char Uint8Order = 0;

	bool bReversed = true;

	Uint8Order = pSquModel->charToOrderMap[Uint8Current];
	//if ((Uint8Current < 49) || (Uint8Current > 57) && (Uint8Current != 62) && (Uint8Current != 45) && (Uint8Current != 58) && (Uint8Current != 44) && (Uint8Current != 46))  //Skip numbers.
	//if (Uint8Current > 65)
	{
		if (Uint8Order < SYMBOL_CAT_ORDER)
			pTemSingleByteInfo->dwTotalCharacters++;
		if (Uint8Order < SAMPLE_SIZE)
		{
			pTemSingleByteInfo->dwFrqCharCount++;

			if (pTemSingleByteInfo->dwLastOrder < SAMPLE_SIZE)
			{
				pTemSingleByteInfo->dwTotalSquence++;
				if (!bReversed)
					++(pTemSingleByteInfo->dwSeqCountersMatrix[pSquModel->precedenceMatrix[pTemSingleByteInfo->dwLastOrder*SAMPLE_SIZE+Uint8Order]]);
				else // reverse the order of the letters in the lookup
					++(pTemSingleByteInfo->dwSeqCountersMatrix[pSquModel->precedenceMatrix[Uint8Order*SAMPLE_SIZE+pTemSingleByteInfo->dwLastOrder]]);
			}
		}
		pTemSingleByteInfo->dwLastOrder = Uint8Order;


		if (pTemSingleByteInfo->dwTotalSquence >= SB_DIVISIBLE_UNIT  && 0 == (pTemSingleByteInfo->dwTotalSquence % SB_DIVISIBLE_UNIT))//Integer multiple of 100
		{
			pTemSingleByteInfo->fConfidence = GetSingleByteConfidence(pSquModel, pTemSingleByteInfo->dwSeqCountersMatrix, pTemSingleByteInfo->dwTotalCharacters, pTemSingleByteInfo->dwFrqCharCount, pTemSingleByteInfo->dwTotalSquence);
			if (pTemSingleByteInfo->fConfidence > POSITIVE_SHORTCUT_THRESHOLD)
			{
#ifdef NO_SPEED_OPTIMIZATION
				return EDetectStateOn;//no speed optimization
#else
				return EDetectStateMe;//found it
#endif
			}
			else if (pTemSingleByteInfo->dwTotalSquence  > SB_ENOUGH_REL_THRESHOLD && pTemSingleByteInfo->fConfidence < NEGATIVE_SHORTCUT_THRESHOLD)
			{
				return EDetectStateNotMe;//not it
			}
		}
	}

	return EDetectStateOn;//go on detecting
}

float  CCharsetDetector::GetSingleByteConfidence(const SequenceModel* const pSquModel,unsigned long dwSeqCountersMatrix[], unsigned long dwTotalChar, unsigned long dwFreqChar, unsigned long dwTotalSeqs)
{
#ifdef NEGATIVE_APPROACH
	if (dwTotalSeqs > 0)
		if (dwTotalSeqs > dwSeqCountersMatrix[NEGATIVE_CAT]*10 )
			return ((float)(dwTotalSeqs - dwSeqCountersMatrix[NEGATIVE_CAT]*10))/dwTotalSeqs * dwFreqChar / dwTotalChar;
	return (float)0.01;
#else  //POSITIVE_APPROACH
	float fConfidence  =  0;

	if (dwTotalSeqs > 0) {
		fConfidence = ((float)1.0) * dwSeqCountersMatrix[POSITIVE_CAT] / dwTotalSeqs / pSquModel->mTypicalPositiveRatio;
		fConfidence = fConfidence*dwFreqChar/dwTotalChar;
		if (fConfidence >= (float)1.00)
			fConfidence = (float)0.99;
		return fConfidence;
	}
	return (float)0.01;
#endif
}

//============WIN1252 dectector is different from SingleByte==============//
unsigned long CCharsetDetector::DetectWIN1252Codepage(const SequenceModel*const pSquModel, unsigned char Uint8Current, SingleByteInfo* pTemSingleByteInfo)
{
	unsigned char Uint8Order = 0;
	unsigned char Uint8Freq = 0;

	Uint8Order = pSquModel->charToOrderMap[Uint8Current];

	Uint8Freq = pSquModel->precedenceMatrix[pTemSingleByteInfo->dwLastOrder*CLASS_NUM + Uint8Order];
	if (0 == Uint8Freq ) {
		return EDetectStateOn;//for test
		//return EDetectStateNotMe;
	}
	else
	{
		pTemSingleByteInfo->dwSeqCountersMatrix[Uint8Freq]++;
		pTemSingleByteInfo->dwLastOrder = Uint8Order;
		pTemSingleByteInfo->dwTotalSquence++;
	}

	//100,200,300.... Calculate the Confidence.
	if (pTemSingleByteInfo->dwTotalSquence >= SB_DIVISIBLE_UNIT  && 0 == (pTemSingleByteInfo->dwTotalSquence % SB_DIVISIBLE_UNIT))
	{
		if(!pTemSingleByteInfo->dwTotalSquence)
			pTemSingleByteInfo->fConfidence = 0.0f;
		else
		{
			pTemSingleByteInfo->fConfidence = pTemSingleByteInfo->dwSeqCountersMatrix[3]*1.0f / pTemSingleByteInfo->dwTotalSquence;
			pTemSingleByteInfo->fConfidence -= pTemSingleByteInfo->dwSeqCountersMatrix[1]*20.0f / pTemSingleByteInfo->dwTotalSquence;
		}

		if (pTemSingleByteInfo->fConfidence < 0.0f)
			pTemSingleByteInfo->fConfidence = 0.0f;
		/*Cp1252 Support so much language, fConfidence is a referece value.*/
		else if (pTemSingleByteInfo->dwTotalSquence  > SB_ENOUGH_REL_THRESHOLD && pTemSingleByteInfo->fConfidence < NEGATIVE_SHORTCUT_THRESHOLD)
		{
			return EDetectStateNotMe;//not it
		}
	}
	return EDetectStateOn;//go on detecting.
}

#define ONE_CHAR_PROB   (float)0.50
unsigned long CCharsetDetector::DetectCodingStateMachine(const SMModel*const pSMModel, unsigned char Uint8Current,SMByteInfo* pTemSMByteInfo)//Integer multiple of 100
{
	ESMState  codingState = eStart;
	float      funlike = (float)0.0;
	if (Uint8Current == 13 || Uint8Current == 10 || (Uint8Current >= 44  && Uint8Current <= 64) || Uint8Current ==175 || Uint8Current == 190 || Uint8Current == 142
		|| Uint8Current == 205 || Uint8Current == 145 || Uint8Current == 148 || Uint8Current == 131 || Uint8Current == 188 || Uint8Current == 155 )
	{
		return EDetectStateOn;
	}
	codingState = (ESMState)NextStateMachineState(pSMModel, Uint8Current, pTemSMByteInfo);
	if (codingState == eError)
	{
		return pTemSMByteInfo->dwDetectState = EDetectStateNotMe;
	}
	if (codingState == eItsMe)
	{
		return pTemSMByteInfo->dwDetectState = EDetectStateMe;
	}
	if (codingState == eStart)
	{
		if (pTemSMByteInfo->dwCurrentCharLen >= 2)
			pTemSMByteInfo->dwTotalMBChar++;
	}

	//if (pTemSMByteInfo->dwTotalMBChar >= SB_DIVISIBLE_UNIT && 0 == (pTemSMByteInfo->dwTotalMBChar % SB_DIVISIBLE_UNIT))
	//{
		if (pTemSMByteInfo->dwDetectState == EDetectStateOn)
		{
			//Caculate confidence
			funlike = (float)0.99;
			if (pTemSMByteInfo->dwTotalMBChar < 6)
			{
				for (unsigned long i = 0; i < pTemSMByteInfo->dwTotalMBChar; i++)
					funlike *= ONE_CHAR_PROB;
				pTemSMByteInfo->fConfidence =  (float)1.0 - funlike;
			}

			if (pTemSMByteInfo->fConfidence > POSITIVE_SHORTCUT_THRESHOLD)
				pTemSMByteInfo->dwDetectState = EDetectStateMe;//found it.
		}
	/*}*/
	return pTemSMByteInfo->dwDetectState;
}

//UTF8SMModel;

#define GETFROMPCK(i, c) \
	(((((c).data)[(i)>>(c).idxsft])>>(((i)&(c).sftmsk)<<(c).bitsft))&(c).unitmsk)

unsigned long CCharsetDetector::NextStateMachineState(const SMModel* const pSMModel, unsigned char Uint8Current, SMByteInfo* pTemSMByteInfo)
{
	//for each byte we get its class , if it is first byte, we also get byte length
	unsigned char byteCls = GETFROMPCK((Uint8Current), pSMModel->classTable);
	unsigned char byteCls2 = getFromPck((Uint8Current), pSMModel->classTable); // fot test.
	if (pTemSMByteInfo->dwSMState == eStart)
	{ 
		pTemSMByteInfo->dwCurrentBytePos = 0; 
		pTemSMByteInfo->dwCurrentCharLen = pSMModel->charLenTable[byteCls];
	}
	//from byte's class and stateTable, we get its next state
	pTemSMByteInfo->dwSMState= (ESMState)GETFROMPCK((pTemSMByteInfo->dwSMState*(pSMModel->classFactor) + byteCls), pSMModel->stateTable);
	pTemSMByteInfo->dwCurrentBytePos++;
	return pTemSMByteInfo->dwSMState;
}

static unsigned long getFromPck(unsigned char i, nsPkgInt c)
{ 
	unsigned long  v0 = 0;
	unsigned long  v1 = 0;
	unsigned long  v2 = 0;
	unsigned long  v3 = 0;
	unsigned long  v4 = 0;
	unsigned long  v5 = 0;
	
	v0 = i >> c.idxsft;//idxsft = 3,
	v1 = c.data[v0]; 
	v2 = i & c.sftmsk;//7
	v3 = v2 << c.bitsft;//2
	v4 = v1 >> v3;
	v5 = v4 & c.unitmsk;
	return v5;
}

long CCharsetDetector::InitVerifiers()
{
	long lResult = ERR_UNSUPPORTED;

	m_VerifierArray[0] = new CUTF8Verifier();
	m_VerifierArray[1] = new CSJISVerifier();
	m_VerifierArray[2] = new CEUCJPVerifier();
	m_VerifierArray[3] = new CISO2022JPVerifier();
	m_VerifierArray[4] = new CEUCKRVerifier();
	m_VerifierArray[5] = new CISO2022KRVerifier();
	m_VerifierArray[6] = new CBig5Verifier();
	m_VerifierArray[7] = new CEUCTWVerifier();
	m_VerifierArray[8] = new CGB2312Verifier();
	m_VerifierArray[9] = new CGB18030Verifier();
	m_VerifierArray[10] = new CISO2022CNVerifier();
	m_VerifierArray[11] = new CHZVerifier();
	m_VerifierArray[12] = new CCP1252Verifier();
	m_VerifierArray[13] = new CUCS2BEVerifier();
	m_VerifierArray[14] = new CUCS2LEVerifier();
	if(	NULL == m_VerifierArray[0] ||
		NULL == m_VerifierArray[1] ||
		NULL == m_VerifierArray[2] ||
		NULL == m_VerifierArray[3] ||
		NULL == m_VerifierArray[4] ||
		NULL == m_VerifierArray[5] ||
		NULL == m_VerifierArray[6] ||
		NULL == m_VerifierArray[7] ||
		NULL == m_VerifierArray[8] ||
		NULL == m_VerifierArray[9] ||
		NULL == m_VerifierArray[10] ||
		NULL == m_VerifierArray[11] ||
		NULL == m_VerifierArray[12] ||
		NULL == m_VerifierArray[13] ||
		NULL == m_VerifierArray[14])
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	for(long lIndex = 0; lIndex < 15; ++lIndex)
	{
		lResult = m_VerifierArray[lIndex]->Init();
		if(lResult)
			goto EXIT;
	}

	m_StatisticsArray[2] = new CEUCJPStatistics();
	m_StatisticsArray[4] = new CEUCKRStatistics();
	m_StatisticsArray[6] = new CBIG5Statistics();
	m_StatisticsArray[7] = new CEUCTWStatistics();
	m_StatisticsArray[8] = new CGB2312Statistics();
	m_StatisticsArray[9] = new CGB2312Statistics();
	if(	NULL == m_StatisticsArray[2] ||
		NULL == m_StatisticsArray[4] ||
		NULL == m_StatisticsArray[6] ||
		NULL == m_StatisticsArray[7] ||
		NULL == m_StatisticsArray[8] ||
		NULL == m_StatisticsArray[9]
	)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	m_bClassRunSampler = (m_StatisticsArray != NULL);
	m_dwClassItems = 15;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		Destroy();
	}

	return lResult;
}

void CCharsetDetector::Sample(char* pBuf, unsigned long dwLength)
{
	Sample(pBuf, dwLength, false);
}

void CCharsetDetector::Sample(char* pBuf, unsigned long dwLength, bool bLastChance)
{
	IEUCStatistics* pTempStatistics = NULL;
	ICharsetVerifier*	pTempVerifier = NULL;

	long possibleCandidateNum = 0;
	long eucNum = 0;
	ECodepageType CodepageType = ECodepageUNKNOWN;
	for (long j = 0; j < m_dwItems; j++)
	{
		if(m_StatisticsArray[m_dwItemIdx[j]] != NULL)
		{
			eucNum++;
		}

		pTempVerifier = m_VerifierArray[m_dwItemIdx[j]];

		if(	!pTempVerifier->IsUCS2())
			//&& ECodepageGB18030 != pTempVerifier->Charset())
		{
			possibleCandidateNum++;
		}
	}

	m_bRunSampler = eucNum > 1;
	if (m_bRunSampler)
	{
		m_bRunSampler = m_pSampler->Sample(pBuf, dwLength);
		if ((bLastChance && m_pSampler->GetSomeData() || m_pSampler->EnoughData())
			&& eucNum == possibleCandidateNum)
		{
			m_pSampler->CalFreq();
			long bestIdx = -1;
			long eucCnt = 0;
			float bestScore = 0.0F;
			for (long j = 0; j < m_dwItems; j++)
			{
				pTempStatistics = m_StatisticsArray[m_dwItemIdx[j]];
				pTempVerifier = m_VerifierArray[m_dwItemIdx[j]];
				if(	pTempStatistics && 
					ECodepageBIG5 != (CodepageType = pTempVerifier->Charset()))
				{
					float score = m_pSampler->GetScore(pTempStatistics->FirstByteFreq(),
						pTempStatistics->FirstByteWeight(), 
						pTempStatistics->SecondByteFreq(), 
						pTempStatistics->SecondByteWeight());

					if (eucCnt++ == 0 || score > bestScore)
					{
						bestScore = score;
						bestIdx = j;
					}
				}
			}

			if (bestIdx >= 0)
			{
				m_eCodepageType = m_VerifierArray[m_dwItemIdx[bestIdx]]->Charset();
				//m_bDone = true;
				//if (ECodepageGB2312 == m_eCodepageType)
				//{
				//	m_bDone = true;
				//}
			}
		}
	}
}

bool	CCharsetDetector::IsDone()
{
	return m_bDone;
}

void	CCharsetDetector::Done()
{
	double dFreq		= 0.0;
	double dFreqKR		= 0.0;
	double	dFreqJP		= 0.0;
	double dFreqJIS	= 0.0;

	double	dFreqSC		= 0.0;
	double	dFreqEUCKR	= 0.0;
	double	dFreqBIG5	= 0.0;
	double	dFreqSJIS	= 0.0;
	ECodepageType eTempCodepageType = ECodepageUNKNOWN;
	double dMaxFreq	= 0.0;
	unsigned long dwTotalccurrenceCharCount = 0;

	if (0 != (m_dwBig5OnlyCount + m_dwCharacterCount))
	{
		dFreq = (double)(m_dwBig5OnlyCount)/(double)(m_dwBig5OnlyCount + m_dwCharacterCount);
	}

	//********************************************************************************************************
	if (0 != m_dwGB2312Count)
	{
		dFreqSC = (double)(m_dwGB2312OccurrenceCharCount)/(double)(m_dwGB2312Count);
	}
	if (0 != m_dwEUCKRCount)
	{
		dFreqEUCKR = (double)(m_dwEUCKROccurrenceCharCount)/(double)(m_dwEUCKRCount);
	}
	if (0 != m_dwBIG5Count)
	{
		dFreqBIG5 = (double)(m_dwBIG5OccurrenceCharCount)/(double)(m_dwBIG5Count);
	}
	if (0 != m_dwJISCount)
	{
		dFreqSJIS = (double)(m_dwJISOccurrenceCharCount)/(double)(m_dwJISCount);
	}
	//********************************************************************************************************
	dwTotalccurrenceCharCount = m_dwGB2312OccurrenceCharCount + m_dwEUCKROccurrenceCharCount + m_dwBIG5OccurrenceCharCount +m_dwJISOccurrenceCharCount;

	if ((m_dwGB2312Count > MIN_AFFECT_CHAR_COUNT || ((double)m_dwGB2312OccurrenceCharCount / (double)dwTotalccurrenceCharCount) > 0.64) && dFreqSC > 0.55)
	{
		m_eCodepageType = ECodepageGB2312;
		dMaxFreq	= dFreqSC;
	}
	if ((m_dwEUCKRCount > MIN_AFFECT_CHAR_COUNT || ((double)m_dwEUCKROccurrenceCharCount/(double)dwTotalccurrenceCharCount) > 0.64) && dFreqEUCKR > 0.55 && dFreqEUCKR > dMaxFreq)
	{
		m_eCodepageType = ECodepageEUCKR;
		dMaxFreq	= dFreqEUCKR;
	}
	if ((m_dwBIG5Count > MIN_AFFECT_CHAR_COUNT || ((double)m_dwBIG5OccurrenceCharCount / (double)dwTotalccurrenceCharCount) > 0.64) && dFreqBIG5 > 0.55 && dFreqBIG5 > dMaxFreq)
	{
		m_eCodepageType = ECodepageBIG5;
		dMaxFreq	= dFreqBIG5;
	}
	if ((m_dwJISCount > MIN_AFFECT_CHAR_COUNT || ((double)m_dwJISOccurrenceCharCount / (double)dwTotalccurrenceCharCount) > 0.64) && dFreqSJIS > 0.55 && dFreqSJIS > dMaxFreq)
	{
		m_eCodepageType = ECodepageSJIS;
		dMaxFreq	= dFreqSJIS;
	}

	if (dMaxFreq > 0.65)
	{
		goto EXIT;
	}


	if (1 == m_dwItems && ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[0]]->Charset())
	{
		if (0 != m_dwCharacterCount)
		{
			dFreqKR = (double)(m_dwKRCount)/(double)(m_dwCharacterCount);
		}			
		if (dFreqKR > 0.805)
		{
			m_eCodepageType = ECodepageEUCKR;
			goto EXIT;
		}
		else if (dFreq >= 0 && dFreq < 0.171)     //while just left gb18030, suit the condition, then set EcodeoageGb2312.
		{
			//m_eCodepageType = ECodepageGB2312;	
		}
		else if (dFreq > 0.34 && dFreq < 1)
		{
			m_eCodepageType = ECodepageBIG5;
			goto EXIT;
		}
	}

	if (dFreq >= 0 && dFreq < 0.171 && true == m_bDone && (ECodepageGB18030 == m_eCodepageType || ECodepageGB2312 == m_eCodepageType))  //m_bDone = True
	{
		//m_eCodepageType = ECodepageGB2312;
		//goto EXIT;
	}
	else if (dFreq > 0.34 && dFreq < 1 && true == m_bDone && ECodepageGB18030 == m_eCodepageType)
	{
		m_eCodepageType = ECodepageBIG5;
		goto EXIT;
	}

	if (0 != (m_dwJISJPCount + m_dwCharacterCount))
	{
		dFreqJIS = (double)(m_dwJISJPCount)/(double)(m_dwJISJPCount + m_dwCharacterCount);
	}
	if (dFreqJIS > 0.9 && m_dwJISJPCount > 40 && (ECodepageSJIS == m_VerifierArray[m_dwItemIdx[0]]->Charset() || ECodepageSJIS == m_VerifierArray[m_dwItemIdx[1]]->Charset()) && m_dwItems <= 2)
	{
		m_eCodepageType = ECodepageSJIS;
		goto EXIT;
	}

	if (m_bDone)
	{
		goto EXIT;
	}

	if (0 != m_dwCharacterCount && m_dwItems <= 3)   //deal with japanese.
	{
		if (0 != m_dwCharacterCount)
		{
			dFreqJP = (double)(m_dwZHJPCount)/(double)(m_dwCharacterCount);
		}
		if (dFreqJP > 0.9 && dFreqJP <= 1)
		{
			m_eCodepageType = ECodepageGB2312;
			goto EXIT;
		}
		if (0 != m_dwCharacterCount)
		{
			dFreqJP = (double)(m_dwKRJPCount + m_dwKRCount)/(double)(m_dwCharacterCount);
		}
		if (dFreqJP > 0.75 && dFreqJP <= 1)
		{
			m_eCodepageType = ECodepageEUCKR;
			goto EXIT;
		}
	}
	else if (0 != m_dwCharacterCount && m_dwItems <= 10)  //japanese only with kana.
	{
		if (0 != m_dwCharacterCount)
		{
			dFreqJP = (double)(m_dwZHJPCount)/(double)(m_dwCharacterCount);
		}
		if (dFreqJP > 0.91)
		{
			m_eCodepageType = ECodepageGB2312;
			goto EXIT;
		}
		if (0 != m_dwCharacterCount)
		{
			dFreqJP = (double)(m_dwKRJPCount + m_dwKRCount)/(double)(m_dwCharacterCount);
		}
		if (dFreqJP > 0.55 && 0 != m_dwKRJPCount)
		{
			m_eCodepageType = ECodepageEUCKR;
			goto EXIT;
		}
		else if (dFreqJP > 0.9)
		{
			m_eCodepageType = ECodepageEUCKR;
			goto EXIT;
		}

	}

	if (m_dwItems == 2)
	{

		if(ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[0]]->Charset() && ECodepageBIG5 == m_VerifierArray[m_dwItemIdx[1]]->Charset())
		{
			if (dFreq >=0 && dFreq < 0.2)
			{
				m_eCodepageType = ECodepageGB2312;
			}
			else if (dFreq >= 0.2)
			{
				m_eCodepageType = ECodepageBIG5;
			}

			if (0 != m_dwCharacterCount)
			{
				dFreqKR = (double)(m_dwKRCount)/(double)(m_dwCharacterCount);
			}	
			if (dFreqKR > 0.84)
			{
				m_eCodepageType = ECodepageEUCKR;
				goto EXIT;
			}
			m_bDone = true;
		}
		else if(ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[1]]->Charset() && ECodepageBIG5 == m_VerifierArray[m_dwItemIdx[0]]->Charset())
		{
			if (dFreq >= 0 && dFreq < 0.2)
			{
				m_eCodepageType = ECodepageGB2312;					
			}
			else if (dFreq >= 0.2)
			{
				m_eCodepageType = ECodepageBIG5;
			}
			if (0 != m_dwCharacterCount)
			{
				dFreqKR = (double)(m_dwKRCount)/(double)(m_dwCharacterCount);
			}	
			if (dFreqKR > 0.84)
			{
				m_eCodepageType = ECodepageEUCKR;
				goto EXIT;
			}
			m_bDone = true;
		}
		else if(ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[0]]->Charset() || ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[1]]->Charset())
		{
			if (dFreq >= 0.41)
			{
				m_eCodepageType = ECodepageBIG5;
			}	
		}

	}
	else if (3 == m_dwItems && 
		(ECodepageBIG5 == m_VerifierArray[m_dwItemIdx[0]]->Charset() || ECodepageBIG5 == m_VerifierArray[m_dwItemIdx[1]]->Charset()|| ECodepageBIG5 == m_VerifierArray[m_dwItemIdx[2]]->Charset()))
	{
		if (dFreq >= 0.3)
		{
			m_eCodepageType = ECodepageBIG5;
			goto EXIT;
		}
		else if (ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[0]]->Charset() || ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[1]]->Charset()|| ECodepageGB18030 == m_VerifierArray[m_dwItemIdx[2]]->Charset())
		{
			if (dFreq < 0.1 && dFreq >= 0)
			{
				m_eCodepageType = ECodepageGB2312;
			}				
		}
	}




	//if (m_bRunSampler)
	//{
	//	Sample(NULL, 0, true);
	//}

EXIT:
	if (ECodepageUNKNOWN != (eTempCodepageType = GetSingleByteCharSet()))
	{
		m_eCodepageType = eTempCodepageType;
	}
	Log("Charset_detector: CodePage = %d", m_eCodepageType);
	m_dwBig5OnlyCount	= 0;
	m_dwCharacterCount	= 0;
	m_fGb2312	= 0.0;
	m_fGbEucKR	= 0.0;
	m_fBig5		= 0.0;
	m_dwKRCount	= 0;
	m_dwZHJPCount	= 0;
	m_dwKRJPCount	= 0;
	m_dwJISJPCount	= 0;	

	m_dwGB2312OccurrenceCharCount	= 0;
	m_dwGB2312Count	= 0;
	m_dwEUCKROccurrenceCharCount	= 0;
	m_dwEUCKRCount	= 0;
	m_dwJISOccurrenceCharCount		= 0;
	m_dwJISCount	= 0;
	m_dwBIG5OccurrenceCharCount		= 0;
	m_dwBIG5Count	= 0;
}

ECodepageType  CCharsetDetector::GetSingleByteCharSet()
{
	ECodepageType  eCodepageType = ECodepageUNKNOWN;
	if (EDetectStateMe == m_pSMUTF_8Info->dwDetectState)
	{
		eCodepageType = ECodepageUTF8;
		goto EXIT;
	}
	//only affect big5.
	if (m_eCodepageType == ECodepageBIG5 && EDetectStateMe == m_pSMUTF_8Info->dwDetectState)
		eCodepageType = ECodepageUTF8;
	if (m_eCodepageType == ECodepageGB2312 || m_eCodepageType == ECodepageSJIS
		||m_eCodepageType == ECodepageEUCKR || m_eCodepageType == ECodepageBIG5)
	{
		eCodepageType =  ECodepageUNKNOWN;
		goto EXIT;
	}

	if (EDetectStateMe == m_pSBWIN_1251Info->dwState)
		eCodepageType = ECodepageWIN1251;
	else if (EDetectStateMe == m_pSBISO8859_2Info->dwState)
		eCodepageType = ECodepageISO8859_2;
	else if (EDetectStateMe == m_pSBWIN_1250Info->dwState)
		eCodepageType = ECodepageWIN1250;
	else if (EDetectStateMe == m_pSBWIN_1252Info->dwState)
		eCodepageType = ECodepageCP1252;
	else if (EDetectStateMe == m_pSBWIN_1253Info->dwState )
		eCodepageType = ECodepageWIN1253;
	else if (EDetectStateMe == m_pSBISO8859_7Info->dwState)
		eCodepageType = ECodepageISO8859_7;
	else if (EDetectStateMe == m_pSBKOIR_8Info->dwState)
		eCodepageType = ECodepageKOI8R;
	else   /*Check the fconfidence*/
	{
		/*Europe: windows-1250, windows-1252, iso-8859-2*/
		if (m_pSBISO8859_2Info->fConfidence >= BOTTOM_CONFIDENCE || m_pSBWIN_1250Info->fConfidence >= BOTTOM_CONFIDENCE)
		{
			/*0.004 is a experience value*/
			if (m_pSBISO8859_2Info->fConfidence - m_pSBWIN_1250Info->fConfidence < (float)0.004)
				eCodepageType = ECodepageCP1252;
			else if (m_dwWINvsISO8859Count > 0)
				eCodepageType = ECodepageWIN1250;
			else if (m_dwWINvsISO8859Count == 0)
				eCodepageType = ECodepageISO8859_2;
		}/*When capital letter is so many, useful*/
		else if (m_pSBWIN_1252Info->fConfidence >= (float)0.968)
		{
			eCodepageType = ECodepageCP1252;
		}

		/*Greek: windows-1253, iso-8859-7*/
		if (m_pSBWIN_1253Info->fConfidence >= GREEK_BOTTOM_CONFIDENCE || m_pSBISO8859_7Info->fConfidence >= GREEK_BOTTOM_CONFIDENCE)
		{
			if (m_dwWINvsISO8859Count > 0)
				eCodepageType = ECodepageWIN1253;
			else if (m_dwWINvsISO8859Count == 0)
				eCodepageType = ECodepageISO8859_7;
		}

		/*Russian: Koi8-r, windows-1251*/
		if (m_pSBWIN_1251Info->fConfidence >= RUSSIAN_BOTTOM_CONFIDENCE || m_pSBKOIR_8Info->fConfidence >= RUSSIAN_BOTTOM_CONFIDENCE)
		{
			if (m_pSBWIN_1251Info->fConfidence > m_pSBKOIR_8Info->fConfidence)
				eCodepageType = ECodepageWIN1251;
			else
				eCodepageType = ECodepageKOI8R;
		}
	}
	goto EXIT;

EXIT:
	Log("GetSingleByteCharSet()::Out, eCodepageType = %d", eCodepageType);
	return eCodepageType;
}

long	CCharsetDetector::Guess()
{
	long			lResult				= ERR_UNSUPPORTED;
	ECodepageType	eCodepageType		= ECodepageUNKNOWN;
	ECodepageType	eTempCodepageType	= ECodepageUNKNOWN;

	if (m_dwItems <= 0) 
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	//15 == verifier array size
	if(15 == m_dwItems)
	{
		eCodepageType = ECodepageASCII;

		lResult = ERR_NONE;
		goto EXIT;
	}
	if(m_dwItems >= 12)
	{
		eCodepageType = ECodepageASCII;

		lResult = ERR_NONE;
		goto EXIT;
	}

	for (long i = 0; i < m_dwItems; ++i)
	{
		eTempCodepageType = m_VerifierArray[m_dwItemIdx[i]]->Charset();

		if(	ECodepageUNKNOWN == eTempCodepageType ||
			ECodepageTypeCount == eTempCodepageType ||
			ECodepageGB18030 == eTempCodepageType) //SPECIAL for GB18030.
		{
			continue;
		}

		eCodepageType = eTempCodepageType;
		lResult = ERR_NONE;
		goto EXIT;
	}
	if(lResult)
		goto EXIT;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		m_eCodepageType = ECodepageUNKNOWN;
	}
	else
	{
		m_eCodepageType = eCodepageType;
	}

	return lResult;
}
