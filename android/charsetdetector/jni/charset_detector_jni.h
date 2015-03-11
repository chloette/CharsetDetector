#ifndef __INCLUDE_CHARSET_DETECTOR_JNI_H__
#define __INCLUDE_CHARSET_DETECTOR_JNI_H__

#define  _JNI(x)  Java_com_android_charsetdetector_CharsetDetector_##x

#if defined(__cplusplus) || defined(c_plusplus)
extern "C" {
#endif

  jint _JNI(nativeDetectCharset)(JNIEnv* env, jobject thiz, jstring filePath);

#if defined(__cplusplus) || defined(c_plusplus)
}
#endif  //__cplusplus__

#endif //__INCLUDE_CHARSET_DETECTOR_JNI_H__
