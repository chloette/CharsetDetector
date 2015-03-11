CharsetDetector
===============

##About
`CharsetDetector` is an android library which can detect charset, based on [jchardet](http://lxr.mozilla.org/mozilla/source/intl/chardet/), supporting ***detect charset*** easily. It includes Java part, JNI part and C part.

[BTW, this project was closed several years ago.]

##Why not jchardet 
* ***Better correct recognition rate.*** (Optimized for subtitles and some specific charsets)
* Of course, you can use it as "jchardet on Android" directly.

##How to use
####Sample code
`CharsetDetector.Codepage codepage = CharsetDetector.getInstance().detect("/sdcard/gb2312.txt");`

####JNI
* Makesure you know what NDK is.
* Then just `cd` to android/charsetdetector/jni
* Run `ndk-build -B`
* That's all, native libraries are ready (under android/charsetdetector/libs/armeabi-v7a).

##Contact
mailto: chloette.e@gmail.com

Have fun~ ^_^
