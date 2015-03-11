CharsetDetector
===============

##About
`CharsetDetector` is an android library which can detect charset, based on [cpdetector](http://cpdetector.sourceforge.net/index.shtml), supporting ***detect charset*** easily. It includes Java part, JNI part and C part.

[BTW, this project was closed several years ago.]

##What's charset
* [Wiki](http://en.wikipedia.org/wiki/Character_encoding)
* [中文可以点这里](http://nobodycare.me/2014/12/31/about-charset/)

##Why not jchardet/cpdetector
* ***Better correct recognition rate.*** (Optimized for subtitles and some specific charsets)
* Of course, you can use it as "jchardet/cpdetector on Android" directly.

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
