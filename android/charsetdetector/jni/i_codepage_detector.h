/*
* i_codepage_detector.h
* 
* In this file, the interface of CodepageDetector is implemented.
*
*/

#ifndef _I_CODEPAGE_DETECTOR_H_
#define _I_CODEPAGE_DETECTOR_H_

#define DEFAULT_CHARSET_LENGTH			32
#define DEFAULT_MAX_STREAM_BUFFER_SIZE	8192

#define DEFAULT_CODEPAGE_UNKNOWN  ("UNKNOWN")

typedef enum _tag_codepage_type
{
	ECodepageASCII,
	ECodepageBIG5,
	ECodepageCP1252,
	ECodepageEUCJP,
	ECodepageEUCKR,
	ECodepageEUCTW,
	ECodepageGB2312,
	ECodepageGB18030,
	ECodepageHZ,
	ECodepageISO2022CN,
	ECodepageISO2022JP,
	ECodepageISO2022KR,
	ECodepageSJIS,
	ECodepageUTF8,
	ECodepageUCS2BE,
	ECodepageUCS2LE,
	ECodepageUCS4BE,
	ECodepageUCS4LE,
	ECodepageKOI8R,
	ECodepageWIN1251,
	ECodepageISO8859_2,
	ECodepageWIN1250,
	ECodepageWIN1253,
	ECodepageISO8859_7,
	ECodepageUNKNOWN,
	ECodepageTypeCount
} ECodepageType;

#define CODEPAGE_DETECTOR_CONFIG_BASE			(0x1987)
#define CODEPAGE_DETECTOR_CONFIG_ENABLE_GUESS	(CODEPAGE_DETECTOR_CONFIG_BASE + (1))	//pData = bool bCanGuess

class ICodepageDetector
{	
public:
	ICodepageDetector();
	virtual	~ICodepageDetector();

public:
	virtual	long	DetectCodepage(const char* szFilePath, ECodepageType* peType);

	virtual	long	DetectCodepage(const char* szFilePath, char* szCharSet, unsigned long dwLength);

	virtual	long	DetectCodepage(unsigned long dwParserType, char* pBuffer, unsigned long dwBufLength, ECodepageType* peType) = 0;

	virtual	long	SetConfig(unsigned long dwCfgType, void* pData);
};

#endif //_I_CODEPAGE_DETECTOR_H_
