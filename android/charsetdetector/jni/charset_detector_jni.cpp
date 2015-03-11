/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include <string.h>
#include <jni.h>
#include "charset_detector_jni.h"
#include "codepage_detector_proxy.h"
#include "i_codepage_detector.h"
#include "charset_detector_common_inc.h"

static const char* getString(JNIEnv* env, jstring string) {
#ifdef __ANDROID__
  if(!string)
    return NULL;

  return env->GetStringUTFChars(string, 0);
#else
  return (const char*)string; //should be char*
#endif
}

static void releaseString(JNIEnv* env, jstring string, const char* array) {
#ifdef __ANDROID__
  env->ReleaseStringUTFChars(string, array);
#else
  return;
#endif
}

static jbyte* getByteArray(JNIEnv* env, jbyteArray data) {
#ifdef __ANDROID__
  if(!data)
    return NULL;

  return env->GetByteArrayElements(data, 0);
#else
  return data.data; //should be char*
#endif
}

static jsize getByteArrayLength(JNIEnv* env, jbyteArray data) {
#ifdef __ANDROID__
  return env->GetArrayLength(data);
#else
  return data.length; //should be char*
#endif
}

static void releaseByteArray(JNIEnv* env, jbyteArray data, jbyte* array) {
#ifdef __ANDROID__
  env->ReleaseByteArrayElements(data, array, 0);
#else
  return;
#endif
}

jint _JNI(nativeDetectCharset)(JNIEnv* env, jobject thiz, jstring filePath) {
  int res = -1;
  ECodepageType type = ECodepageUNKNOWN;
  const char* pFilePath = NULL;

#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#define ABI "armeabi-v7a/NEON"
#else
#define ABI "armeabi-v7a"
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__mips__)
#define ABI "mips"
#else
#define ABI "unknown"
#endif

  Log("%s %d E   compiled with ABI %s \n", __func__, __LINE__, ABI);

  pFilePath = getString(env, filePath);

  res = CCodepageDetectorProxy::GetInstance(true)->DetectCodepage(pFilePath, &type);
  if(res)
    goto EXIT;

  res = 0;
EXIT:
  if(res) {
    type = ECodepageUNKNOWN;
  }

  releaseString(env, filePath, pFilePath);

  return type;
}
