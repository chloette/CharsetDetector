#include "charset_detector_common_inc.h"
#include "i_codepage_detector.h"

ICodepageDetector::ICodepageDetector()
{

}

ICodepageDetector::~ICodepageDetector()
{

}

long	ICodepageDetector::DetectCodepage(const char* szFilePath, ECodepageType* peType)
{
	Log("ICodepageDetector::DetectCodepage IN ");

	long	lResult					    = ERR_UNSUPPORTED;
	FILE*	hFile								= NULL;
	long	lSize								= 0;
	char*	pBuffer						    = NULL;
	char*  pPoint                            = NULL;
	unsigned long dwParserType                  = 0;

	if(	NULL == szFilePath ||
		NULL == peType)
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	Log("ICodepageDetector::DetectCodepage HERE 1");

	hFile = fopen(szFilePath, "r");
	if(NULL == hFile)
	{
		lResult = ERR_BAD_STATE;
		goto EXIT;
	}

	fseek(hFile, 0L, SEEK_END);
	lSize = ftell(hFile);
	if(0 >= lSize)
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	fseek(hFile, 0L, 0);

	Log("ICodepageDetector::DetectCodepage HERE 2 lSize = %d", lSize);

	//re-size
	lSize = lSize > DEFAULT_MAX_STREAM_BUFFER_SIZE ? DEFAULT_MAX_STREAM_BUFFER_SIZE : lSize;

	pBuffer = (char*)malloc(sizeof(char) * lSize);
	if(NULL == pBuffer)
	{
		lResult = ERR_NO_MEMORY;
		goto EXIT;
	}

	if(lSize != fread(pBuffer, lSize, 1, hFile))
	{
		lResult = ERR_BAD_STATE;
		goto EXIT;
	}
	
	//find the '.' in the full path
	pPoint = strchr(szFilePath, '.');
	if(NULL != pPoint)
		if (0 == strcasecmp(pPoint, ".smi"))
			dwParserType = 5;//5 is eParserTypeSmi.

	Log("ICodepageDetector::DetectCodepage HERE 3");

	lResult = DetectCodepage(dwParserType, pBuffer, lSize, peType);
	if(lResult)
		goto EXIT;

	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{

	}

	if(hFile)
	{
		fclose(hFile);
		hFile = NULL;
	}

	if(pBuffer)
	{
		free(pBuffer);
		pBuffer = NULL;
	}

	Log("ICodepageDetector::DetectCodepage OUT lResult = %d", lResult);

	return lResult;
}

long	ICodepageDetector::DetectCodepage(const char* szFilePath, char* szCharSet, unsigned long dwLength)
{
	long			lResult		= ERR_UNSUPPORTED;
	ECodepageType	eType		= ECodepageUNKNOWN;
	
	if(	NULL == szFilePath ||
		NULL == szCharSet ||
		dwLength < DEFAULT_CHARSET_LENGTH)
	{
		lResult = ERR_INVALID_PARAM;
		goto EXIT;
	}

	lResult = DetectCodepage(szFilePath, &eType);
	if(lResult)
		goto EXIT;

	switch(eType)
	{
	case ECodepageASCII:
		strcpy(szCharSet, ("US-ASCii"));
		break;
	case ECodepageBIG5:
		strcpy(szCharSet, ("Big5"));
		break;
	case ECodepageCP1252:
		strcpy(szCharSet, ("windows-1252"));
		break;
	case ECodepageEUCJP:
		strcpy(szCharSet, ("EUC-JP"));
		break;
	case ECodepageEUCKR:
		strcpy(szCharSet, ("EUC-KR"));
		break;
	case ECodepageEUCTW:
		strcpy(szCharSet, ("x-euc-tw"));
		break;
	case ECodepageGB2312:
		strcpy(szCharSet, ("GB2312"));
		break;
	case ECodepageGB18030:
		strcpy(szCharSet, ("GB18030"));
		break;
	case ECodepageHZ:
		strcpy(szCharSet, ("HZ-GB-2312"));
		break;
	case ECodepageISO2022CN:
		strcpy(szCharSet, ("ISO-2022-CN"));
		break;
	case ECodepageISO2022JP:
		strcpy(szCharSet, ("ISO-2022-JP"));
		break;
	case ECodepageISO2022KR:
		strcpy(szCharSet, ("ISO-2022-KR"));
		break;
	case ECodepageSJIS:
		strcpy(szCharSet, ("Shift_JIS"));
		break;
	case ECodepageUTF8:
		strcpy(szCharSet, ("UTF-8"));
		break;
	case ECodepageUCS2BE:
		strcpy(szCharSet, ("UTF-16BE"));
		break;
	case ECodepageUCS2LE:
		strcpy(szCharSet, ("UTF-16LE"));
		break;
	case ECodepageUCS4BE:
		strcpy(szCharSet, ("UTF-32BE"));
		break;
	case ECodepageUCS4LE:
		strcpy(szCharSet, ("UTF-32LE"));
		break;
	case ECodepageKOI8R:
		strcpy(szCharSet, ("KOI8-5"));
		break;
	case ECodepageWIN1251:
		strcpy(szCharSet, ("WIN-1251"));
		break;
	case ECodepageISO8859_2:
		strcpy(szCharSet, ("ISO-8859-2"));
		break;
	case ECodepageWIN1250:
		strcpy(szCharSet, ("WIN-1250"));
		break;
	case ECodepageUNKNOWN:
	default:
		strcpy(szCharSet, DEFAULT_CODEPAGE_UNKNOWN);
		break;
	}
	
	lResult = ERR_NONE;
EXIT:
	if(lResult)
	{
		strcpy(szCharSet, DEFAULT_CODEPAGE_UNKNOWN);
	}

	return lResult;
}

long	ICodepageDetector::SetConfig(unsigned long dwCfgType, void* pData)
{
	long lResult = ERR_UNSUPPORTED;

	switch(dwCfgType)
	{
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
