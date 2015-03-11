/*
* charset_detector.h
* 
* In this file, the interface of Charset Detector is implemented.
*
*/

#ifndef _CHARSET_DETECTOR_H_
#define _CHARSET_DETECTOR_H_

#define MAX_CHARSET_VERIFIERS_COUNT 16

#include "i_charset_verifier.h"
#include "i_euc_statistics.h"
#include "euc_sampler.h"
#include "i_codepage_detector.h"
#include "character_order.h"

//NOTICE:
//Actually that I even dont't know the total implement detail of code below. 
//What I know just its target and a little detail of it through guessing.

#define SAMPLE_SIZE 64
#define SB_ENOUGH_REL_THRESHOLD  1024
#define SB_DIVISIBLE_UNIT  100
#define POSITIVE_SHORTCUT_THRESHOLD  (float)0.80
#define NEGATIVE_SHORTCUT_THRESHOLD  (float)0.05
#define SYMBOL_CAT_ORDER  250
#define NUMBER_OF_SEQ_CAT 4
#define POSITIVE_CAT   (NUMBER_OF_SEQ_CAT-1)
#define NEGATIVE_CAT   0
#define BOTTOM_CONFIDENCE   (float)0.507//experience value.
#define RUSSIAN_BOTTOM_CONFIDENCE   (float)0.488 //experience value.
#define GREEK_BOTTOM_CONFIDENCE   (float)0.44//experience value.

;//The compiler want this semicolon.
typedef struct
{
	unsigned long   dwTotalCharacters;
	unsigned long   dwFrqCharCount;
	unsigned long   dwTotalSquence;
	unsigned long   dwSeqCountersMatrix[NUMBER_OF_SEQ_CAT];
	unsigned char    dwLastOrder;
	unsigned long   dwState;
	float     fConfidence;
	/*SingleByteInfo():dwTotalCharacters(0)*/
}SingleByteInfo;

typedef struct
{
	unsigned long   dwDetectState;
	unsigned long   dwSMState;
	unsigned long   dwCurrentBytePos;
	unsigned long   dwCurrentCharLen;
	unsigned long   dwTotalMBChar;
	float     fConfidence;
}SMByteInfo;

class CCharsetDetector
{
public:
	CCharsetDetector();

	virtual	~CCharsetDetector();

	long	Init();

	long	Destroy();

public:

	void Reset();

	bool DoIt(unsigned long dwParserType, char* pBuf, unsigned long dwLength);

	ECodepageType	GetResultCharset();

	bool	CanGuess();

	long	EnableGuess(bool bCanGuess);


protected:
	long InitVerifiers();

	void Sample(char* pBuf, unsigned long dwLength);

	void Sample(char* pBuf, unsigned long dwLength, bool bLastChance);

	unsigned long DetectSingleByteCodepage(const SequenceModel* const pSquModel, unsigned char Uint8Current, SingleByteInfo* pTemSingleByteInfo);

	float GetSingleByteConfidence(const SequenceModel* const  pSquModel,unsigned long dwSeqCountersMatrix[], unsigned long dwTotalChar, unsigned long dwFreqChar, unsigned long dwTotalSeqs);

	unsigned long DetectWIN1252Codepage(const SequenceModel* const pSquModel, unsigned char Uint8Current, SingleByteInfo* pTemSingleByteInfo);

	unsigned long DetectCodingStateMachine(const SMModel*const pSMModel, unsigned char Uint8Current,SMByteInfo* pTemSMByteInfo);

	unsigned long NextStateMachineState(const SMModel* const pSMModel, unsigned char Uint8Current, SMByteInfo* pTemSMByteInfo);

public:

	bool	IsDone();

	void	Done();

	long	Guess();

private:
	void SetSingleByte(SingleByteInfo* pTempSingleByteInfo);
	void SetSMByte(SMByteInfo* pTempSMByteInfo);
	ECodepageType  GetSingleByteCharSet();
	unsigned long IsSamiNoise(char* pBuf, unsigned long dwCurrentBuf, unsigned long dwLength);
	unsigned long IsMicroDvdNoise(char* pBuf, unsigned long dwCurrentBuf, unsigned long dwLength);
	bool   IsTextUTF8( char* lpstrInputStream, unsigned long iLen );

private:
	ICharsetVerifier*	m_VerifierArray[MAX_CHARSET_VERIFIERS_COUNT];
	IEUCStatistics*		m_StatisticsArray[MAX_CHARSET_VERIFIERS_COUNT];

	CEUCSampler*		m_pSampler;

	unsigned char	m_State[MAX_CHARSET_VERIFIERS_COUNT];
	unsigned long	m_dwItemIdx[MAX_CHARSET_VERIFIERS_COUNT];
	unsigned long	m_dwItems;
	unsigned long	m_dwClassItems;
	bool	m_bDone;
	bool	m_bRunSampler;
	bool	m_bClassRunSampler;
	
	ECodepageType	m_eCodepageType;

	bool	m_bCanGuess;

	double	m_fGb2312;
	double	m_fGbEucKR;
	double	m_fBig5;

	unsigned long	m_dwBig5OnlyCount;
	unsigned long	m_dwCharacterCount;
	bool	m_bSecondChar;
	unsigned long	m_dwKRCount;
	unsigned long	m_dwZHJPCount;
	unsigned long	m_dwKRJPCount;
	unsigned long	m_dwJISJPCount;
	
	unsigned long	m_dwGB2312OccurrenceCharCount;
	unsigned long	m_dwGB2312Count;

	unsigned long	m_dwEUCKROccurrenceCharCount;
	unsigned long	m_dwEUCKRCount;

	unsigned long	m_dwJISOccurrenceCharCount;
	unsigned long	m_dwJISCount;

	unsigned long	m_dwBIG5OccurrenceCharCount;
	unsigned long	m_dwBIG5Count;

	unsigned long m_dwWINvsISO8859Count;
	
	SingleByteInfo* m_pSBKOIR_8Info;	
	SingleByteInfo* m_pSBWIN_1251Info;
	SingleByteInfo* m_pSBISO8859_2Info;
	SingleByteInfo* m_pSBWIN_1250Info;
	SingleByteInfo* m_pSBWIN_1252Info;
	SingleByteInfo* m_pSBWIN_1253Info;
	SingleByteInfo* m_pSBISO8859_7Info;
	SMByteInfo*     m_pSMUTF_8Info;

};
#endif //_CHARSET_DETECTOR_H_
