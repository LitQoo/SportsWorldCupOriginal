#pragma once



#include "URLDelegator.h"


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni.h"
#include "platform/android/jni/JniHelper.h"


class JNIKelper
{
private:
	JniMethodInfo minfo;
	jobject jobj;
	
	string className;
public:
	
	void setJNI(string _className);
	void releaseJNI();
	void callJava_simple(string functionName);
	string callJava_getUDID();
	string callJava_getDeviceInfo();
	void callJava_otherApp(string packName);
	void callJava_moreApp(string company);

	string callJava_getUTF8String(string _strkey);
public:
	static JNIKelper* getInstance()
	{
		static JNIKelper* _ins = 0;
		if(_ins == 0)
			_ins = new JNIKelper();
		return _ins;
	}
private:
	JNIKelper()
	{
		setJNI("com.litqoo.lib");
	}
	~JNIKelper()
	{
	}
};



#endif
