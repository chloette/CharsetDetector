/*
* charset_detector_common_inc.h
* 
* In this file, the common head files needed by Charset Detector is implemented.
*
*/

#ifndef _CHARSET_DETECTOR_COMMON_INC_H_
#define _CHARSET_DETECTOR_COMMON_INC_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define ERR_NONE						0
#define RESULT_OK						0

#define ERR_BASIC_BASE					0X0001
#define ERR_UNKNOWN						(ERR_BASIC_BASE)
#define ERR_INVALID_PARAM				(ERR_BASIC_BASE+1)
#define ERR_UNSUPPORTED					(ERR_BASIC_BASE+2)
#define ERR_NO_MEMORY					(ERR_BASIC_BASE+3)
#define ERR_BAD_STATE					(ERR_BASIC_BASE+4)

#ifdef DEBUG
	#ifdef __ANDROID__
		#define LOG_TAG "CHARSET_DETECTOR"
		#include <utils/Log.h>
		#define Log LOGD
	#else
		#define  Log
	#endif //_LOGGER_
#else
	#define Log
#endif

#endif //_CHARSET_DETECTOR_COMMON_INC_H_

