# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CFLAGS 		+= -D__STDC_CONSTANT_MACROS #-D__cplusplus

LOCAL_MODULE    :=  charsetdetector

LOCAL_SRC_FILES :=  charset_detector_jni.cpp \
                    codepage_detector_proxy.cpp \
                    i_charset_verifier.cpp \
                    i_codepage_detector.cpp \
                    charset_detector.cpp \
                    i_euc_statistics.cpp \
                    ascii_detector.cpp \
                    big5_statistics.cpp \
                    big5_verifier.cpp \
                    cp1252_verifier.cpp \
                    euc_sampler.cpp \
                    euc_jp_statistics.cpp \
                    euc_jp_verifier.cpp \
                    euc_kr_statistics.cpp \
                    euc_kr_verifier.cpp \
                    euc_tw_statistics.cpp \
                    euc_tw_verifier.cpp \
                    gb2312_statistics.cpp \
                    gb2312_verifier.cpp \
                    gb18030_verifier.cpp \
                    hz_verifier.cpp \
                    iso2022_cn_verifier.cpp \
                    iso2022_jp_verifier.cpp \
                    iso2022_kr_verifier.cpp \
                    jchar_detector.cpp \
                    sjis_verifier.cpp \
                    ucs2be_verifier.cpp \
                    ucs2le_verifier.cpp \
                    unicode_detector.cpp \
                    utf8_verifier.cpp
										
LOCAL_SHARED_LIBRARIES := 

LOCAL_LDLIBS    := -llog -lz -lm

LOCAL_C_INCLUDES := $(LOCAL_PATH)

include $(BUILD_SHARED_LIBRARY)