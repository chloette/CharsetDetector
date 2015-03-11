package com.android.charsetdetector;

import android.util.Log;

public class CharsetDetector {
    public static final String TAG = CharsetDetector.class.getSimpleName();

    static {
        try {
            System.loadLibrary("charsetdetector");
        } catch (UnsatisfiedLinkError e) {
            e.printStackTrace();
            Log.e(TAG, "PLEASE MAKE SURE ALL LIBRARIES WERE PLACED!");
            throw e;
        }
    }

    private CharsetDetector() {

    }

    private static CharsetDetector INSTANCE = null;

    public static CharsetDetector getInstance() {
        if(INSTANCE == null) {
            INSTANCE = new CharsetDetector();
        }

        return INSTANCE;
    }

    public enum Codepage {
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
    };

    protected native int nativeDetectCharset(String file);

    public Codepage detect(String file) {
        try {
            int codepage = nativeDetectCharset(file);

            return Codepage.values()[codepage];
        } catch (Exception e) {
            e.printStackTrace();
            return Codepage.ECodepageUNKNOWN;
        }
    }

}