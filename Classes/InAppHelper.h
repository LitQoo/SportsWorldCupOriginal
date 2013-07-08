#pragma once



#include "InAppDelegator.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "jni.h"
#include "platform/android/jni/JniHelper.h"


class InAppHelper
{
private:
	JniMethodInfo minfo;
	jobject jobj;
	
	string className;
public:
	
	void setJNI(string _className);
	void releaseJNI();
	void callJava_buyItem(string itemID, CCObject* target, RecvInAppSelType selector);
	string callJava_getPrice(string itemID);
	
public:
	static InAppHelper* getInstance()
	{
		static InAppHelper* _ins = 0;
		if(_ins == 0)
			_ins = new InAppHelper();
		return _ins;
	}
private:
	InAppHelper()
	{
		setJNI("com.litqoo.SportsWorldCup.SportsWorldCup");
	}
	~InAppHelper()
	{
	}
};



#endif
